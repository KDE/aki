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

#include "server.h"
using namespace Aki;

namespace Aki
{
class ServerPrivate
{
public:
    ServerPrivate()
        : encoding(QString()),
        name(QString()),
        serviceName(QString()),
        servicePassword(QString()),
        isAutoIdentifyEnabled(false),
        isAutoJoinChannelsEnabled(false),
        isAutoReconnectionEnabled(false),
        isConnectOnStartupEnabled(false),
        isConnectToRandomServerEnabled(false),
        isDefaultEncodingEnabled(false),
        isSslEnabled(false),
        retryAttemptCount(0),
        retryInterval(0)
    {
        addressList.clear();
        channelList.clear();
    }

    QStringList addressList;
    QStringList channelList;
    QString encoding;
    QString name;
    QString serviceName;
    QString servicePassword;
    bool isAutoIdentifyEnabled;
    bool isAutoJoinChannelsEnabled;
    bool isAutoReconnectionEnabled;
    bool isConnectOnStartupEnabled;
    bool isConnectToRandomServerEnabled;
    bool isDefaultEncodingEnabled;
    bool isSslEnabled;
    int retryAttemptCount;
    int retryInterval;
}; // End of class ServerPrivate.
} // End of namespace Aki.

Server::Server()
{
    d.reset(new Aki::ServerPrivate);
}

Server::Server(const QString &name)
{
    d.reset(new Aki::ServerPrivate);
    d->name = name;
}

Server::~Server()
{
}

QStringList
Server::addressList() const
{
    return d->addressList;
}

QStringList
Server::channelList() const
{
    return d->channelList;
}

QString
Server::encoding() const
{
    return d->encoding;
}

bool
Server::isAutoIdentifyEnabled() const
{
    return d->isAutoIdentifyEnabled;
}

bool
Server::isAutoJoinChannelsEnabled() const
{
    return d->isAutoJoinChannelsEnabled;
}

bool
Server::isAutoReconnectionEnabled() const
{
    return d->isAutoReconnectionEnabled;
}

bool
Server::isConnectOnStartupEnabled() const
{
    return d->isConnectOnStartupEnabled;
}

bool
Server::isConnectToRandomServerEnabled() const
{
    return d->isConnectToRandomServerEnabled;
}

bool
Server::isDefaultEncodingEnabled() const
{
    return d->isDefaultEncodingEnabled;
}

bool
Server::isSslEnabled() const
{
    return d->isSslEnabled;
}

QString
Server::name() const
{
    return d->name;
}

int
Server::retryAttemptCount() const
{
    return d->retryAttemptCount;
}

int
Server::retryInterval() const
{
    return d->retryInterval;
}

QString
Server::serviceName() const
{
    return d->serviceName;
}

QString
Server::servicePassword() const
{
    return d->servicePassword;
}

void
Server::setAddressList(const QStringList &addresses)
{
    d->addressList = addresses;
}

void
Server::setAutoIdentify(bool enable)
{
    d->isAutoIdentifyEnabled = enable;
}

void
Server::setAutoJoinChannels(bool enable)
{
    d->isAutoJoinChannelsEnabled = enable;
}

void
Server::setAutoReconnection(bool enable)
{
    d->isAutoReconnectionEnabled = enable;
}

void
Server::setChannelList(const QStringList &channels)
{
    d->channelList = channels;
}

void
Server::setConnectOnStartup(bool enable)
{
    d->isConnectOnStartupEnabled = enable;
}

void
Server::setConnectToRandomServer(bool enable)
{
    d->isConnectToRandomServerEnabled = enable;
}

void
Server::setDefaultEncoding(bool enable)
{
    d->isDefaultEncodingEnabled = enable;
}

void
Server::setEncoding(const QString &encodingName)
{
    d->encoding = encodingName;
}

void
Server::setName(const QString &name)
{
    d->name = name;
}
void
Server::setRetryAttempts(int count)
{
    d->retryAttemptCount = count;
}

void
Server::setRetryInterval(int seconds)
{
    d->retryInterval = seconds;
}

void
Server::setServiceName(const QString &name)
{
    d->serviceName = name;
}

void
Server::setServicePassword(const QString &password)
{
    d->servicePassword = password;
}

void
Server::setSsl(bool enable)
{
    d->isSslEnabled = enable;
}
