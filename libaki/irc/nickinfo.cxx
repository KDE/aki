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

#include "nickinfo.hpp"
#include "debughelper.hpp"
#include "private/nickinfo_p.hpp"
using namespace Aki;
using namespace Irc;

NickInfo::NickInfo()
    : _d(new Aki::Irc::NickInfoPrivate)
{
}

NickInfo::NickInfo(const QString& hostmask)
    : _d(new Aki::Irc::NickInfoPrivate)
{
    setHostmask(hostmask);
}

NickInfo::NickInfo(const Aki::Irc::NickInfo& other)
    : _d(other._d)
{
    setHostmask(other.hostmask());
}

NickInfo::~NickInfo()
{
}

Aki::Irc::NickInfo&
NickInfo::operator=(const Aki::Irc::NickInfo& other)
{
    _d = other._d;
    return *this;
}

QString
NickInfo::host() const
{
    return _d->host();
}

QString
NickInfo::hostmask() const
{
    return _d->hostmask;
}

QString
NickInfo::nick() const
{
    return _d->nick();
}

void
NickInfo::setHostmask(const QString& hostmask)
{
    _d->hostmask = hostmask;
}

QString
NickInfo::user() const
{
    return _d->user();
}

QDebug operator<<(QDebug dbg, const Aki::Irc::NickInfo& nickInfo)
{
    dbg << "Hostmask: " << nickInfo.hostmask() << '\n'
        << "Host: " << nickInfo.host() << '\n'
        << "Nick: " << nickInfo.nick() << '\n'
        << "User: " << nickInfo.user();
    return dbg;
}
