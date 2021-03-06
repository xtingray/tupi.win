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

#ifndef TUPNEWPROYECT_H
#define TUPNEWPROYECT_H

#include "tosd.h"
#include "tabdialog.h"
#include "twizard.h"
#include "txyspinbox.h"

class TupProjectManagerParams;

/**
 * @author Jorge Cuadrado
*/

class TupNewProject : public TabDialog
{
    Q_OBJECT

    public:

        enum Format { FREE = 0, 
                      FORMAT_480P,
                      FORMAT_576P,
                      FORMAT_720P,
                      FORMAT_1280P
                    };

        TupNewProject(QWidget *parent = 0);
        ~TupNewProject();
        TupProjectManagerParams *parameters();
        bool useNetwork() const;
        void focusProjectLabel();
        QString login() const;

    public slots:
        void ok();

    private:
        void setupNetOptions();

    private slots:
        void setBgColor();
        void setPresets(int index);

    public slots:
        void enableNetOptions(bool isEnabled);

    private:
        struct Private;
        Private *const k;
};

#endif
