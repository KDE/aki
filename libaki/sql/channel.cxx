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

#include "channel.hpp"
using namespace Aki;
using namespace Sql;

Channel::Channel(QObject* parent)
    : Aki::Sql::Table(parent)
{
}

Channel::~Channel()
{
}

int
Channel::channelServer() const
{
    return 0;
}

int
Channel::id() const
{
    return 0;
}

QString
Channel::name() const
{
    return QString();
}

QString
Channel::password() const
{
    return QString();
}

void
Channel::setChannelServer(int id)
{
    Q_UNUSED(id)
}

void
Channel::setId(int id)
{
    Q_UNUSED(id)
}

void
Channel::setName(const QString& name)
{
    Q_UNUSED(name)
}

void
Channel::setPassword(const QString& password)
{
    Q_UNUSED(password)
}
