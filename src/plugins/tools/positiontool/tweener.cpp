/***************************************************************************
 *   Project TUPI: Magia 2D                                                *
 *   Project Contact: info@maefloresta.com                                 *
 *   Project Website: http://www.maefloresta.com                           *
 *   Project Leader: Gustav Gonzalez <info@maefloresta.com>                *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 *
 *    Gustavo Gonzalez / xtingray                                          *
 *                                                                         *
 *   KTooN's versions:                                                     * 
 *                                                                         *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       *
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Gustav Gonzalez - http://www.maefloresta.com       *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "tweener.h"
#include "configurator.h"
#include "tglobal.h"
#include "tdebug.h"
#include "taction.h"
#include "tnodegroup.h"
#include "tosd.h"
#include "tupsvg2qt.h"
#include "tupinputdeviceinformation.h"
#include "tupbrushmanager.h"
#include "tupgraphicsscene.h"
#include "tupgraphicobject.h"
#include "tuppathitem.h"
#include "tupellipseitem.h"
#include "tuprectitem.h"
#include "tuplineitem.h"
#include "tupsvgitem.h"
#include "tupitemtweener.h"
#include "tuprequestbuilder.h"
#include "tupprojectrequest.h"
#include "tuplibraryobject.h"
#include "tupscene.h"
#include "tuplayer.h"

#include <QPointF>
#include <QKeySequence>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QMatrix>
#include <QGraphicsLineItem>
#include <QGraphicsView>
#include <QDomDocument>

struct Tweener::Private
{
    QMap<QString, TAction *> actions;
    Configurator *configurator;

    TupGraphicsScene *scene;
    QGraphicsPathItem *path;
    QList<QGraphicsItem *> objects;

    TupItemTweener *currentTween;
    TNodeGroup *group;

    bool pathAdded;
    int startPoint;

    TupToolPlugin::Mode mode;
    // Settings::EditMode editMode;
    TupToolPlugin::EditMode editMode;

    QPointF itemObjectReference;
    QPointF pathOffset;
    QPointF firstNode;
};

Tweener::Tweener() : TupToolPlugin(), k(new Private)
{
    setupActions();

    k->configurator = 0;
    k->path = 0;
    k->group = 0;
    k->startPoint = 0;
    k->currentTween = 0;
}

Tweener::~Tweener()
{
    delete k;
}

/* This method initializes the plugin */

void Tweener::init(TupGraphicsScene *scene)
{
    #ifdef K_DEBUG
           T_FUNCINFO;
    #endif

    delete k->path;
    k->path = 0;
    delete k->group;
    k->group = 0;
    k->scene = scene;
    k->objects.clear();

    k->pathAdded = false;
    k->pathOffset = QPointF(0, 0); 
    k->firstNode = QPointF(0, 0);
    k->itemObjectReference = QPointF(0, 0);

    k->mode = TupToolPlugin::View;
    k->editMode = TupToolPlugin::None;

    k->configurator->resetUI();

    QList<QString> tweenList = k->scene->scene()->getTweenNames(TupItemTweener::Position);
    if (tweenList.size() > 0) {
        k->configurator->loadTweenList(tweenList);
        setCurrentTween(tweenList.at(0));
    }

    int total = framesTotal();
    k->configurator->initStartCombo(total, k->scene->currentFrameIndex());
}

void Tweener::updateStartPoint(int index)
{
    if (k->startPoint != index && index >= 0) 
        k->startPoint = index;
}

/* This method returns the plugin name */

QStringList Tweener::keys() const
{
    return QStringList() << tr("Position Tween");
}

/* This method makes an action when the mouse is pressed on the workspace 
 * depending on the active mode: Selecting an object or Creating a path  
*/

