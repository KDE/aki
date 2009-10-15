/*
    This file is part of Aki IRC client.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "dbus.h"
#include "ui/serverview.h"
#include "ui/serverwindow.h"
#include <Aki/Irc/Socket>
#include <QDBusConnection>

DBus::DBus(QObject *parent)
    : QObject(parent)
{
    QDBusConnection::sessionBus().registerObject("/main", this, QDBusConnection::ExportAllContents);
}

DBus::~DBus()
{
}

void
DBus::setServerView(Aki::ServerView *view)
{
    m_serverView = view;
}

QString
DBus::currentNickFromServer(const QString &server) const
{
    Aki::BaseWindow *window = m_serverView->findWindow(server);
    if (window && window->windowType() == Aki::BaseWindow::ServerWindow) {
        return window->socket()->currentNick();
    }
    return QString();
}

QStringList
DBus::serverList() const
{
    QStringList list;

    foreach (Aki::BaseWindow *window, m_serverView->serverList()) {
        if (window && window->windowType() == Aki::BaseWindow::ServerWindow) {
            list << window->name();
        }
    }

    return list;
}

void
DBus::setAllAway(const QString &message)
{
    foreach (Aki::BaseWindow *window, m_serverView->serverList()) {
        if (window && window->windowType() == Aki::BaseWindow::ServerWindow) {
            qobject_cast<Aki::ServerWindow*>(window)->socket()->rfcAway(message);
        }
    }
}
