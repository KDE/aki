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

#include "sqlnetwork.hpp"
#include "aki.hpp"
#include "utils/sqlidentity.hpp"
#include "private/sqlnetwork_p.hpp"
#include <QtCore/QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
using namespace Aki;

SqlNetwork::SqlNetwork(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::SqlNetworkPrivate(this));
    _d->server = Aki::Server::Ptr(new Aki::Server);
}

SqlNetwork::~SqlNetwork()
{
}

bool
SqlNetwork::operator!=(const Aki::SqlNetwork& rhs) const
{
    return !(*this == rhs);
}

bool
SqlNetwork::operator==(const Aki::SqlNetwork& rhs) const
{
    return (isAutoIdentifyEnabled() == rhs.isAutoIdentifyEnabled() &&
            isAutoJoinChannelsEnabled() == rhs.isAutoJoinChannelsEnabled() &&
            isAutoReconnectEnabled() == rhs.isAutoReconnectEnabled() &&
            isConnectOnStartupEnabled() == rhs.isConnectOnStartupEnabled() &&
            isConnectToRandomServerEnabled() == rhs.isConnectToRandomServerEnabled() &&
            isDefaultEncodingEnabled() == rhs.isDefaultEncodingEnabled() &&
            name() == rhs.name() &&
            retryAttemptCount() == rhs.retryAttemptCount() &&
            retryInterval() == rhs.retryInterval() &&
            serverIdentityId() == rhs.serverIdentityId() &&
            serviceName() == rhs.serviceName() &&
            servicePassword() == rhs.servicePassword() &&
            id() == rhs.id());
}