void Tweener::press(const TupInputDeviceInformation *input, TupBrushManager *brushManager, TupGraphicsScene *scene)
{
    #ifdef K_DEBUG
           T_FUNCINFO;
    #endif

    Q_UNUSED(brushManager);
    Q_UNUSED(scene);

    if (k->editMode == TupToolPlugin::Path && k->scene->currentFrameIndex() == k->startPoint) {
        if (k->path) {
            QPointF point = k->path->mapFromParent(input->pos());
            QPainterPath path = k->path->path();
            path.cubicTo(point, point, point);
            k->path->setPath(path);
        }
    } 
}

/* This method is executed while the mouse is pressed and on movement */

void Tweener::move(const TupInputDeviceInformation *input, TupBrushManager *brushManager, TupGraphicsScene *scene)
{
    Q_UNUSED(input);
    Q_UNUSED(brushManager);
    Q_UNUSED(scene);
}

/* This method finishes the action started on the press method depending
 * on the active mode: Selecting an object or Creating a path
*/

void Tweener::release(const TupInputDeviceInformation *input, TupBrushManager *brushManager, TupGraphicsScene *scene)
{
    #ifdef K_DEBUG
           T_FUNCINFO;
    #endif

    Q_UNUSED(input);
    Q_UNUSED(brushManager);

    if (scene->currentFrameIndex() == k->startPoint) {

        if (k->editMode == TupToolPlugin::Path) {

            if (k->group) {
                k->group->createNodes(k->path);
                k->group->expandAllNodes();
                k->configurator->updateSteps(k->path);
                QPainterPath::Element e  = k->path->path().elementAt(0);
                QPointF begin = QPointF(e.x, e.y);

                if (begin != k->firstNode) {
                    QPointF oldPos = k->firstNode;
                    QPointF newPos = begin;

                    int distanceX = newPos.x() - oldPos.x();
                    int distanceY = newPos.y() - oldPos.y();

                    if (k->objects.size() > 0) {
                        foreach (QGraphicsItem *item, k->objects) {
                                 item->moveBy(distanceX, distanceY);
                        }
                        QGraphicsItem *item = k->objects.at(0);
                        QRectF rect = item->sceneBoundingRect();
                        k->itemObjectReference = rect.center();
                    }

                    k->firstNode = newPos;
                }
            }

        } else {

            if (scene->selectedItems().size() > 0) {

                k->objects = scene->selectedItems();
                k->configurator->notifySelection(true);

                QGraphicsItem *item = k->objects.at(0);
                QRectF rect = item->sceneBoundingRect();
                QPointF newPos = rect.center();
                QPointF oldPos = k->itemObjectReference;
                k->itemObjectReference = newPos;

                if (!k->path) {
                    k->path = new QGraphicsPathItem;
                    k->path->setZValue(maxZValue());

                    QColor color = Qt::lightGray;
                    color.setAlpha(200);
                    QPen pen(QBrush(color), 1, Qt::DotLine);
                    k->path->setPen(pen);

                    QPainterPath path;
                    path.moveTo(newPos);
                    k->firstNode = newPos;
                    k->path->setPath(path);
                    scene->addItem(k->path);
                    k->pathAdded = true;

                    k->pathOffset = QPointF(0, 0);
                } else {
                    int distanceX = newPos.x() - oldPos.x();
                    int distanceY = newPos.y() - oldPos.y();
                    k->path->moveBy(distanceX, distanceY);

                    k->pathOffset = QPointF(distanceX, distanceY);
                }
            } 
        }
    } 
}

/* This method returns the list of actions defined in this plugin */

QMap<QString, TAction *> Tweener::actions() const
{
    return k->actions;
}

/* This method returns the list of actions defined in this plugin */

int Tweener::toolType() const
{
    return TupToolInterface::Tweener;
}

/* This method returns the tool panel associated to this plugin */

