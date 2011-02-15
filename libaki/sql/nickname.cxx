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

#include "nickname.hpp"
#include "private/nickname_p.hpp"
using namespace Aki;
using namespace Sql;

AKI_REGISTER_SQL_TABLE(Aki::Sql::Nickname, nickname)

Nickname::Nickname(QObject* parent)
    : Aki::Sql::Table(parent)
{
    _d.reset(new Aki::Sql::NicknamePrivate(this));
}

Nickname::~Nickname()
{
}

int
Nickname::id() const
{
    return _d->id;
}

QString
Nickname::nickname() const
{
    return _d->nickname;
}

int
Nickname::nicknameIdentity() const
{
    return _d->nicknameIdentity;
}

void
Nickname::setId(int id)
{
    _d->id = id;
}

void
Nickname::setNickname(const QString& nickname)
{
    _d->nickname = nickname;
}

void
Nickname::setNicknameIdentity(int id)
{
    _d->nicknameIdentity = id;
}

#include "sql/nickname.moc"
