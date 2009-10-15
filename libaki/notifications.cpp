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

#include "notifications.h"
#include <KDebug>
#include <KMainWindow>
#include <KNotification>
using namespace Aki;

namespace Aki
{
class NotificationsPrivate
{
public:
    NotificationsPrivate()
        : window(0)
    {
    }

    KMainWindow *window;
}; // End of class NotificationsPrivate.
} // End of namespace Aki.

Notifications::Notifications(QObject *parent)
    : QObject(parent),
    d(new NotificationsPrivate)
{
}

Notifications::~Notifications()
{
    delete d;
}

void
Notifications::setWindow(KMainWindow *window)
{
    d->window = window;
}

void
Notifications::highlight(const QString &from, const QString &message)
{
    kDebug() << d->window;
    if (!d->window) {
        return;
    }

    KNotification::event("highlight", QString("&lt;%1&gt; %2").arg(from, message),
                         QPixmap(), d->window);
}

void
Notifications::privateMessage(const QString &from, const QString &message)
{
    if (!d->window) {
        return;
    }

    KNotification::event("private", QString("-%1- %2").arg(from, message),
                         QPixmap(), d->window);
}