QWidget *Tweener::configurator()
{
    if (!k->configurator) {
        k->mode = TupToolPlugin::View;

        k->configurator = new Configurator;

        connect(k->configurator, SIGNAL(startingPointChanged(int)), this, SLOT(updateStartPoint(int)));
        connect(k->configurator, SIGNAL(clickedCreatePath()), this, SLOT(setCreatePath()));
        connect(k->configurator, SIGNAL(clickedSelect()), this, SLOT(setSelect()));
        connect(k->configurator, SIGNAL(clickedRemoveTween(const QString &)), this, SLOT(removeTween(const QString &)));
        connect(k->configurator, SIGNAL(clickedResetInterface()), this, SLOT(applyReset()));
        connect(k->configurator, SIGNAL(setMode(TupToolPlugin::Mode)), this, SLOT(updateMode(TupToolPlugin::Mode)));
        connect(k->configurator, SIGNAL(clickedApplyTween()), this, SLOT(applyTween()));
        // connect(k->configurator, SIGNAL(editModeOn()), this, SLOT(setEditEnv())); 
        connect(k->configurator, SIGNAL(getTweenData(const QString &)), this, SLOT(setCurrentTween(const QString &)));

    } else {
        k->mode = k->configurator->mode();
    }

    return k->configurator;
}

/* This method is called when there's a change on/of scene */
void Tweener::aboutToChangeScene(TupGraphicsScene *)
{
}

/* This method is called when this plugin is off */

void Tweener::aboutToChangeTool()
{
    if (k->editMode == TupToolPlugin::Selection) {
        clearSelection();
        disableSelection();

        return;
    }

    if (k->editMode == TupToolPlugin::Path) {
        if (k->path) {
            k->scene->removeItem(k->path);
            k->pathAdded = false;
            delete k->group;
            k->group = 0;
        }
        return;
    }
}

/* SQA: What is it? */

bool Tweener::isComplete() const
{
    return true;
}

/* This method defines the actions contained in this plugin */

void Tweener::setupActions()
{
    TAction *translater = new TAction(QPixmap(kAppProp->themeDir() + "icons/position_tween.png"), 
                                      tr("Position Tween"), this);
    translater->setCursor(QCursor(kAppProp->themeDir() + "cursors/tweener.png"));
    translater->setShortcut(QKeySequence(tr("Shift+W")));

    k->actions.insert(tr("Position Tween"), translater);
}

/* This method initializes the "Create path" mode */

void Tweener::setCreatePath()
{
    if (k->path) {

        k->pathOffset = QPointF(0, 0);

        if (!k->pathAdded) {
            k->scene->addItem(k->path);
            k->pathAdded = true;
        } 

        if (k->group) {
            k->group->createNodes(k->path);
        } else {
            k->group = new TNodeGroup(k->path, k->scene, TNodeGroup::PositionTween);
            connect(k->group, SIGNAL(nodeReleased()), SLOT(updatePath()));
            k->group->createNodes(k->path);
        }
        k->group->expandAllNodes();
    }

    k->editMode = TupToolPlugin::Path;
    disableSelection();
}

/* This method initializes the "Select object" mode */

void Tweener::setSelect()
{
    if (k->mode == TupToolPlugin::Edit) {
        if (k->startPoint != k->scene->currentFrameIndex()) {
            TupProjectRequest request = TupRequestBuilder::createFrameRequest(k->scene->currentSceneIndex(),
                                                                            k->scene->currentLayerIndex(),
                                                                            k->startPoint, TupProjectRequest::Select, "1");
            emit requested(&request);
        }
    }

    if (k->path) {
        k->scene->removeItem(k->path);
        k->pathAdded = false;
        delete k->group;
        k->group = 0;
    }

    k->editMode = TupToolPlugin::Selection;

    foreach (QGraphicsView * view, k->scene->views()) {
             view->setDragMode(QGraphicsView::RubberBandDrag);
             foreach (QGraphicsItem *item, view->scene()->items()) {
                      if ((item->zValue() >= 10000) && (item->toolTip().length()==0)) {
                          item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
                      }
             }
    }

    // When Object selection is enabled, previous selection is set
    if (k->objects.size() > 0) {
        foreach (QGraphicsItem *item, k->objects) {
                 item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
                 item->setSelected(true);
        }
    }

}

/* This method transforms the path created into a QString representation */

