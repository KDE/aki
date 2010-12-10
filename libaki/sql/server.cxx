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

#include "server.hpp"
using namespace Aki;
using namespace Sql;

AKI_REGISTER_SQL_TABLE(Aki::Sql::Server, server)

Server::Server(QObject* parent)
    : Aki::Sql::Table(parent)
{
}

Server::~Server()
{
}

QString
Server::defaultEncoding() const
{
    return QString();
}

QString
Server::encoding() const
{
    return QString();
}

int
Server::id() const
{
    return 0;
}

bool
Server::isAutoIdentify() const
{
    return false;
}

bool
Server::isAutoJoinChannels() const
{
    return false;
}

bool
Server::isAutoReconnect() const
{
    return false;
}

bool
Server::isConnectOnStartup() const
{
    return false;
}

bool
Server::isConnectToRandomServer() const
{
    return false;
}

QString
Server::name() const
{
    return QString();
}

int
Server::retryAttemptCount() const
{
    return 0;
}

int
Server::retryInterval() const
{
    return 0;
}

int
Server::serverIdentity() const
{
    return 0;
}

QString
Server::serviceName() const
{
    return QString();
}

QString
Server::servicePassword() const
{
    return QString();
}

void
Server::setAutoIdentify(bool enable)
{
    Q_UNUSED(enable);
}

void
Server::setAutoJoinChannels(bool enable)
{
    Q_UNUSED(enable);
}

void
Server::setAutoReconnect(bool enable)
{
    Q_UNUSED(enable);
}

void
Server::setConnectOnStartup(bool enable)
{
    Q_UNUSED(enable);
}

void
Server::setConnectToRandomServer(bool enable)
{
    Q_UNUSED(enable);
}

void
Server::setDefaultEncoding(const QString& encoding)
{
    Q_UNUSED(encoding);
}

void
Server::setEncoding(const QString& encoding)
{
    Q_UNUSED(encoding);
}

void
Server::setId(int id)
{
    Q_UNUSED(id)
}

void
Server::setName(const QString& name)
{
    Q_UNUSED(name)
}

void
Server::setRetryAttemptCount(int count)
{
    Q_UNUSED(count)
}

void
Server::setRetryInterval(int seconds)
{
    Q_UNUSED(seconds)
}

void
Server::setServerIdentity(int id)
{
    Q_UNUSED(id)
}

void
Server::setServiceName(const QString& name)
{
    Q_UNUSED(name)
}

void
Server::setServicePassword(const QString& password)
{
    Q_UNUSED(password)
}
