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

#include "channelmodereply.hpp"
#include "private/channelmodereply_p.hpp"
using namespace Aki;
using namespace Irc;

ChannelModeReply::ChannelModeReply()
    : Aki::Irc::Reply(),
    _d(new Aki::Irc::ChannelModeReplyPrivate)
{
}

ChannelModeReply::ChannelModeReply(const Aki::Irc::ReplyInfo& replyInfo)
    : Aki::Irc::Reply(replyInfo),
    _d(new Aki::Irc::ChannelModeReplyPrivate)
{
    if (replyInfo.numeric() == Aki::Irc::RPL_CHANNELMODEIS) {
        _d->channel = replyInfo.params().at(1);
        _d->modes = replyInfo.params().at(2);
    } else {
        _d->channel = replyInfo.params().at(0);
        _d->modes = replyInfo.params().at(1);
        _d->options = replyInfo.params().value(2);
    }
}

ChannelModeReply::ChannelModeReply(const Aki::Irc::ChannelModeReply& other)
    : Aki::Irc::Reply(other),
    _d(other._d)
{
}

ChannelModeReply::~ChannelModeReply()
{
}

Aki::Irc::ChannelModeReply&
ChannelModeReply::operator=(const Aki::Irc::ChannelModeReply& other)
{
    Aki::Irc::Reply::operator=(other);
    _d = other._d;
    return *this;
}

QString
ChannelModeReply::channel() const
{
    return _d->channel;
}

QString
ChannelModeReply::modes() const
{
    return _d->modes;
}

QString
ChannelModeReply::options() const
{
    return _d->options;
}