QString Tweener::pathToCoords()
{
    QString strPath = "";
    QChar t;
    int offsetX = k->pathOffset.x();
    int offsetY = k->pathOffset.y();

    for (int i=0; i < k->path->path().elementCount(); i++) {
         QPainterPath::Element e = k->path->path().elementAt(i);
         switch (e.type) {
            case QPainterPath::MoveToElement:
            {
                if (t != 'M') {
                    t = 'M';
                    strPath += "M " + QString::number(e.x + offsetX) + " " + QString::number(e.y + offsetY) + " ";
                } else {
                    strPath += QString::number(e.x + offsetX) + " " + QString::number(e.y + offsetY) + " ";
                }
            }
            break;
            case QPainterPath::LineToElement:
            {
                if (t != 'L') {
                    t = 'L';
                    strPath += " L " + QString::number(e.x + offsetX) + " " + QString::number(e.y + offsetY) + " ";
                } else {
                    strPath += QString::number(e.x + offsetX) + " " + QString::number(e.y + offsetY) + " ";
                }
            }
            break;
            case QPainterPath::CurveToElement:
            {
                if (t != 'C') {
                    t = 'C';
                    strPath += " C " + QString::number(e.x + offsetX) + " " + QString::number(e.y + offsetY) + " ";
                } else {
                    strPath += "  " + QString::number(e.x + offsetX) + " " + QString::number(e.y + offsetY) + " ";
                }
            }
            break;
            case QPainterPath::CurveToDataElement:
            {
                if (t == 'C')
                    strPath +=  " " + QString::number(e.x + offsetX) + "  " + QString::number(e.y + offsetY) + " ";
            }
            break;
        }
    }

    return strPath.trimmed();
}

/* This method resets this plugin */

void Tweener::applyReset()
{
    k->mode = TupToolPlugin::View;
    k->editMode = TupToolPlugin::None;

    clearSelection();
    disableSelection();

    if (k->group) {
        k->group->clear();
        k->group = 0;
    }

    if (k->path) {
        if (k->startPoint == k->scene->currentFrameIndex())
            k->scene->removeItem(k->path);
        k->pathAdded = false;
        k->path = 0;
    }

    k->startPoint = k->scene->currentFrameIndex();
    k->configurator->cleanData();
}

/* This method applies to the project, the Tween created from this plugin */

