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

#include "indicationsystem.hpp"
#include "config.hpp"
#include "debughelper.hpp"
#include "private/indicationsystem_p.hpp"
#include "ui/mainwindow.hpp"
using namespace Aki;
using namespace Irc;

template<> Aki::IndicationSystem* Aki::Singleton<Aki::IndicationSystem>::_instance = 0;

IndicationSystem::IndicationSystem()
    : Singleton<Aki::IndicationSystem>()
{
    DEBUG_FUNC_NAME;
    _d.reset(new Aki::IndicationSystemPrivate(this));
#if defined(HAVE_LIBINDICATE)
    _d->_indicateNotification.reset(new Aki::IndicateNotification(this));
#else
    _d->_indicateNotification.reset(new Aki::KNotifications(this));
#endif // HAVE_LIBINDICATE
}

IndicationSystem::~IndicationSystem()
{
}

void
IndicationSystem::addChannelMessage(const QString& channelName, const QString& message)
{
    _d->_indicateNotification->addChannelMessage(channelName, message);
}

void
IndicationSystem::addPrivateMessage(const Aki::Irc::NickInfo& from, const QString& message)
{
    _d->_indicateNotification->addPrivateMessage(from, message);
}

void
IndicationSystem::setMainWindow(Aki::MainWindow* window)
{
    _d->_indicateNotification->setMainWindow(window);
}
