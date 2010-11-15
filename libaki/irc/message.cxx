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
#include <QtCore/QTextCodec>
#include <KGlobal>
using namespace Aki;
using namespace Irc;

Message::Message()
{
    _d.reset(new Aki::Irc::MessagePrivate);
}

Message::Message (const Aki::Irc::Message& other)
{
    _d.reset(new Aki::Irc::MessagePrivate);
}

Message::~Message()
{
}

void Message::clear()
{
    if (_d->message.isEmpty()) {
        return;
    }

    _d->message.clear();
}

Aki::Irc::Message::Direction
Message::direction() const
{
    return _d->direction;
}

QString
Message::html() const
{
    return _d->message;
}

bool
Message::isRightToLeft() const
{
    return _d->message.isRightToLeft();
}

QString Message::plainText() const
{
    return _d->message;
}

const Aki::Irc::NickInfo&
Message::sender() const
{
    return _d->sender;
}

void
Message::setDirection(Message::Direction direction)
{
    if (_d->direction != direction) {
        _d->direction = direction;
    }
}

void
Message::setHtml(const QString& message)
{
    if (_d->message != message) {
        _d->message = message;
    }
}

void
Message::setPlainText (const QString& message)
{
    if (_d->message != message) {
        _d->message = message;
    }
}

void
Message::setSender(const Aki::Irc::NickInfo& sender)
{
    if (_d->sender != sender) {
        _d->sender = sender;
    }
}

void
Message::setState(Message::State state)
{
    if (_d->state != state) {
        _d->state = state;
    }
}

void
Message::setTarget (const Aki::Irc::NickInfo& target)
{
    if (_d->target != target) {
        _d->target = target;
    }
}

void Message::setTimeStamp (const KDateTime& timeStamp)
{
    if (timeStamp.isNull() || !timeStamp.isValid()) {
        return;
    }

    if (_d->timeStamp != timeStamp) {
        _d->timeStamp = KGlobal::locale()->formatDateTime(timeStamp, KGlobal::locale()->dateFormat());
    }
}

Aki::Irc::Message::State
Message::state() const
{
    return _d->state;
}

const Aki::Irc::NickInfo&
Message::target() const
{
    return _d->target;
}

const KDateTime&
Message::timeStamp() const
{
    return _d->timeStamp;
}

QDebug operator<<(QDebug dbg, const Aki::Irc::Message& message)
{
    dbg << "Sender: " << message.sender().hostmask() << '\n'
        << "Target: " << message.target().hostmask() << '\n'
        << "Message: " << message.plainText() << '\n'
        << "RTL: " << message.isRightToLeft() << '\n'
        << "State: " << (message.state() == Aki::Irc::Message::Error) ? "Error" :
            (message.state() == Aki::Irc::Message::Sending) ? "Sending" :
            (message.state() == Aki::Irc::Message::Sent) ? "Sent" : "Unknown" << '\n'
        << "TimeStamp: " << message.timeStamp() << '\n'
        << "Type: " << (message.type() == Aki::Irc::Message::Action) ? "CTCP Action" :
                       (message.type() == Aki::Irc::Message::Ctcp) ? "CTCP" :
                       (message.type() == Aki::Irc::Message::DccChat) ? "DCC Chat" :
                       (message.type() == Aki::Irc::Message::DccFile) ? "DCC File" :
                       (message.type() == Aki::Irc::Message::Highlight) ? "Highlight" : "Normal" << '\n'
        << "Direction: " << (message.direction() == Aki::Irc::Message::Incoming) ? "Incoming" : "Outgoing";
    return dbg;
}
