#include "server.hpp"
#include "private/server_p.hpp"
#include <QtCore/QSharedPointer>
using namespace Aki;

Server::Server()
{
    _d.reset(new Aki::ServerPrivate);
}

Server::Server(const QString& name)
{
    _d.reset(new Aki::ServerPrivate);
    setName(name);
}

Server::~Server()
{
}

Aki::AddressList
Server::addressList() const
{
    return _d->addressList;
}

Aki::ChannelList
Server::channelList() const
{
    return _d->channelList;
}

QByteArray
Server::encoding() const
{
    return _d->encoding;
}

bool
Server::isAutoIdentifyEnabled() const
{
    return _d->isAutoIdentifyEnabled;
}

bool
Server::isAutoJoinChannelsEnabled() const
{
    return _d->isAutoJoinChannelsEnabled;
}

bool
Server::isAutoReconnectionEnabled() const
{
    return _d->isAutoReconnectionEnabled;
}

bool
Server::isConnectOnStartupEnabled() const
{
    return _d->isConnectOnStartupEnabled;
}

bool
Server::isConnectToRandomServerEnabled() const
{
    return _d->isConnectToRandomServerEnabled;
}

bool
Server::isDefaultEncodingEnabled() const
{
    return _d->isDefaultEncodingEnabled;
}

bool
Server::isSslEnabled() const
{
    return _d->isSslEnabled;
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
Server::setAddressList(const Aki::AddressList& addresses)
{
    _d->addressList = addresses;
}

void
Server::setAutoIdentify(bool enabled)
{
    _d->isAutoIdentifyEnabled = enabled;
}

void
Server::setAutoJoinChannels(bool enabled)
{
    _d->isAutoJoinChannelsEnabled = enabled;
}

void
Server::setAutoReconnection(bool enabled)
{
    _d->isAutoReconnectionEnabled = enabled;
}

void
Server::setChannelList(const Aki::ChannelList& channels)
{
    _d->channelList = channels;
}

void
Server::setConnectOnStartup(bool enabled)
{
    _d->isConnectOnStartupEnabled = enabled;
}

void
Server::setConnectToRandomServer(bool enabled)
{
    _d->isConnectToRandomServerEnabled = enabled;
}

void
Server::setDefaultEncoding(bool enabled)
{
    _d->isDefaultEncodingEnabled = enabled;
}

void
Server::setEncoding(const QByteArray& encoding)
{
    _d->encoding = encoding;
}

void
Server::setName(const QString& name)
{
    _d->name = name;
}

void
Server::setRetryAttempts(int count)
{
    _d->retryAttemptCount = count;
}

void
Server::setRetryInterval(int seconds)
{
    _d->retryInterval = seconds;
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

void
Server::setSsl(bool enabled)
{
    _d->isSslEnabled = enabled;
}
