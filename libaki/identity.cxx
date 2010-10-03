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
#include <QtCore/QSharedPointer>
using namespace Aki;

Identity::Identity()
{
    _d.reset(new Aki::IdentityPrivate);
}

Identity::Identity(const QString& name)
{
    _d.reset(new Aki::IdentityPrivate);
    setName(name);
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

bool
Identity::isAwayMessagesEnabled() const
{
    return _d->isAwayMessagesEnabled;
}

bool
Identity::isMarkLastPositionEnabled() const
{
    return _d->isMarkLastPositionEnabled;
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
Identity::nickname(int index) const
{
    return _d->nicknameList.value(index, QString());
}

QStringList
Identity::nicknameList() const
{
    return _d->nicknameList;
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
Identity::setAwayNickname(const QString& nick)
{
    _d->awayNickname = nick;
}

void
Identity::setKickMessage(const QString& message)
{
    _d->kickMessage = message;
}

void
Identity::setMarkLastPosition(bool enabled)
{
    _d->isMarkLastPositionEnabled = enabled;
}

void
Identity::setName(const QString& name)
{
    _d->name = name;
}

void
Identity::setNickname(int index, const QString& nickname)
{
    _d->nicknameList.replace(index, nickname);
}

void
Identity::setNicknameList(const QStringList& nicknames)
{
    _d->nicknameList = nicknames;
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

void
Identity::setUseAwayMessages(bool enabled)
{
    _d->isAwayMessagesEnabled = enabled;
}
