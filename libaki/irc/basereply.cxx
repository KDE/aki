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

#include "basereply.hpp"
#include "private/basereply_p.hpp"
using namespace Aki;
using namespace Irc;

BaseReply::BaseReply()
    : _d(new Aki::Irc::BaseReplyPrivate())
{
}

BaseReply::BaseReply(const Aki::Irc::BaseReply& other)
    : _d(other._d)
{
    Q_UNUSED(other)
}

BaseReply::~BaseReply()
{
}

Aki::Irc::BaseReply&
BaseReply::operator=(const Aki::Irc::BaseReply& other)
{
    if (_d != other._d) {
        _d = other._d;
        _d->command = other.command();
        _d->isNumeric = other.isNumeric();
        _d->message = other.message();
        _d->params = other.params();
        _d->replyCode = other.replyCode();
        _d->sender = other.sender();
    }
    return *this;
}

bool
BaseReply::operator==(const Aki::Irc::BaseReply& other) const
{
    Q_UNUSED(other)
    return false;
}

QString
BaseReply::command() const
{
    return _d->command;
}

bool
BaseReply::isNumeric() const
{
    return _d->isNumeric;
}

QString
BaseReply::message() const
{
    return _d->message;
}

QStringList
BaseReply::params() const
{
    return _d->params;
}

Aki::Irc::BaseReply
BaseReply::parse(const QString& line)
{
    if (line.isEmpty()) {
        return Aki::Irc::BaseReply();
    }

    QString tmp = line;

    Aki::Irc::BaseReply reply;
    reply._d->message = line;

    if (tmp.startsWith(':')) {
        Aki::Irc::NickInfo sender;
        sender.setHostmask(Aki::Irc::BaseReplyPrivate::removeStringToFirstWhitespace(&tmp, 1, 1));
        reply._d->sender = sender;
    }

    reply._d->command = Aki::Irc::BaseReplyPrivate::removeStringToFirstWhitespace(&tmp);
    reply._d->replyCode = static_cast<Aki::Irc::ReplyCodes>(reply.replyCode());

    while (!line.isEmpty()) {
        if (line.startsWith(':')) {
            tmp.remove(0, 1);
            reply._d->params << tmp;
            tmp.clear();
        } else {
            reply._d->params << Aki::Irc::BaseReplyPrivate::removeStringToFirstWhitespace(&tmp);
        }
    }

    if (reply.params().isEmpty()) {
        return reply;
    }

    reply._d->params.removeAll("");
    return reply;
}

Aki::Irc::ReplyCodes
BaseReply::replyCode() const
{
    return _d->replyCode;
}

Aki::Irc::NickInfo
BaseReply::sender() const
{
    return _d->sender;
}

QDebug
operator<<(QDebug dbg, const Aki::Irc::BaseReply& reply)
{
    dbg << "\nSender: " << reply.sender().hostmask() << '\n'
    << "---- Host: " << reply.sender().host() << '\n'
    << "---- Nick: " << reply.sender().nick() << '\n'
    << "---- User: " << reply.sender().user() << '\n'
    << "Command: " << reply.command() << '\n'
    << "IsNumeric: " << reply.isNumeric() << '\n'
    << "Numeric: " << reply.isNumeric() << '\n'
    << "Message: " << reply.message().remove("\r\n") << '\n'
    << "Params: " << reply.params();
    return dbg;
}
