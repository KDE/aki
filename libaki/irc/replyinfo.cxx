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

#include "replyinfo.hpp"
#include "private/replyinfo_p.hpp"
using namespace Aki;
using namespace Irc;

ReplyInfo::ReplyInfo()
    : _d(new Aki::Irc::ReplyInfoPrivate)
{
}

ReplyInfo::ReplyInfo(const Aki::Irc::NickInfo& sender, const QString& command, const QString& message,
                     const QStringList& params, Aki::Irc::ReplyCodes replyCode)
    : _d(new Aki::Irc::ReplyInfoPrivate)
{
    _d->sender = sender;
    _d->command = command;
    _d->message = message;
    _d->params = params;
    _d->replyCode = replyCode;
}

ReplyInfo::ReplyInfo(const Aki::Irc::ReplyInfo& other)
    : _d(other._d)
{
}

ReplyInfo::~ReplyInfo()
{
}

Aki::Irc::ReplyInfo&
ReplyInfo::operator=(const Aki::Irc::ReplyInfo& other)
{
    _d = other._d;
    return *this;
}

QString
ReplyInfo::command() const
{
    return _d->command;
}

bool
ReplyInfo::isNumeric() const
{
    return (numeric() != Aki::Irc::RPL_NULL);
}

QString
ReplyInfo::message() const
{
    return _d->message;
}

Aki::Irc::ReplyCodes
ReplyInfo::numeric() const
{
    return _d->replyCode;
}

QStringList
ReplyInfo::params() const
{
    return _d->params;
}

Aki::Irc::NickInfo
ReplyInfo::sender() const
{
    return _d->sender;
}

QDebug
operator<<(QDebug dbg, const Aki::Irc::ReplyInfo& replyInfo)
{
    dbg << "Command: " << replyInfo.command() << '\n'
        << "Has Numeric: " << replyInfo.isNumeric() << '\n'
        << "Message: " << replyInfo.message() << '\n'
        << "Numeric: " << replyInfo.numeric() << '\n'
        << "Params: " << replyInfo.params() << '\n'
        << "Sender: " << replyInfo.sender();
    return dbg;
}
