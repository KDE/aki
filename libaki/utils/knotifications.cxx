/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "knotifications.hpp"
#include "irc/nickinfo.hpp"
#include "private/knotifications_p.hpp"
#include <KDE/KNotification>
using namespace Aki;

KNotifications::KNotifications(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::KNotificationsPrivate(this));
}

KNotifications::~KNotifications()
{
}

void
KNotifications::addChannelMessage(const QString& channelName, const QString& message)
{
    Q_ASSERT(_d->mainWindow);
    KNotification::event("highlight", QString("&lt;%1&gt; %2").arg(channelName, message), QPixmap(), _d->mainWindow);
}

void
KNotifications::addCustomMessage(const QString& message, const KIcon& icon, const QString& eventName)
{
    Q_ASSERT(_d->mainWindow);
    KNotification::event(eventName, message, icon.pixmap(QSize(16, 16)), _d->mainWindow);
}

void
KNotifications::addPrivateMessage(const Aki::Irc::NickInfo& from, const QString& message)
{
    Q_ASSERT(_d->mainWindow);
    KNotification::event("private", QString("&lt;%1&gt; %2").arg(from.nick(), message), QPixmap(), _d->mainWindow);
}

void
KNotifications::setMainWindow(MainWindow* window)
{
    Q_ASSERT(window);
    _d->mainWindow = window;
}
