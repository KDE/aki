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

#include "nickinfo_p.hpp"
#include "irc/nickinfo.hpp"
using namespace Aki::Irc;

NickInfoPrivate::NickInfoPrivate(Aki::Irc::NickInfo* qq)
    : hostmask(QString()),
    _q(qq)
{
}

QString
NickInfoPrivate::host() const
{
    QString tmp = hostmask.section('!', 1);
    return tmp.section('@', 1);
}

QString
NickInfoPrivate::nick() const
{
    return hostmask.section('!', 0, 0);
}

QString
NickInfoPrivate::user() const
{
    QString userHost = hostmask.section('!', 1);
    if (!userHost.isEmpty()) {
        return userHost.section('@', 0, 0);
    }

    return QString();
}