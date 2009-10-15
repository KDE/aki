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

#include "identityconfig.h"
#include "akiversion.h"
#include <KUser>
using namespace Aki;

IdentityConfig::IdentityConfig(QObject *parent)
    : Aki::ConfigFile("akiidentityrc", parent)
{
}

IdentityConfig::~IdentityConfig()
{
}

QString
IdentityConfig::awayMessage()
{
    return currentGroup().readEntry("AwayMessage", "/me is away from the computer now.");
}

QString
IdentityConfig::awayNickname()
{
    KUser user(KUser::UseRealUserID);
    return currentGroup().readEntry("AwayNickname", user.loginName() + "|away");
}

QString
IdentityConfig::kickMessage()
{
    return currentGroup().readEntry("KickMessage", "Bye bye!! :)");
}

bool
IdentityConfig::isAwayMessagesEnabled()
{
    return currentGroup().readEntry("AwayMessagesEnabled", false);
}

bool
IdentityConfig::isMarkLastPositionEnabled()
{
    return currentGroup().readEntry("MarkLastPositionEnabled", false);
}

QStringList
IdentityConfig::nicknameList()
{
    KUser user(KUser::UseRealUserID);
    QString nickname(user.loginName() + QChar(',') + user.loginName() + "_," + user.loginName() + "__");
    QString tmp = currentGroup().readEntry("Nicknames", nickname);
    
    return tmp.split(QChar(','), QString::SkipEmptyParts);
}

QString
IdentityConfig::partMessage()
{
    return currentGroup().readEntry("PartMessage", QString("Aki IRC Client: %1").arg(AKI_VERSION_STR));
}

QString
IdentityConfig::quitMessage()
{
    return currentGroup().readEntry("QuitMessage", QString("Aki IRC Client: %1").arg(AKI_VERSION_STR));
}

QString
IdentityConfig::realName()
{
    KUser user(KUser::UseRealUserID);
    QString name = user.property(KUser::FullName).toString();
    if (name.isEmpty()) {
        name = "";
    }
    
    return currentGroup().readEntry("RealName", name);
}

QString
IdentityConfig::returnMessage()
{
    return currentGroup().readEntry("ReturnMessage", "/me is back!");
}

void
IdentityConfig::setAwayMessage(const QString &message)
{
    currentGroup().writeEntry("AwayMessage", message);
}

void
IdentityConfig::setAwayNickname(const QString &nick)
{
    currentGroup().writeEntry("AwayNickname", nick);
}

void
IdentityConfig::setKickMessage(const QString &message)
{
    currentGroup().writeEntry("KickMessage", message);
}

void
IdentityConfig::setMarkLastPosition(bool enable)
{
    currentGroup().writeEntry("MarkLastPositionEnabled", enable);
}

void
IdentityConfig::setNicknameList(const QStringList &nicknames)
{
    QString nicknameList = nicknames.join(QChar(','));
    currentGroup().writeEntry("Nicknames", nicknameList);
}

void
IdentityConfig::setPartMessage(const QString &message)
{
    currentGroup().writeEntry("PartMessage", message);
}

void
IdentityConfig::setQuitMessage(const QString &message)
{
    currentGroup().writeEntry("QuitMessage", message);
}

void
IdentityConfig::setRealName(const QString &name)
{
    currentGroup().writeEntry("RealName", name);
}

void
IdentityConfig::setReturnMessage(const QString &message)
{
    currentGroup().writeEntry("ReturnMessage", message);
}

void
IdentityConfig::setUseAwayMessages(bool enable)
{
    currentGroup().writeEntry("AwayMessagesEnabled", enable);
}

QString
IdentityConfig::name() const
{
    return m_name;
}

void
IdentityConfig::setName(const QString &name)
{
    m_name = name;
}
