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

#ifndef AKI_NOTIFICATIONS_H
#define AKI_NOTIFICATIONS_H

#include <QObject>

class KMainWindow;
namespace Aki
{
class NotificationsPrivate;
class Notifications : public QObject
{
    Q_OBJECT
public:
    Notifications(QObject *parent = 0);
    ~Notifications();
    void setWindow(KMainWindow *window);
    void highlight(const QString &from, const QString &message);
    void privateMessage(const QString &from, const QString &message);
private:
    friend class NotificationsPrivate;
    NotificationsPrivate* const d;
}; // End of class Notifications.
} // End of namespace Aki.

#endif // AKI_NOTIFICATIONS_H
