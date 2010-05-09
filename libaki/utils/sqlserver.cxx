#include "sqlserver.hpp"
#include "aki.hpp"
#include "utils/sqlidentity.hpp"
#include "private/sqlserver_p.hpp"
#include <QtCore/QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
using namespace Aki;

SqlServer::SqlServer(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::SqlServerPrivate(this));
    _d->server = Aki::Server::Ptr(new Aki::Server);
}

SqlServer::~SqlServer()
{
}

bool
SqlServer::operator!=(const Aki::SqlServer& rhs) const
{
    return !(*this == rhs);
}

bool
SqlServer::operator==(const Aki::SqlServer& rhs) const
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

Aki::SqlServer::List
SqlServer::serversForIdentity(const Aki::SqlIdentity* identity)
{
    SqlServer::List list;
    QSqlQuery query;
    query.prepare(QLatin1String("SELECT * FROM Server WHERE serverIdentity = ?"));
    query.addBindValue(identity->id());

    if (!query.exec()) {
        Aki::SqlServerPrivate::checkError(query.lastError());
        return list;
    }

    if (query.size() == 0) {
        return list;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf(QLatin1String("id"));
    const int encoding = record.indexOf(QLatin1String("encoding"));
    const int autoIdentify = record.indexOf(QLatin1String("autoIdentify"));
    const int autoJoinChannels = record.indexOf(QLatin1String("autoJoinChannels"));
    const int autoReconnect = record.indexOf(QLatin1String("autoReconnect"));
    const int connectOnStartup = record.indexOf(QLatin1String("connectOnStartup"));
    const int connectToRandomServer = record.indexOf(QLatin1String("connectToRandomServer"));
    const int defaultEncoding = record.indexOf(QLatin1String("defaultEncoding"));
    const int name = record.indexOf(QLatin1String("name"));
    const int retryAttempts = record.indexOf(QLatin1String("retryAttempts"));
    const int retryInterval = record.indexOf(QLatin1String("retryInterval"));
    const int serviceName = record.indexOf(QLatin1String("serviceName"));
    const int servicePassword = record.indexOf(QLatin1String("servicePassword"));
    const int serverIdentity = record.indexOf(QLatin1String("serverIdentity"));

    while (query.next()) {
        Aki::SqlServer* server = new Aki::SqlServer;
        server->setId(query.value(id).toInt());
        server->setEncoding(query.value(encoding).toByteArray());
        server->setAutoIdentity(query.value(autoIdentify).toBool());
        server->setAutoJoinChannels(query.value(autoJoinChannels).toBool());
        server->setAutoReconnect(query.value(autoReconnect).toBool());
        server->setConnectOnStartup(query.value(connectOnStartup).toBool());
        server->setConnectToRandomServer(query.value(connectToRandomServer).toBool());
        server->setDefaultEncoding(query.value(defaultEncoding).toBool());
        server->setName(query.value(name).toString());
        server->setRetryAttempts(query.value(retryAttempts).toBool());
        server->setRetryInterval(query.value(retryInterval).toBool());
        server->setServiceName(query.value(serviceName).toString());
        server->setServicePassword(query.value(servicePassword).toString());
        server->setServerIdentityId(query.value(serverIdentity).toInt());
        list.append(server);
    }

    return list;
}

Aki::SqlServer*
SqlServer::newServer(const QString& name, const Aki::SqlIdentity* identity)
{
    if (name.isEmpty() || name.isNull()) {
        return 0;
    }

    QSqlQuery query;
    query.prepare(QLatin1String("INSERT INTO Server (encoding,autoIdentify,autoJoinChannels,autoReconnect,\n\
                                 connectOnStartup,connectToRandomServer,defaultEncoding,name,retryAttempts,retryInterval,\n\
                                 serviceName,servicePassword,serverIdentity)\n\
                                 VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)"));

    QScopedPointer<Aki::SqlServer> server(new Aki::SqlServer);
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

    query.addBindValue(QString(QLatin1String(server->encoding())));
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
        Aki::SqlServerPrivate::checkError(query.lastError());
        return 0;
    }

    server->setId(query.lastInsertId().toInt());

    return server.take();
}

QByteArray
SqlServer::encoding() const
{
    return _d->server->encoding();
}

