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

#include "invitesentreply.hpp"
#include "private/invitesentreply_p.hpp"
using namespace Aki;
using namespace Irc;

InviteSentReply::InviteSentReply()
    : Aki::Irc::Reply(),
    _d(new Aki::Irc::InviteSentReplyPrivate)
{
}

InviteSentReply::InviteSentReply(const Aki::Irc::ReplyInfo& replyInfo)
    : Aki::Irc::Reply(replyInfo),
    _d(new Aki::Irc::InviteSentReplyPrivate)
{
    _d->nick = replyInfo.params().at(1);
    _d->channel = replyInfo.params().at(2);
}

InviteSentReply::InviteSentReply(const Aki::Irc::InviteSentReply& other)
    : Aki::Irc::Reply(other),
    _d(other._d)
{
}

Aki::Irc::InviteSentReply&
InviteSentReply::operator=(const Aki::Irc::InviteSentReply& other)
{
    Aki::Irc::Reply::operator=(other);
    _d = other._d;
    return *this;
}

InviteSentReply::~InviteSentReply()
{
}

QString
InviteSentReply::channel() const
{
    return _d->channel;
}

QString
InviteSentReply::nick() const
{
    return _d->nick;
}
