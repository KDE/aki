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

Identity::Identity()
{
}

Identity::Identity(const QString &name)
    : m_name(name)
{
}

QString
Identity::awayMessage() const
{
    return m_awayMessage;
}

QString
Identity::awayNickname() const
{
    return m_awayNickname;
}

QString
Identity::name() const
{
    return m_name;
}

bool
Identity::isAwayMessagesEnabled() const
{
    return m_isAwayMessagesEnabled;
}

bool
Identity::isMarkLastPositionEnabled() const
{
    return m_isMarkLastPositionEnabled;
}

QString
Identity::kickMessage() const
{
    return m_kickMessage;
}

QString
Identity::nickname(int index) const
{
    return m_nicknameList.value(index);
}

QStringList
Identity::nicknameList() const
{
    return m_nicknameList;
}

QString
Identity::partMessage() const
{
    return m_partMessage;
}

QString
Identity::quitMessage() const
{
    return m_quitMessage;
}

QString
Identity::realName() const
{
    return m_realName;
}

QString
Identity::returnMessage() const
{
    return m_returnMessage;
}

void
Identity::setAwayMessage(const QString &message)
{
    m_awayMessage = message;
}

void
Identity::setAwayNickname(const QString &nickname)
{
    m_awayNickname = nickname;
}

void
Identity::setName(const QString &name)
{
    m_name = name;
}

void
Identity::setKickMessage(const QString &message)
{
    m_kickMessage = message;
}

void
Identity::setMarkLastPosition(bool enable)
{
    m_isMarkLastPositionEnabled = enable;
}

void
Identity::setNickname(int index, const QString &nickname)
{
    m_nicknameList.replace(index, nickname);
}

void
Identity::setNicknameList(const QStringList &nicknames)
{
    m_nicknameList = nicknames;
}

void
Identity::setPartMessage(const QString &message)
{
    m_partMessage = message;
}

void
Identity::setQuitMessage(const QString &message)
{
    m_quitMessage = message;
}

void
Identity::setRealName(const QString &name)
{
    m_realName = name;
}

void
Identity::setReturnMessage(const QString &message)
{
    m_returnMessage = message;
}

void
Identity::setUseAwayMessages(bool enable)
{
    m_isAwayMessagesEnabled = enable;
}