void Tweener::applyTween()
{
    #ifdef K_DEBUG
           T_FUNCINFO;
    #endif

    QString name = k->configurator->currentTweenName();

    if (name.length() == 0) {
        TOsd::self()->display(tr("Error"), tr("Tween name is missing!"), TOsd::Error);
        return;
    }

    if (!k->scene->scene()->tweenExists(name, TupItemTweener::Position)) {

        foreach (QGraphicsItem *item, k->objects) {   

                 TupLibraryObject::Type type = TupLibraryObject::Item;
                 int objectIndex = k->scene->currentFrame()->indexOf(item); 
                 QRectF rect = item->sceneBoundingRect();
                 QPointF point = rect.topLeft();

                 if (TupSvgItem *svg = qgraphicsitem_cast<TupSvgItem *>(item)) {
                     type = TupLibraryObject::Svg;
                     objectIndex = k->scene->currentFrame()->indexOf(svg);
                 } else {
                     if (qgraphicsitem_cast<TupPathItem *>(item) || qgraphicsitem_cast<TupEllipseItem *>(item) 
                         || qgraphicsitem_cast<TupLineItem *>(item) || qgraphicsitem_cast<TupRectItem *>(item))
                         point = item->pos();
                 }

                 QString route = pathToCoords();

                 TupProjectRequest request = TupRequestBuilder::createItemRequest(
                                            k->scene->currentSceneIndex(),
                                            k->scene->currentLayerIndex(),
                                            k->startPoint,
                                            objectIndex,
                                            QPointF(), k->scene->spaceMode(), type,
                                            TupProjectRequest::SetTween, 
                                            k->configurator->tweenToXml(k->startPoint, point, route));
                 emit requested(&request);
        }

        int framesNumber = framesTotal();
        int total = k->startPoint + k->configurator->totalSteps() - 1;

        if (total > framesNumber) {
            for (int i = framesNumber; i <= total; i++) {
                 TupProjectRequest requestFrame = TupRequestBuilder::createFrameRequest(k->scene->currentSceneIndex(),
                                                                                      k->scene->currentLayerIndex(),
                                                                                      i, TupProjectRequest::Add, tr("Frame %1").arg(i + 1));
                 emit requested(&requestFrame);
            }
        }

        TupProjectRequest request = TupRequestBuilder::createFrameRequest(k->scene->currentSceneIndex(), 
                                                                        k->scene->currentLayerIndex(),
                                                                        k->startPoint, TupProjectRequest::Select, "1");
        emit requested(&request);

    } else {

        removeTweenFromProject(name);
        QList<QGraphicsItem *> newList;

        foreach (QGraphicsItem *item, k->objects) {

                 TupLibraryObject::Type type = TupLibraryObject::Item;
                 TupScene *scene = k->scene->scene();
                 TupLayer *layer = scene->layer(k->scene->currentLayerIndex());
                 TupFrame *frame = layer->frame(k->currentTween->startFrame());
                 int objectIndex = frame->indexOf(item);

                 QRectF rect = item->sceneBoundingRect();
                 QPointF point = rect.topLeft();

                 if (TupSvgItem *svg = qgraphicsitem_cast<TupSvgItem *>(item)) {
                     type = TupLibraryObject::Svg;
                     objectIndex = frame->indexOf(svg);
                 } else {
                     if (qgraphicsitem_cast<TupPathItem *>(item) || qgraphicsitem_cast<TupEllipseItem *>(item) 
                         || qgraphicsitem_cast<TupLineItem *>(item) || qgraphicsitem_cast<TupRectItem *>(item))
                         point = item->pos();
                 }

                 if (k->startPoint != k->currentTween->startFrame()) {
                     QDomDocument dom;
                     dom.appendChild(dynamic_cast<TupAbstractSerializable *>(item)->toXml(dom));

                     TupProjectRequest request = TupRequestBuilder::createItemRequest(k->scene->currentSceneIndex(), 
                                                                                    k->scene->currentLayerIndex(), 
                                                                                    k->startPoint, -1, 
                                                                                    QPointF(), k->scene->spaceMode(), type, 
                                                                                    TupProjectRequest::Add, dom.toString());
                     emit requested(&request);

                     request = TupRequestBuilder::createItemRequest(k->scene->currentSceneIndex(), 
                                                                   k->scene->currentLayerIndex(),
                                                                   k->currentTween->startFrame(),
                                                                   objectIndex, QPointF(), 
                                                                   k->scene->spaceMode(), type,
                                                                   TupProjectRequest::Remove);
                     emit requested(&request);

                     frame = layer->frame(k->startPoint);
                     if (type == TupLibraryObject::Item)
                         objectIndex = frame->graphicItemsCount() - 1;
                     else
                         objectIndex = frame->svgItemsCount() - 1;

                     newList.append(frame->graphic(objectIndex)->item());
                 }

                 QString route = pathToCoords();

                 TupProjectRequest request = TupRequestBuilder::createItemRequest(
                                            k->scene->currentSceneIndex(),
                                            k->scene->currentLayerIndex(),
                                            k->startPoint,
                                            objectIndex,
                                            QPointF(), k->scene->spaceMode(), type,
                                            TupProjectRequest::SetTween,
                                            k->configurator->tweenToXml(k->startPoint, point, route));
                 emit requested(&request);

                 int total = k->startPoint + k->configurator->totalSteps();

                 int framesNumber = framesTotal();

                 if (framesNumber < total) {
                     for (int i = framesNumber; i < total; i++) {
                          TupProjectRequest requestFrame = TupRequestBuilder::createFrameRequest(k->scene->currentSceneIndex(),
                                                          k->scene->currentLayerIndex(),
                                                          i, TupProjectRequest::Add, tr("Frame %1").arg(i + 1));
                          emit requested(&requestFrame);
                     }
                 }

                 request = TupRequestBuilder::createFrameRequest(k->scene->currentSceneIndex(), k->scene->currentLayerIndex(),
                                                                k->startPoint, TupProjectRequest::Select, "1");
                 emit requested(&request);
        }

        if (newList.size() > 0)
            k->objects = newList;
    }

    setCurrentTween(name);

    TOsd::self()->display(tr("Info"), tr("Tween %1 applied!").arg(name), TOsd::Info);
}

