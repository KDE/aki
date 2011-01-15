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

#include "topicsetbyreply.hpp"
#include "private/topicsetbyreply_p.hpp"
using namespace Aki;
using namespace Irc;

TopicSetByReply::TopicSetByReply()
    : Aki::Irc::Reply(),
    _d(new Aki::Irc::TopicSetByReplyPrivate)
{
}

TopicSetByReply::TopicSetByReply(const Aki::Irc::ReplyInfo& replyInfo)
    : Aki::Irc::Reply(replyInfo),
    _d(new Aki::Irc::TopicSetByReplyPrivate)
{
    _d->channel = replyInfo.params().at(1);
    _d->user = Aki::Irc::NickInfo(replyInfo.params().at(2));
    KDateTime dateTime;
    dateTime.setTime_t(replyInfo.params().at(3).toULong());
    _d->dateTime = dateTime;
}

TopicSetByReply::TopicSetByReply(const Aki::Irc::TopicSetByReply& other)
    : Aki::Irc::Reply(other),
    _d(other._d)
{
}

TopicSetByReply::~TopicSetByReply()
{
}

Aki::Irc::TopicSetByReply&
TopicSetByReply::operator=(const Aki::Irc::TopicSetByReply& other)
{
    Aki::Irc::Reply::operator=(other);
    _d = other._d;
    return *this;
}

QString
TopicSetByReply::channel() const
{
    return _d->channel;
}

KDateTime
TopicSetByReply::dateTime() const
{
    return _d->dateTime;
}

Aki::Irc::NickInfo
TopicSetByReply::user() const
{
    return _d->user;
}
