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

#include "identity.hpp"
#include "private/identity_p.hpp"
using namespace Aki;
using namespace Sql;

AKI_REGISTER_SQL_TABLE(Aki::Sql::Identity, identity)

Identity::Identity(QObject* parent)
    : Aki::Sql::Table(parent)
{
    _d.reset(new Aki::Sql::IdentityPrivate(this));
}

Identity::~Identity()
{
}

QString
Identity::awayMessage() const
{
    return _d->awayMessage;
}

QString
Identity::awayNickname() const
{
    return _d->awayNickname;
}

int
Identity::id() const
{
    return _d->id;
}

bool
Identity::isMarkLastPositionEnabled() const
{
    return _d->markLastPosition;
}

bool
Identity::isMessagesEnabled() const
{
    return _d->enableMessages;
}

QString
Identity::kickMessage() const
{
    return _d->kickMessage;
}

QString
Identity::name() const
{
    return _d->name;
}

QString
Identity::partMessage() const
{
    return _d->partMessage;
}

QString
Identity::quitMessage() const
{
    return _d->quitMessage;
}

QString
Identity::realName() const
{
    return _d->realName;
}

QString
Identity::returnMessage() const
{
    return _d->returnMessage;
}

void
Identity::setAwayMessage(const QString& message)
{
    _d->awayMessage = message;
}

void
Identity::setAwayNickname(const QString& nickname)
{
    _d->awayNickname = nickname;
}

void
Identity::setId(int id)
{
    _d->id = id;
}

void
Identity::setKickMessage(const QString& message)
{
    _d->kickMessage = message;
}

void
Identity::setMarkLastPositionEnabled(bool enable)
{
    _d->markLastPosition = enable;
}

void
Identity::setMessagesEnabled(bool enable)
{
    _d->enableMessages = enable;
}

void
Identity::setName(const QString& name)
{
    _d->name = name;
}

void
Identity::setPartMessage(const QString& message)
{
    _d->partMessage = message;
}

void
Identity::setQuitMessage(const QString& message)
{
    _d->quitMessage = message;
}

void
Identity::setRealName(const QString& name)
{
    _d->realName = name;
}

void
Identity::setReturnMessage(const QString& message)
{
    _d->returnMessage = message;
}

#include "sql/identity.moc"