/* This method updates the data of the path into the tool panel 
 * and disables edition mode for nodes
*/

void Tweener::updatePath()
{
    #ifdef K_DEBUG
           T_FUNCINFO;
    #endif

    k->configurator->updateSteps(k->path);
}

/* This method saves the settings of this plugin */

void Tweener::saveConfig()
{
}

/* This method updates the workspace when the plugin changes the scene */

void Tweener::updateScene(TupGraphicsScene *scene)
{
    #ifdef K_DEBUG
           T_FUNCINFO;
    #endif

    k->mode = k->configurator->mode();

    if (k->mode == TupToolPlugin::Edit) {

       int total = k->startPoint + k->configurator->totalSteps();

       if (k->editMode == TupToolPlugin::Path) {
           if (scene->currentFrameIndex() >= k->startPoint && scene->currentFrameIndex() < total) {
               if (k->path && k->group) {
                   k->scene->addItem(k->path);            
                   k->group->createNodes(k->path);
                   k->group->expandAllNodes();
               }
           }
       } 

       int framesNumber = framesTotal();

       if (k->configurator->startComboSize() < framesNumber)
           k->configurator->initStartCombo(framesNumber, k->startPoint);

    } else if (k->mode == TupToolPlugin::Add) {

               int total = framesTotal();

               if (k->configurator->startComboSize() < total) {
                   k->configurator->initStartCombo(total, k->startPoint);
               } else {
                   if (scene->currentFrameIndex() != k->startPoint)
                       k->configurator->setStartFrame(scene->currentFrameIndex());
               }

               if (k->editMode == TupToolPlugin::Path) {

                       k->path = 0;
                       k->configurator->cleanData();
                       k->configurator->activateMode(TupToolPlugin::Selection);
                       clearSelection();
                       setSelect();

               } else if (k->editMode == TupToolPlugin::Selection) {
                       
                       k->path = 0;
                       if (scene->currentFrameIndex() != k->startPoint)
                           clearSelection();
                       k->startPoint = scene->currentFrameIndex();
                       setSelect();
               } 

    } else {
             if (scene->currentFrameIndex() != k->startPoint) {
                 k->configurator->setStartFrame(scene->currentFrameIndex());
             }
    }
}

void Tweener::updateMode(TupToolPlugin::Mode mode)
{
    k->mode = mode;

    if (k->mode == TupToolPlugin::Edit)
        setEditEnv();
}

int Tweener::maxZValue()
{
    int max = -1;
    foreach (QGraphicsView *view, k->scene->views()) {
             foreach (QGraphicsItem *item, view->scene()->items()) {
                      if (item->zValue() > max)
                          max = item->zValue();

             }
    }

    return max + 1;
}

void Tweener::removeTweenFromProject(const QString &name)
{
    TupScene *scene = k->scene->scene();
    scene->removeTween(name, TupItemTweener::Position);

    foreach (QGraphicsView * view, k->scene->views()) {
             foreach (QGraphicsItem *item, view->scene()->items()) {
                      QString tip = item->toolTip();
                      if (tip.startsWith(tr("Position Tween") + ": " + name))
                          item->setToolTip("");
             }
    }
}

void Tweener::removeTween(const QString &name)
{
    removeTweenFromProject(name);
    applyReset();
}