Aki::SqlServer*
SqlServer::findServer(const QString& name, const Aki::SqlIdentity* identity)
{
    if (name.isEmpty() || name.isNull()) {
        return 0;
    }

    QSqlQuery query;
    query.prepare(QLatin1String("SELECT * FROM Server WHERE name=? AND serverIdentity=? LIMIT 1"));
    query.addBindValue(name);
    query.addBindValue(identity->id());

    if (!query.exec()) {
        Aki::SqlServerPrivate::checkError(query.lastError());
        return 0;
    }

    if (query.size() == 0) {
        return 0;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf(QLatin1String("id"));
    const int encoding = record.indexOf(QLatin1String("encoding"));
    const int autoIdentify = record.indexOf(QLatin1String("autoIdentify"));
    const int autoJoinChannels = record.indexOf(QLatin1String("autoJoinChannels"));
    const int autoReconnect = record.indexOf(QLatin1String("autoReconnect"));
    const int connectOnStartup = record.indexOf(QLatin1String("connectOnStartup"));
    const int connectToRandomServer = record.indexOf(QLatin1String("connectToRandomServer"));
    const int defaultEncoding = record.indexOf(QLatin1String("defaultEncoding"));
    const int nameIndex = record.indexOf(QLatin1String("name"));
    const int retryAttempts = record.indexOf(QLatin1String("retryAttempts"));
    const int retryInterval = record.indexOf(QLatin1String("retryInterval"));
    const int serviceName = record.indexOf(QLatin1String("serviceName"));
    const int servicePassword = record.indexOf(QLatin1String("servicePassword"));
    const int serverIdentity = record.indexOf(QLatin1String("serverIdentity"));

    Aki::SqlServer* server = new Aki::SqlServer;
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
SqlServer::isAutoIdentifyEnabled() const
{
    return _d->server->isAutoIdentifyEnabled();
}

bool
SqlServer::isAutoJoinChannelsEnabled() const
{
    return _d->server->isAutoJoinChannelsEnabled();
}

bool
SqlServer::isAutoReconnectEnabled() const
{
    return _d->server->isAutoReconnectionEnabled();
}

bool
SqlServer::isConnectOnStartupEnabled() const
{
    return _d->server->isConnectOnStartupEnabled();
}

bool SqlServer::isConnectToRandomServerEnabled() const
{
    return _d->server->isConnectToRandomServerEnabled();
}

bool
SqlServer::isDefaultEncodingEnabled() const
{
    return _d->server->isDefaultEncodingEnabled();
}

QString
SqlServer::name() const
{
    return _d->server->name();
}

int
SqlServer::retryAttemptCount() const
{
    return _d->server->retryAttemptCount();
}

int
SqlServer::retryInterval() const
{
    return _d->server->retryInterval();
}

QString
SqlServer::serviceName() const
{
    return _d->server->serviceName();
}

QString
SqlServer::servicePassword() const
{
    return _d->server->servicePassword();
}

void
SqlServer::setAutoIdentity(bool enabled)
{
    _d->server->setAutoIdentify(enabled);
}

void
SqlServer::setAutoJoinChannels(bool enabled)
{
    _d->server->setAutoJoinChannels(enabled);
}

void
SqlServer::setAutoReconnect(bool enabled)
{
    _d->server->setAutoReconnection(enabled);
}

void
SqlServer::setConnectOnStartup(bool enabled)
{
    _d->server->setConnectOnStartup(enabled);
}

void
SqlServer::setConnectToRandomServer(bool enabled)
{
    _d->server->setConnectToRandomServer(enabled);
}

void
SqlServer::setDefaultEncoding(bool enabled)
{
    _d->server->setDefaultEncoding(enabled);
}

void
SqlServer::setEncoding(const QByteArray& encoding)
{
    _d->server->setEncoding(encoding);
}

void
SqlServer::setId(int id)
{
    _d->id = id;
}

void
SqlServer::setName(const QString& name)
{
    _d->server->setName(name);
}

void
SqlServer::setRetryAttempts(int count)
{
    _d->server->setRetryAttempts(count);
}

void
SqlServer::setRetryInterval(int seconds)
{
    _d->server->setRetryInterval(seconds);
}

void
SqlServer::setServiceName(const QString& name)
{
    _d->server->setServiceName(name);
}

void
SqlServer::setServicePassword(const QString& password)
{
    _d->server->setServicePassword(password);
}

int
SqlServer::id() const
{
    return _d->id;
}

int
SqlServer::serverIdentityId() const
{
    return _d->serverId;
}

void
SqlServer::setServerIdentityId(int id)
{
    _d->serverId = id;
}

bool
SqlServer::save()
{
    QSqlQuery query;
    const QString str = QLatin1String("UPDATE Server SET encoding=?, autoIdentify=?, autoJoinChannels=?,"
                                      " autoReconnect=?, connectOnStartup=?, connectToRandomServer=?, defaultEncoding=?,"
                                      " name=?, retryAttempts=?, retryInterval=?, serviceName=?, servicePassword=?,"
                                      " serverIdentity=? WHERE id=?");
    query.prepare(str);
    query.addBindValue(QLatin1String(encoding()));
    query.addBindValue(isAutoIdentifyEnabled());
    query.addBindValue(isAutoJoinChannelsEnabled());
    query.addBindValue(isAutoReconnectEnabled());
    query.addBindValue(isConnectOnStartupEnabled());
    query.addBindValue(isConnectToRandomServerEnabled());
    query.addBindValue(isDefaultEncodingEnabled());
    query.addBindValue(name());
    query.addBindValue(retryAttemptCount());
    query.addBindValue(retryInterval());
    query.addBindValue(Aki::SqlServerPrivate::checkString(serviceName()));
    query.addBindValue(Aki::SqlServerPrivate::checkString(servicePassword()));
    query.addBindValue(serverIdentityId());
    query.addBindValue(id());

    if (!query.exec()) {
        Aki::SqlServerPrivate::checkError(query.lastError());
        return false;
    }

    return true;
}

bool
SqlServer::remove()
{
    QSqlQuery query;
    const QString str = QLatin1String("DELETE FROM Server WHERE name=? AND id=?");
    query.prepare(str);
    query.addBindValue(name());
    query.addBindValue(id());

    if (!query.exec()) {
        Aki::SqlServerPrivate::checkError(query.lastError());
        return false;
    }

    return true;
}