Aki::SqlNetwork::List
SqlNetwork::networksForIdentity(const Aki::SqlIdentity* identity)
{
    SqlNetwork::List list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Server WHERE serverIdentity = ?");
    query.addBindValue(identity->id());

    if (!query.exec()) {
        Aki::SqlNetworkPrivate::checkError(query.lastError());
        return list;
    }

    if (query.size() == 0) {
        return list;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf("id");
    const int encoding = record.indexOf("encoding");
    const int autoIdentify = record.indexOf("autoIdentify");
    const int autoJoinChannels = record.indexOf("autoJoinChannels");
    const int autoReconnect = record.indexOf("autoReconnect");
    const int connectOnStartup = record.indexOf("connectOnStartup");
    const int connectToRandomServer = record.indexOf("connectToRandomServer");
    const int defaultEncoding = record.indexOf("defaultEncoding");
    const int name = record.indexOf("name");
    const int retryAttempts = record.indexOf("retryAttempts");
    const int retryInterval = record.indexOf("retryInterval");
    const int serviceName = record.indexOf("serviceName");
    const int servicePassword = record.indexOf("servicePassword");
    const int serverIdentity = record.indexOf("serverIdentity");

    while (query.next()) {
        Aki::SqlNetwork* network = new Aki::SqlNetwork;
        network->setId(query.value(id).toInt());
        network->setEncoding(query.value(encoding).toByteArray());
        network->setAutoIdentity(query.value(autoIdentify).toBool());
        network->setAutoJoinChannels(query.value(autoJoinChannels).toBool());
        network->setAutoReconnect(query.value(autoReconnect).toBool());
        network->setConnectOnStartup(query.value(connectOnStartup).toBool());
        network->setConnectToRandomServer(query.value(connectToRandomServer).toBool());
        network->setDefaultEncoding(query.value(defaultEncoding).toBool());
        network->setName(query.value(name).toString());
        network->setRetryAttempts(query.value(retryAttempts).toInt());
        network->setRetryInterval(query.value(retryInterval).toInt());
        network->setServiceName(query.value(serviceName).toString());
        network->setServicePassword(query.value(servicePassword).toString());
        network->setServerIdentityId(query.value(serverIdentity).toInt());
        list.append(network);
    }

    return list;
}

Aki::SqlNetwork*
SqlNetwork::newNetwork(const QString& name, const Aki::SqlIdentity* identity)
{
    if (name.isEmpty() || name.isNull()) {
        return 0;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Server (encoding,autoIdentify,autoJoinChannels,autoReconnect,\n\
                  connectOnStartup,connectToRandomServer,defaultEncoding,name,retryAttempts,retryInterval,\n\
                  serviceName,servicePassword,serverIdentity)\n\
                  VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)");

    QScopedPointer<Aki::SqlNetwork> server(new Aki::SqlNetwork);
    server->setAutoIdentity(false);
    server->setAutoJoinChannels(false);
    server->setAutoReconnect(true);
    server->setConnectOnStartup(false);
    server->setConnectToRandomServer(false);
    server->setDefaultEncoding(true);
    server->setEncoding("UTF-8");
    server->setName(name);
    server->setRetryAttempts(0);
    server->setRetryInterval(10);
    server->setServiceName(QString());
    server->setServicePassword(QString());
    server->setServerIdentityId(identity->id());

    query.addBindValue(QString(server->encoding()));
    query.addBindValue(server->isAutoIdentifyEnabled());
    query.addBindValue(server->isAutoJoinChannelsEnabled());
    query.addBindValue(server->isAutoReconnectEnabled());
    query.addBindValue(server->isConnectOnStartupEnabled());
    query.addBindValue(server->isConnectToRandomServerEnabled());
    query.addBindValue(server->isDefaultEncodingEnabled());
    query.addBindValue(server->name());
    query.addBindValue(server->retryAttemptCount());
    query.addBindValue(server->retryInterval());
    query.addBindValue(server->serviceName());
    query.addBindValue(server->servicePassword());
    query.addBindValue(identity->id());

    if (!query.exec()) {
        Aki::SqlNetworkPrivate::checkError(query.lastError());
        return 0;
    }

    server->setId(query.lastInsertId().toInt());

    return server.take();
}

QByteArray
SqlNetwork::encoding() const
{
    return _d->server->encoding();
}

Aki::SqlNetwork*
SqlNetwork::findNetwork(const QString& name, const Aki::SqlIdentity* identity)
{
    if (name.isEmpty() || name.isNull()) {
        return 0;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM Server WHERE name=? AND serverIdentity=? LIMIT 1");
    query.addBindValue(name);
    query.addBindValue(identity->id());

    if (!query.exec()) {
        Aki::SqlNetworkPrivate::checkError(query.lastError());
        return 0;
    }

    if (query.size() == 0) {
        return 0;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf("id");
    const int encoding = record.indexOf("encoding");
    const int autoIdentify = record.indexOf("autoIdentify");
    const int autoJoinChannels = record.indexOf("autoJoinChannels");
    const int autoReconnect = record.indexOf("autoReconnect");
    const int connectOnStartup = record.indexOf("connectOnStartup");
    const int connectToRandomServer = record.indexOf("connectToRandomServer");
    const int defaultEncoding = record.indexOf("defaultEncoding");
    const int nameIndex = record.indexOf("name");
    const int retryAttempts = record.indexOf("retryAttempts");
    const int retryInterval = record.indexOf("retryInterval");
    const int serviceName = record.indexOf("serviceName");
    const int servicePassword = record.indexOf("servicePassword");
    const int serverIdentity = record.indexOf("serverIdentity");

    Aki::SqlNetwork* server = new Aki::SqlNetwork;
    while (query.next()) {
        server->setId(query.value(id).toInt());
        server->setEncoding(query.value(encoding).toByteArray());
        server->setAutoIdentity(query.value(autoIdentify).toBool());
        server->setAutoJoinChannels(query.value(autoJoinChannels).toBool());
        server->setAutoReconnect(query.value(autoReconnect).toBool());
        server->setConnectOnStartup(query.value(connectOnStartup).toBool());
        server->setConnectToRandomServer(query.value(connectToRandomServer).toBool());
        server->setDefaultEncoding(query.value(defaultEncoding).toBool());
        server->setName(query.value(nameIndex).toString());
        server->setRetryAttempts(query.value(retryAttempts).toBool());
        server->setRetryInterval(query.value(retryInterval).toBool());
        server->setServiceName(query.value(serviceName).toString());
        server->setServicePassword(query.value(servicePassword).toString());
        server->setServerIdentityId(query.value(serverIdentity).toInt());
        return server;
    }

    return 0;
}

bool
SqlNetwork::isAutoIdentifyEnabled() const
{
    return _d->server->isAutoIdentifyEnabled();
}

bool
SqlNetwork::isAutoJoinChannelsEnabled() const
{
    return _d->server->isAutoJoinChannelsEnabled();
}

bool
SqlNetwork::isAutoReconnectEnabled() const
{
    return _d->server->isAutoReconnectionEnabled();
}

bool
SqlNetwork::isConnectOnStartupEnabled() const
{
    return _d->server->isConnectOnStartupEnabled();
}

bool
SqlNetwork::isConnectToRandomServerEnabled() const
{
    return _d->server->isConnectToRandomServerEnabled();
}

bool
SqlNetwork::isDefaultEncodingEnabled() const
{
    return _d->server->isDefaultEncodingEnabled();
}

QString
SqlNetwork::name() const
{
    return _d->server->name();
}

int
SqlNetwork::retryAttemptCount() const
{
    return _d->server->retryAttemptCount();
}

int
SqlNetwork::retryInterval() const
{
    return _d->server->retryInterval();
}

QString
SqlNetwork::serviceName() const
{
    return _d->server->serviceName();
}

QString
SqlNetwork::servicePassword() const
{
    return _d->server->servicePassword();
}

void
SqlNetwork::setAutoIdentity(bool enabled)
{
    _d->server->setAutoIdentify(enabled);
}

void
SqlNetwork::setAutoJoinChannels(bool enabled)
{
    _d->server->setAutoJoinChannels(enabled);
}

void
SqlNetwork::setAutoReconnect(bool enabled)
{
    _d->server->setAutoReconnection(enabled);
}

void
SqlNetwork::setConnectOnStartup(bool enabled)
{
    _d->server->setConnectOnStartup(enabled);
}

void
SqlNetwork::setConnectToRandomServer(bool enabled)
{
    _d->server->setConnectToRandomServer(enabled);
}

void
SqlNetwork::setDefaultEncoding(bool enabled)
{
    _d->server->setDefaultEncoding(enabled);
}

void
SqlNetwork::setEncoding(const QByteArray& encoding)
{
    _d->server->setEncoding(encoding);
}

void
SqlNetwork::setId(int id)
{
    _d->id = id;
}

void
SqlNetwork::setName(const QString& name)
{
    _d->server->setName(name);
}

void
SqlNetwork::setRetryAttempts(int count)
{
    _d->server->setRetryAttempts(count);
}

void
SqlNetwork::setRetryInterval(int seconds)
{
    _d->server->setRetryInterval(seconds);
}

void
SqlNetwork::setServiceName(const QString& name)
{
    _d->server->setServiceName(name);
}

void
SqlNetwork::setServicePassword(const QString& password)
{
    _d->server->setServicePassword(password);
}

int
SqlNetwork::id() const
{
    return _d->id;
}

int
SqlNetwork::serverIdentityId() const
{
    return _d->serverId;
}

void
SqlNetwork::setServerIdentityId(int id)
{
    _d->serverId = id;
}

bool
SqlNetwork::save()
{
    QSqlQuery query;
    const QString str("UPDATE Server SET encoding=?, autoIdentify=?, autoJoinChannels=?,"
                      " autoReconnect=?, connectOnStartup=?, connectToRandomServer=?, defaultEncoding=?,"
                      " name=?, retryAttempts=?, retryInterval=?, serviceName=?, servicePassword=?,"
                      " serverIdentity=? WHERE id=?");
    query.prepare(str);
    query.addBindValue(encoding());
    query.addBindValue(isAutoIdentifyEnabled());
    query.addBindValue(isAutoJoinChannelsEnabled());
    query.addBindValue(isAutoReconnectEnabled());
    query.addBindValue(isConnectOnStartupEnabled());
    query.addBindValue(isConnectToRandomServerEnabled());
    query.addBindValue(isDefaultEncodingEnabled());
    query.addBindValue(name());
    query.addBindValue(retryAttemptCount());
    query.addBindValue(retryInterval());
    query.addBindValue(Aki::SqlNetworkPrivate::checkString(serviceName()));
    query.addBindValue(Aki::SqlNetworkPrivate::checkString(servicePassword()));
    query.addBindValue(serverIdentityId());
    query.addBindValue(id());

    if (!query.exec()) {
        Aki::SqlNetworkPrivate::checkError(query.lastError());
        return false;
    }

    return true;
}

bool
SqlNetwork::remove()
{
    QSqlQuery query;
    const QString str("DELETE FROM Server WHERE name=? AND id=?");
    query.prepare(str);
    query.addBindValue(name());
    query.addBindValue(id());

    if (!query.exec()) {
        Aki::SqlNetworkPrivate::checkError(query.lastError());
        return false;
    }

    return true;
}

#include "utils/sqlnetwork.moc"
