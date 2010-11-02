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
using namespace Aki;
using namespace Irc;

Message::Message()
{
    _d.reset(new Aki::Irc::MessagePrivate(this));
}

Message::Message(const QString& line)
{
    _d.reset(new Aki::Irc::MessagePrivate(this));
    parse(line);
}

Message::Message(const Aki::Irc::Message& message)
{
    _d.reset(new Aki::Irc::MessagePrivate(this));
    _d->sender = message.sender();
    _d->command = message.command();
    _d->isNumeric = message.isNumeric();
    _d->message = message.message();
    _d->numeric = message.numeric();
    _d->params = message.params();
}

Aki::Irc::Message&
Message::operator=(const Aki::Irc::Message& rhs)
{
    _d.reset(new Aki::Irc::MessagePrivate(this));
    _d->sender = rhs.sender();
    _d->command = rhs.command();
    _d->isNumeric = rhs.isNumeric();
    _d->message = rhs.message();
    _d->numeric = rhs.numeric();
    _d->params = rhs.params();

    return *this;
}


Message::~Message()
{
}

QString
Message::command() const
{
    return _d->command;
}

bool
Message::isNumeric() const
{
    return _d->isNumeric;
}

QString
Message::message() const
{
    if (_d->message.endsWith("\r\n")) {
        return _d->message;
    }

    return _d->message + "\r\n";
}

Aki::Irc::ReplyCodes
Message::numeric() const
{
    return _d->numeric;
}

QStringList
Message::params() const
{
    return _d->params;
}

void
Message::parse(const QString& str)
{
    QString line = str;
    _d->message = str;

    if (line.startsWith(':')) {
        _d->sender.setHostmask(_d->removeStringToFirstWhitespace(&line, 1, 1));
    }

    _d->command = _d->removeStringToFirstWhitespace(&line);
    _d->numeric = static_cast<Aki::Irc::ReplyCodes>(_d->command.toInt(&_d->isNumeric));

    while (not line.isEmpty()) {
        if (line.startsWith(':')) {
            line.remove(0, 1);
            _d->params << line;
            line.clear();
        } else {
            _d->params << _d->removeStringToFirstWhitespace(&line);
        }
    }

    if (_d->params.isEmpty()) {
        return;
    }

    _d->params.removeAll("");
}

Aki::Irc::NickInfo
Message::sender() const
{
    return _d->sender;
}

QDebug operator<<(QDebug dbg, const Aki::Irc::Message& message)
{
    dbg << "\nSender: " << message.sender().hostmask() << '\n'
    << "---- Host: " << message.sender().host() << '\n'
    << "---- Nick: " << message.sender().nick() << '\n'
    << "---- User: " << message.sender().user() << '\n'
    << "Command: " << message.command() << '\n'
    << "IsNumeric: " << message.isNumeric() << '\n'
    << "Numeric: " << message.numeric() << '\n'
    << "Message: " << message.message().remove("\r\n") << '\n'
    << "Params: " << message.params();
    return dbg;
}
