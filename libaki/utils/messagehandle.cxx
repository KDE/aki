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

#include "messagehandle.hpp"
using namespace Aki;

MessageHandle::MessageHandle(QObject* parent, Aki::Irc::ReplyCodes replyCode, const char* slot, const QString& help,
                             Aki::MessageHandle::Type type)
    : KAction(parent),
    _help(help),
    _replyCode(replyCode),
    _type(type)
{
    connect(parent, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));
    connect(this, SIGNAL(triggered(bool)), parent, slot);
}

MessageHandle::~MessageHandle()
{
}

QString
MessageHandle::help() const
{
    return _help;
}

Aki::Irc::ReplyCodes
MessageHandle::replyCode()
{
    return _replyCode;
}

Aki::MessageHandle::Type
MessageHandle::type() const
{
    return _type;
}

#include "utils/messagehandle.moc"
