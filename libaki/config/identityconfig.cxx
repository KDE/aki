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

#include "identityconfig.hpp"
#include <KDE/KUser>
using namespace Aki;

IdentityConfig::IdentityConfig(QObject* parent)
    : ConfigFile("akiidentityrc", parent)
{
}

IdentityConfig::~IdentityConfig()
{
}

QString
IdentityConfig::awayMessage()
{
    return currentGroup().readEntry("AwayMessage",
                                    QString("/me is away from the computer now."));
}

QString
IdentityConfig::awayNickname()
{
    KUser user(KUser::UseRealUserID);
    return currentGroup().readEntry("AwayNickname",
                                    user.loginName() + "|away");
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

QString
IdentityConfig::kickMessage()
{
    return currentGroup().readEntry("KickMessage",
                                    QString("Bye bye!! :)"));
}

QString
IdentityConfig::name()
{
    return currentGroup(),name();
}

QStringList
IdentityConfig::nicknameList()
{
    KUser user(KUser::UseRealUserID);
    QString nickname(user.loginName() + QLatin1Char(',') + user.loginName() + "_,"
                     + user.loginName() + "__");
    QString tmp = currentGroup().readEntry("Nicknames", nickname);

    return tmp.split(QLatin1Char(','), QString::SkipEmptyParts);
}

QString
IdentityConfig::partMessage()
{
    return currentGroup().readEntry("PartMessage",
                                    QString("Aki IRC Client: %v"));
}

QString
IdentityConfig::quitMessage()
{
    return currentGroup().readEntry("QuitMessage",
                                    QString("Aki IRC Client: %v"));
}

QString
IdentityConfig::realName()
{
    KUser user(KUser::UseRealUserID);
    QString name = user.property(KUser::FullName).toString();
    if (name.isEmpty() || name.isNull()) {
        name.clear();
    }

    return currentGroup().readEntry("RealName", name);
}

QString
IdentityConfig::returnMessage()
{
    return currentGroup().readEntry("ReturnMessage",
                                    QString("/me is back!"));
}

void
IdentityConfig::setAwayMessage(const QString& message)
{
    currentGroup().writeEntry("AwayMessage", message);
}

void
IdentityConfig::setAwayNickname(const QString& nick)
{
    currentGroup().writeEntry("AwayNickname", nick);
}

void
IdentityConfig::setKickMessage(const QString& message)
{
    currentGroup().writeEntry("KickMessage", message);
}

void
IdentityConfig::setMarkLastPosition(bool enabled)
{
    currentGroup().writeEntry("MarkLastPositionEnabled", enabled);
}

void
IdentityConfig::setName(const QString& name)
{
    if(this->name() != name) {
        deleteGroup(this->name());
        createGroup(name);
    }
}

void
IdentityConfig::setNicknameList(const QStringList& nicknames)
{
    currentGroup().writeEntry("Nicknames", nicknames);
}

void
IdentityConfig::setPartMessage(const QString& message)
{
    currentGroup().writeEntry("PartMessage", message);
}

void
IdentityConfig::setQuitMessage(const QString& message)
{
    currentGroup().writeEntry("QuitMessage", message);
}

void
IdentityConfig::setRealName(const QString& name)
{
    currentGroup().writeEntry("RealName", name);
}

void
IdentityConfig::setReturnMessage(const QString& message)
{
    currentGroup().writeEntry("ReturnMessage", message);
}

void
IdentityConfig::setUseAwayMessages(bool enabled)
{
    currentGroup().writeEntry("AwayMessagesEnabled", enabled);
}
