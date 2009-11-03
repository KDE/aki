/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "identity.h"
using namespace Aki;

namespace Aki
{
class IdentityPrivate
{
public:
    IdentityPrivate()
        : awayMessage(QString()),
        awayNickname(QString()),
        name(QString()),
        kickMessage(QString()),
        partMessage(QString()),
        quitMessage(QString()),
        realName(QString()),
        returnMessage(QString()),
        isAwayMessagesEnabled(false),
        isMarkLastPositionEnabled(false)
    {
        nicknameList.clear();
    }

    QStringList nicknameList;
    QString awayMessage;
    QString awayNickname;
    QString name;
    QString kickMessage;
    QString partMessage;
    QString quitMessage;
    QString realName;
    QString returnMessage;
    bool isAwayMessagesEnabled;
    bool isMarkLastPositionEnabled;
}; // End of class IdentityPrivate.
} // End of namespace Aki.

Identity::Identity()
{
    d.reset(new Aki::IdentityPrivate);
}

Identity::Identity(const QString &name)
{
    d.reset(new Aki::IdentityPrivate);
    d->name = name;
}

Identity::~Identity()
{
}

QString
Identity::awayMessage() const
{
    return d->awayMessage;
}

QString
Identity::awayNickname() const
{
    return d->awayNickname;
}

QString
Identity::name() const
{
    return d->name;
}

bool
Identity::isAwayMessagesEnabled() const
{
    return d->isAwayMessagesEnabled;
}

bool
Identity::isMarkLastPositionEnabled() const
{
    return d->isMarkLastPositionEnabled;
}

QString
Identity::kickMessage() const
{
    return d->kickMessage;
}

QString
Identity::nickname(int index) const
{
    return d->nicknameList.value(index);
}

QStringList
Identity::nicknameList() const
{
    return d->nicknameList;
}

QString
Identity::partMessage() const
{
    return d->partMessage;
}

QString
Identity::quitMessage() const
{
    return d->quitMessage;
}

QString
Identity::realName() const
{
    return d->realName;
}

QString
Identity::returnMessage() const
{
    return d->returnMessage;
}

void
Identity::setAwayMessage(const QString &message)
{
    d->awayMessage = message;
}

void
Identity::setAwayNickname(const QString &nickname)
{
    d->awayNickname = nickname;
}

void
Identity::setName(const QString &name)
{
    d->name = name;
}

void
Identity::setKickMessage(const QString &message)
{
    d->kickMessage = message;
}

void
Identity::setMarkLastPosition(bool enable)
{
    d->isMarkLastPositionEnabled = enable;
}

void
Identity::setNickname(int index, const QString &nickname)
{
    d->nicknameList.replace(index, nickname);
}

void
Identity::setNicknameList(const QStringList &nicknames)
{
    d->nicknameList = nicknames;
}

void
Identity::setPartMessage(const QString &message)
{
    d->partMessage = message;
}

void
Identity::setQuitMessage(const QString &message)
{
    d->quitMessage = message;
}

void
Identity::setRealName(const QString &name)
{
    d->realName = name;
}

void
Identity::setReturnMessage(const QString &message)
{
    d->returnMessage = message;
}

void
Identity::setUseAwayMessages(bool enable)
{
    d->isAwayMessagesEnabled = enable;
}
