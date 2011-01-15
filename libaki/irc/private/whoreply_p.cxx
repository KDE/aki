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

#include "whoreply_p.hpp"

using namespace Aki::Irc;

WhoReplyPrivate::WhoReplyPrivate()
    : QSharedData(),
    channel(QString()),
    userName(QString()),
    address(QString()),
    server(QString()),
    nickName(QString()),
    info(QString()),
    message(QString()),
    flags(QStringList()),
    hops(-1),
    lastMessage(false)
{
}

WhoReplyPrivate::WhoReplyPrivate(const Aki::Irc::WhoReplyPrivate& other)
    : QSharedData(other),
    channel(other.channel),
    userName(other.userName),
    address(other.address),
    server(other.server),
    nickName(other.nickName),
    info(other.info),
    message(other.message),
    flags(other.flags),
    hops(other.hops),
    lastMessage(other.lastMessage)
{
}

