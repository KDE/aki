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
#include <QScopedPointer>

class KMainWindow;
namespace Aki
{
class NotificationsPrivate;
/**
 * Notifications is a wrapper around KNotify to give Aki taskbar
 * popups on each system of important information.
 */
class Notifications : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates the notification object.
     * @param parent Parent of the object.
     */
    Notifications(QObject *parent = 0);
    /**
     * Destroys the object.
     */
    ~Notifications();
    /**
     * Sets the main window of the notification.
     * @param window Main window of the application.
     */
    void setWindow(KMainWindow *window);
    /**
     * A highlight was triggered.
     * @param from Nick of the person who triggered this.
     * @param message Message of the user who sent this.
     */
    void highlight(const QString &from, const QString &message);
    /**
     * A private message was triggered.
     * @param from Nick of the person who triggered this.
     * @param message Message of the user who sent this.
     */
    void privateMessage(const QString &from, const QString &message);
private:
    friend class NotificationsPrivate;
    QScopedPointer<NotificationsPrivate> d;
}; // End of class Notifications.
} // End of namespace Aki.

#endif // AKI_NOTIFICATIONS_H
