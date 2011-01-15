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

#include "globalusersreply.hpp"
#include "private/globalusersreply_p.hpp"
using namespace Aki;
using namespace Irc;

GlobalUsersReply::GlobalUsersReply()
    : Aki::Irc::Reply(),
    _d(new Aki::Irc::GlobalUsersReplyPrivate)
{
}

GlobalUsersReply::GlobalUsersReply(const Aki::Irc::ReplyInfo& replyInfo)
    : Aki::Irc::Reply(replyInfo),
    _d(new Aki::Irc::GlobalUsersReplyPrivate)
{
    if (replyInfo.params().count() > 2) {
        _d->global = replyInfo.params().at(1).toInt();
        _d->max = replyInfo.params().at(2).toInt();
        _d->message = replyInfo.params().at(3);
    } else {
        // This is a fix for afternet
        // Need to make it so that we get the global/max count.
        _d->message = replyInfo.params().at(1);
    }
}

GlobalUsersReply::GlobalUsersReply(const Aki::Irc::GlobalUsersReply& other)
    : Aki::Irc::Reply(other),
    _d(other._d)
{
}

GlobalUsersReply::~GlobalUsersReply()
{
}

Aki::Irc::GlobalUsersReply&
GlobalUsersReply::operator=(const Aki::Irc::GlobalUsersReply& other)
{
    Aki::Irc::Reply::operator=(other);
    _d = other._d;
    return *this;
}

int
GlobalUsersReply::global() const
{
    return _d->global;
}

int
GlobalUsersReply::max() const
{
    return _d->max;
}

QString
GlobalUsersReply::message() const
{
    return _d->message;
}
