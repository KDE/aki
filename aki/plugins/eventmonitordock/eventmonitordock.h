/*
    This file is part of Aki IRC.
    Copyright 2009 - 2010 Keith Rusler <xzekex@live.co.uk>

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

#ifndef EVENTMONITORDOCK_H
#define EVENTMONITORDOCK_H

#include <QDockWidget>
#include <QScopedPointer>
#include <QTreeWidgetItem>

class EventItem;
class EventMonitorDockPrivate;
class EventMonitorDock : public QDockWidget
{
    Q_OBJECT
public:
    EventMonitorDock(QWidget *parent = 0);
    ~EventMonitorDock();
    void addItem(const EventItem &item);
    void removeItem(int index);
    void clear();
private:
    Q_PRIVATE_SLOT(d, void clearTriggered())
    Q_PRIVATE_SLOT(d, void customContextMenuRequested(const QPoint &pos))
private:
    enum {
        CustomType = QTreeWidgetItem::UserType,
        IrcEventRole = Qt::UserRole
    }; // End of anonymous enum.
    enum {
        MESSAGE_LENGTH = 1024 * 2
    };
    friend class EventMonitorDockPrivate;
    QScopedPointer<EventMonitorDockPrivate> d;
}; // End of class EventMonitorDock.

#endif // EVENTMONITORDOCK_H
