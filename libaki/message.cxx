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

#include "message.hpp"
#include "private/message_p.hpp"
using namespace Aki;

Message::Message(const QString& line)
    : Aki::Irc::Message(line)
{
    _d.reset(new Aki::MessagePrivate(this));
}

Message::Message(const Aki::Message& message)
    : Aki::Irc::Message(message)
{
    setDirection(message.direction());
    setTimeStamp(message.timeStamp());
}

Message::~Message()
{
}

Message&
Message::operator=(const Aki::Message& rhs)
{
    Aki::Irc::Message::operator=(rhs);
    setDirection(rhs.direction());
    setTimeStamp(rhs.timeStamp());
    return *this;
}

void
Message::createTimestamp()
{
    _d->timeStamp = KDateTime::currentLocalDateTime();
}

Aki::Message::Direction
Message::direction() const
{
    return _d->direction;
}

bool
Message::isRightToLeft() const
{
    return message().isRightToLeft();
}

void
Message::setDirection(Message::Direction direction)
{
    _d->direction = direction;
}

void
Message::setTimeStamp(const KDateTime& timeStamp)
{
    _d->timeStamp = timeStamp;
}

KDateTime
Message::timeStamp() const
{
    return _d->timeStamp;
}
