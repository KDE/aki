/*
    This file is part of Aki IRC.
    Copyright 2009 - 2010 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "eventitem.h"
#include <aki/irc/nickinfo.h>

class EventItemPrivate
{
public:
    EventItemPrivate()
        : type(EventItem::UnknownEvent),
        server(QString()),
        channel(QString()),
        message(QString()),
        from(QString())
    {
        time = KDateTime::currentLocalDateTime();
        time = time.toClockTime();
    }

    EventItem::EventType type;
    QString server;
    QString channel;
    QString message;
    Aki::Irc::NickInfo from;
    KDateTime time;
}; // End of class EventItemPrivate.

EventItem::EventItem()
{
    d.reset(new EventItemPrivate);
}

EventItem::EventItem(const EventItem &copy)
{
    d.reset(new EventItemPrivate);
    d->channel = copy.d->channel;
    d->from = copy.d->from;
    d->server = copy.d->server;
    d->time = copy.d->time;
    d->type = copy.d->type;
}

EventItem::~EventItem()
{
}

EventItem&
EventItem::operator=(const EventItem& rhs)
{
    d.reset(new EventItemPrivate);
    d->channel = rhs.d->channel;
    d->from = rhs.d->from;
    d->server = rhs.d->server;
    d->time = rhs.d->time;
    d->type = rhs.d->type;
    return *this;
}

QString
EventItem::channel() const
{
    return d->channel;
}

EventItem::EventType
EventItem::event() const
{
    return d->type;
}

Aki::Irc::NickInfo
EventItem::from() const
{
    return d->from;
}

QString EventItem::server() const
{
    return d->server;
}

void
EventItem::setChannel(const QString& channel)
{
    d->channel = channel;
}

void
EventItem::setEvent(EventItem::EventType type)
{
    d->type = type;
}

void
EventItem::setFrom(const Aki::Irc::NickInfo& from)
{
    d->from = from;
}

void
EventItem::setServer(const QString& server)
{
    d->server = server;
}

KDateTime
EventItem::time() const
{
    return d->time;
}

QString
EventItem::message() const
{
    return d->message;
}

void
EventItem::setMessage(const QString& message)
{
    d->message = message;
}
