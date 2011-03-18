/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
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

#include "konversationconfigurationprivate.h"
using namespace Aki;

KonversationConfigurationPrivate::KonversationConfigurationPrivate(Aki::KonversationConfiguration* qq)
    : database(0),
    _q(qq)
{
}

int
KonversationConfigurationPrivate::channelCount() const
{
    return _q->groupLists().filter(QRegExp("Channel [\\d]+$")).count();
}

int
KonversationConfigurationPrivate::identityCount() const
{
    return _q->groupLists().filter(QRegExp("^Identity [\\d]+$")).count();
}

QStringList
KonversationConfigurationPrivate::isAutoJoinChannels() const
{
    return _q->currentGroup().readEntry("AutoJoinChannels").split(",", QString::SkipEmptyParts);
}

bool
KonversationConfigurationPrivate::isAutoReconnect() const
{
    return _q->currentGroup().readEntry("AutoConnect", false);
}

bool
KonversationConfigurationPrivate::isSslEnabled() const
{
    return _q->currentGroup().readEntry("SSLEnabled", false);
}

QString
KonversationConfigurationPrivate::name() const
{
    return _q->currentGroup().readEntry("Name", QString());
}

QString
KonversationConfigurationPrivate::password() const
{
    return _q->currentGroup().readEntry("Password", QString());
}

int
KonversationConfigurationPrivate::port() const
{
    return _q->currentGroup().readEntry("Port", 6667);
}

QString
KonversationConfigurationPrivate::server() const
{
    return _q->currentGroup().readEntry("Server", QString());
}

int
KonversationConfigurationPrivate::serverCount() const
{
    return _q->groupLists().filter(QRegExp("^Server [\\d]+$")).count();
}

int
KonversationConfigurationPrivate::serverGroupCount() const
{
    return _q->groupLists().filter(QRegExp("^ServerGroup [\\d]+$")).count();
}

QStringList
KonversationConfigurationPrivate::serverList() const
{
    return _q->currentGroup().readEntry("ServerList", QString()).split(",", QString::SkipEmptyParts);
}
