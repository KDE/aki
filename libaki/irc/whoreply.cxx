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

#include "whoreply.hpp"
#include "private/whoreply_p.hpp"
using namespace Aki;
using namespace Irc;

WhoReply::WhoReply()
    : Aki::Irc::Reply(),
    _d(new Aki::Irc::WhoReplyPrivate)
{
}

WhoReply::WhoReply(const Aki::Irc::ReplyInfo& replyInfo)
    : Aki::Irc::Reply(replyInfo),
    _d(new Aki::Irc::WhoReplyPrivate)
{
    _d->lastMessage = (replyInfo.numeric() == Aki::Irc::RPL_ENDOFWHO);
    if (!_d->lastMessage) {
        _d->channel = replyInfo.params().at(1);
        _d->userName = replyInfo.params().at(2);
        _d->address = replyInfo.params().at(3);
        _d->server = replyInfo.params().at(4);
        _d->nickName = replyInfo.params().at(5);

        const QString tmpFlags = replyInfo.params().at(6);
        foreach (QChar ch, tmpFlags) {
            _d->flags.append(ch);
        }

        QString tmp = replyInfo.params().at(7);
        _d->hops = tmp.left(tmp.indexOf(' ')).toInt();
        tmp.remove(0, tmp.length() + 1);

        _d->info = tmp;
    } else {
        _d->channel = replyInfo.params().at(1);
        _d->message = replyInfo.params().at(2);
    }
}

WhoReply::WhoReply(const Aki::Irc::WhoReply& other)
    : Aki::Irc::Reply(other),
    _d(other._d)
{
}

WhoReply::~WhoReply()
{
}

Aki::Irc::WhoReply&
WhoReply::operator=(const Aki::Irc::WhoReply& other)
{
    Aki::Irc::Reply::operator=(other);
    _d = other._d;
    return *this;
}

QString
WhoReply::address() const
{
    return _d->address;
}

QString
WhoReply::channel() const
{
    return _d->channel;
}

QStringList
WhoReply::flags() const
{
    return _d->flags;
}

int
WhoReply::hops() const
{
    return _d->hops;
}

QString
WhoReply::info() const
{
    return _d->info;
}

bool
WhoReply::isLastMessage() const
{
    return _d->lastMessage;
}

QString
WhoReply::message() const
{
    return _d->message;
}

QString
WhoReply::nickName() const
{
    return _d->nickName;
}

QString
WhoReply::server() const
{
    return _d->server;
}

QString
WhoReply::userName() const
{
    return _d->userName;
}
