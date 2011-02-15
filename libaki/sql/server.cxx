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

#include "server.hpp"
#include "private/server_p.hpp"
using namespace Aki;
using namespace Sql;

AKI_REGISTER_SQL_TABLE(Aki::Sql::Server, server)

Server::Server(QObject* parent)
    : Aki::Sql::Table(parent),
    _d(new Aki::Sql::ServerPrivate)
{
}

Server::~Server()
{
}

QString
Server::defaultEncoding() const
{
    return _d->defaultEncoding;
}

QString
Server::encoding() const
{
    return _d->encoding;
}

int
Server::id() const
{
    return _d->id;
}

bool
Server::isAutoIdentify() const
{
    return _d->isAutoIdentify;
}

bool
Server::isAutoJoinChannels() const
{
    return _d->isAutoJoinChannels;
}

bool
Server::isAutoReconnect() const
{
    return _d->isAutoReconnect;
}

bool
Server::isConnectOnStartup() const
{
    return _d->isConnectOnStartup;
}

bool
Server::isConnectToRandomServer() const
{
    return _d->isConnectToRandomServer;
}

QString
Server::name() const
{
    return _d->name;
}

int
Server::retryAttemptCount() const
{
    return _d->retryAttemptCount;
}

int
Server::retryInterval() const
{
    return _d->retryInterval;
}

int
Server::serverIdentity() const
{
    return _d->serverIdentity;
}

QString
Server::serviceName() const
{
    return _d->serviceName;
}

QString
Server::servicePassword() const
{
    return _d->servicePassword;
}

void
Server::setAutoIdentify(bool enable)
{
    _d->isAutoIdentify = enable;
}

void
Server::setAutoJoinChannels(bool enable)
{
    _d->isAutoJoinChannels = enable;
}

void
Server::setAutoReconnect(bool enable)
{
    _d->isAutoReconnect = enable;
}

void
Server::setConnectOnStartup(bool enable)
{
    _d->isConnectOnStartup = enable;
}

void
Server::setConnectToRandomServer(bool enable)
{
    _d->isConnectToRandomServer = enable;
}

void
Server::setDefaultEncoding(const QString& encoding)
{
    _d->defaultEncoding = encoding;
}

void
Server::setEncoding(const QString& encoding)
{
    _d->encoding = encoding;
}

void
Server::setId(int id)
{
    _d->id = id;
}

void
Server::setName(const QString& name)
{
    _d->name = name;
}

void
Server::setRetryAttemptCount(int count)
{
    _d->retryAttemptCount = count;
}

void
Server::setRetryInterval(int seconds)
{
    _d->retryInterval = seconds;
}

void
Server::setServerIdentity(int id)
{
    _d->serverIdentity = id;
}

void
Server::setServiceName(const QString& name)
{
    _d->serviceName = name;
}

void
Server::setServicePassword(const QString& password)
{
    _d->servicePassword = password;
}

#include "sql/server.moc"