void Tweener::setCurrentTween(const QString &name)
{
    TupScene *scene = k->scene->scene();
    k->currentTween = scene->tween(name, TupItemTweener::Position);
    if (k->currentTween) {
        k->configurator->setCurrentTween(k->currentTween);
    } 
}

void Tweener::setEditEnv()
{
    k->startPoint = k->currentTween->startFrame();
    if (k->startPoint != k->scene->currentFrameIndex()) {
        TupProjectRequest request = TupRequestBuilder::createFrameRequest(k->scene->currentSceneIndex(),
                                                                       k->scene->currentLayerIndex(),
                                                                       k->startPoint, TupProjectRequest::Select, "1");
        emit requested(&request);
    }

    k->mode = TupToolPlugin::Edit;

    TupScene *scene = k->scene->scene();
    k->objects = scene->getItemsFromTween(k->currentTween->name(), TupItemTweener::Position);
    QGraphicsItem *item = k->objects.at(0);
    QRectF rect = item->sceneBoundingRect();
    k->itemObjectReference = rect.center();

    k->path = k->currentTween->graphicsPath();
    k->path->setZValue(maxZValue());

    QPainterPath::Element e  = k->path->path().elementAt(0);
    k->firstNode = QPointF(e.x, e.y);

    QPointF oldPos = QPointF(e.x, e.y);
    QPointF newPos = rect.center();

    int distanceX = newPos.x() - oldPos.x();
    int distanceY = newPos.y() - oldPos.y();
    k->path->moveBy(distanceX, distanceY);
    k->pathOffset = QPointF(distanceX, distanceY);

    QColor color = Qt::lightGray;
    color.setAlpha(200);
    QPen pen(QBrush(color), 1, Qt::DotLine);
    k->path->setPen(pen);

    setCreatePath();
}

int Tweener::framesTotal()
{
    int total = 1;
    TupLayer *layer = k->scene->scene()->layer(k->scene->currentLayerIndex());
    if (layer)
        total = layer->framesTotal();

    return total;
}

/* This method clear selection */

void Tweener::clearSelection()
{
    if (k->objects.size() > 0) {
        foreach (QGraphicsItem *item, k->objects) {
                 if (item->isSelected())
                     item->setSelected(false);
        }
        k->objects.clear();
        k->configurator->notifySelection(false);
    }
}

/* This method disables object selection */

void Tweener::disableSelection()
{
    foreach (QGraphicsView *view, k->scene->views()) {
             view->setDragMode (QGraphicsView::NoDrag);
             foreach (QGraphicsItem *item, view->scene()->items()) {
                      item->setFlag(QGraphicsItem::ItemIsSelectable, false);
                      item->setFlag(QGraphicsItem::ItemIsMovable, false);
             }
    }
}

void Tweener::sceneResponse(const TupSceneResponse *event)
{
    #ifdef K_DEBUG
           T_FUNCINFO;
    #endif

    if ((event->action() == TupProjectRequest::Remove || event->action() == TupProjectRequest::Reset)
        && (k->scene->currentSceneIndex() == event->sceneIndex())) {
        tError() << "Tracing 1";
        init(k->scene);
    }

    if (event->action() == TupProjectRequest::Select)
        init(k->scene);
}

void Tweener::layerResponse(const TupLayerResponse *event)
{
    #ifdef K_DEBUG
           T_FUNCINFO;
    #endif

    if (event->action() == TupProjectRequest::Remove)
        init(k->scene);        
}

void Tweener::frameResponse(const TupFrameResponse *event)
{
    #ifdef K_DEBUG
           T_FUNCINFO;
    #endif

    if (event->action() == TupProjectRequest::Remove && k->scene->currentLayerIndex() == event->layerIndex())
        init(k->scene);
}

TupToolPlugin::Mode Tweener::currentMode()
{
    return k->mode;
}

TupToolPlugin::EditMode Tweener::currentEditMode()
{
    return k->editMode;
}

Q_EXPORT_PLUGIN2(tup_tweener, Tweener);
