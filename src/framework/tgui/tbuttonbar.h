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

#ifndef TBUTTONBAR_H
#define TBUTTONBAR_H

#include "tideality.h"

#include <QToolBar>
#include <QButtonGroup>
#include <QMap>
#include <QTimer>

class TViewButton;
class ToolView;
class QAction;
class QMenu;

/**
 * @author David Cuadrado
*/

class T_IDEAL_EXPORT TButtonBar : public QToolBar
{
    Q_OBJECT

    public:
        TButtonBar(Qt::ToolBarArea area, QWidget *parent = 0);
        ~TButtonBar();

        void addButton(TViewButton *viewButton);
        void removeButton(TViewButton *viewButton);
        bool isEmpty() const;
        void disable(TViewButton *v);
        void enable(TViewButton *v);

        bool isExclusive() const;
        bool autohide() const;
        void showSeparator(bool e);

        int count() const;
        void setEnableButtonBlending(bool enable);

    public slots:
        void onlyShow(ToolView *tool, bool ensureVisible = false);
        void setExclusive(bool excl);
        void setAutoHide(bool autohide);
        void setShowOnlyIcons();
        void setShowOnlyTexts();

    private:
        QMenu *createMenu();

    private slots:
        void hideOthers(QAbstractButton *source);
        void doNotHide();

    protected:
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void enterEvent(QEvent *event);
        virtual void leaveEvent(QEvent *event);

    private:
        QButtonGroup m_buttons;
        QMap<QWidget *, QAction *> m_actionForWidget;
        QAction *m_separator;
        QTimer m_hider;
        bool m_autoHide;
        bool m_blockHider;
};

#endif
