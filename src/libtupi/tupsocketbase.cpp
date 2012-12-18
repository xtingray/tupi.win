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

#include "tupsocketbase.h"
#include "tdebug.h"

#include <QTextStream>
#include <QQueue>

struct TupSocketBase::Private
{
    QQueue<QString> queue;
};

TupSocketBase::TupSocketBase(QObject *parent) : QTcpSocket(parent), k(new Private)
{
    connect(this, SIGNAL(readyRead ()), this, SLOT(readFromServer()));
    connect(this, SIGNAL(connected()), this, SLOT(sendQueue()));
    connect(this, SIGNAL(disconnected()), this, SLOT(clearQueue()));
}

TupSocketBase::~TupSocketBase()
{
    delete k;
}

void TupSocketBase::sendQueue()
{
    while (k->queue.count() > 0) {
        if (state() == QAbstractSocket::ConnectedState)
            send(k->queue.dequeue());
        else 
            break;
    }
}

void TupSocketBase::clearQueue()
{
    k->queue.clear();
}

void TupSocketBase::send(const QString &str)
{
    if (state() == QAbstractSocket::ConnectedState) {
        QTextStream stream(this);
        stream << str.toLocal8Bit().toBase64() << "%%" << endl;
    } else {
        k->queue.enqueue(str);
    }
}

void TupSocketBase::send(const QDomDocument &doc)
{
    send(doc.toString(0));
}

void TupSocketBase::readFromServer()
{
    QString readed;

    while (this->canReadLine()) {
           readed += this->readLine();
           if (readed.endsWith("%%\n"))
               break;
    }
    
    if (!readed.isEmpty()) {
        readed.remove(readed.lastIndexOf("%%"), 2);
        readed = QString::fromLocal8Bit(QByteArray::fromBase64(readed.toLocal8Bit()));
        
        this->readed(readed);
    }
    
    if (this->canReadLine()) 
        readFromServer();
}

