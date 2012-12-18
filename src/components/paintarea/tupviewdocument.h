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

#ifndef TUPVIEWDOCUMENT_H
#define TUPVIEWDOCUMENT_H

#include "tupdocumentruler.h"
#include "tactionmanager.h"
#include "tosd.h"
#include "tupfilterinterface.h"
#include "tuptoolinterface.h"
#include "tupconfigurationarea.h"
#include "tupglobal.h"
#include "tupstoryboard.h"

#include <QMainWindow>

class TupProjectRequest;
class TupProject;
class TupBrushManager;
class TupPaintAreaCommand;
class TupPaintAreaEvent;
class TupProjectResponse;

/**
 * @author Jorge Cuadrado
*/

class TupViewDocument : public QMainWindow
{
    Q_OBJECT

    public:
        TupViewDocument(TupProject *project, QWidget *parent = 0, bool isNetworked = true, const QStringList &users = QStringList());
        ~TupViewDocument();
        void closeArea();
        QSize sizeHint() const;

        void setAntialiasing(bool useIt);
        void setOpenGL(bool useIt);
        void setDrawGrid(bool draw);

        QPainter::RenderHints renderHints() const;
        void setRotationAngle(int angle);
        void setZoom(qreal factor);

        TupBrushManager *brushManager() const;
        TupPaintAreaCommand *createCommand(const TupPaintAreaEvent *event);
        void updatePaintArea();
        TupProject::Mode spaceContext();
        TupProject *project();
        int currentFramesTotal();
        int currentSceneIndex();
        void setZoomView(const QString &percent);
        QSize workSpaceSize() const;
        void updateUsersOnLine(const QString &login, int state);

    private slots:
        void setNextOnionSkin(int n);
        void setPreviousOnionSkin(int n);

        void toggleShowGrid();

        // void setZoomFactor(int porcent);
        void updateScaleVars(double factor);
        void changeRulerOrigin(const QPointF &zero);
        void saveTimer();
        void showFullScreen();
        void closeFullScreen();
        void loadPlugin(int menu, int index);
        void updateStatusBgColor(const QColor color);
        void updatePenThickness(int size);
        void updateOnionOpacity(double opacity);

    private:
        struct Private;
        Private *const k;

    private:
        void setupDrawActions();
        void createToolBar();
        void createMenu();
        void createTools();

    private slots: 
        // Plugins
        void loadPlugins();
        void setSpaceContext();
        void updateBgColor(const QColor color);
        void enableOnionFeature();
        void setDefaultOnionFactor();
        void setOnionFactor(double value);
        void exportImage();
        void postImage();
        void storyboardSettings();
        void selectFrame(int frame, int layer, int scene);
        void selectScene(int scene);

    private slots:
        void showPos(const QPointF &p);	
        void setCursor(const QCursor &c);
        void updateZoomFactor( double f);
        void selectToolFromMenu(QAction *act);
        void callAutoSave();
        void updateStoryboard(TupStoryboard *storyboard);

    public slots:
        void undo();
        void redo();
        void selectTool();
        void applyFilter();

        bool handleProjectResponse(TupProjectResponse *event);

    signals:
        void sendToStatus(const QString &msg);
        void requestTriggered(const TupProjectRequest *event);
        void localRequestTriggered(const TupProjectRequest *event);
        void autoSave();
        void modeHasChanged(int mode);
        void expandColorPanel();
        void requestExportImageToServer(int frameIndex, int sceneIndex, const QString &title, const QString &topics, const QString &description);
        void openColorDialog(const QColor &);
        void updateColorFromFullScreen(const QColor &color);
        void updatePenFromFullScreen(const QPen &pen);

    // protected:
    // void closeEvent(QCloseEvent *e);
};

#endif
