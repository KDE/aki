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
using namespace Aki;
using namespace Sql;

AKI_REGISTER_SQL_TABLE(Aki::Sql::Identity, identity)

Identity::Identity(QObject* parent)
    : Aki::Sql::Table(parent)
{
}

Identity::~Identity()
{

}

QString
Identity::awayMessage() const
{
    return QString();
}

QString
Identity::awayNickname() const
{
    return QString();
}

int
Identity::id() const
{
    return 0;
}

bool
Identity::isMarkLastPositionEnabled() const
{
    return false;
}

bool
Identity::isMessagesEnabled() const
{
    return false;
}

QString
Identity::kickMessage() const
{
    return QString();
}

QString
Identity::name() const
{
    return QString();
}

QString
Identity::partMessage() const
{
    return QString();
}

QString
Identity::quitMessage() const
{
    return QString();
}

QString
Identity::realName() const
{
    return QString();
}

QString
Identity::returnMessage() const
{
    return QString();
}

void
Identity::setAwayMessage(const QString& message)
{
    Q_UNUSED(message)
}

void
Identity::setAwayNickname(const QString& nickname)
{
    Q_UNUSED(nickname)
}

void
Identity::setId(int id)
{
    Q_UNUSED(id)
}

void
Identity::setKickMessage(const QString& message)
{
    Q_UNUSED(message)
}

void
Identity::setMarkLastPositionEnabled(bool enable)
{
    Q_UNUSED(enable)
}

void
Identity::setMessagesEnabled(bool enable)
{
    Q_UNUSED(enable)
}

void
Identity::setName(const QString& name)
{
    Q_UNUSED(name)
}

void
Identity::setPartMessage(const QString& message)
{
    Q_UNUSED(message)
}

void
Identity::setQuitMessage(const QString& message)
{
    Q_UNUSED(message)
}

void
Identity::setRealName(const QString& name)
{
    Q_UNUSED(name)
}

void
Identity::setReturnMessage(const QString& message)
{
    Q_UNUSED(message)
}
