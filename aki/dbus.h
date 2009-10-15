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

#ifndef DBUS_H
#define DBUS_H

#include <QObject>
#include <QStringList>

namespace Aki
{
class ServerView;
} // End of namespace Aki.

class DBus : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.aki.main")
public:
    DBus(QObject *parent = 0);
    ~DBus();
    void setServerView(Aki::ServerView *view);
public Q_SLOTS:
    QString currentNickFromServer(const QString &server) const;
    QStringList serverList() const;
    void setAllAway(const QString &awayMessage);
private:
    Aki::ServerView *m_serverView;
}; // End of class DBus.

#endif // DBUS_H
