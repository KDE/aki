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

#include "actionreply.hpp"
#include "private/actionreply_p.hpp"
using namespace Aki;
using namespace Irc;

ActionReply::ActionReply()
    : Aki::Irc::CtcpReply(),
    _d(new Aki::Irc::ActionReplyPrivate)
{
}

ActionReply::ActionReply(const Aki::Irc::CtcpReply& reply)
    : Aki::Irc::CtcpReply(reply),
    _d(new Aki::Irc::ActionReplyPrivate)
{
    _d->message = reply.param();
}

ActionReply::ActionReply(const Aki::Irc::ActionReply& other)
    : Aki::Irc::CtcpReply(other),
    _d(other._d)
{
}

ActionReply::~ActionReply()
{
}

Aki::Irc::ActionReply&
ActionReply::operator=(const Aki::Irc::ActionReply& other)
{
    Aki::Irc::CtcpReply::operator=(other);
    _d = other._d;
    return *this;
}

QString
ActionReply::message() const
{
    return _d->message;
}
