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

Server::Server()
{
}

Server::Server(const QString &name)
    : m_name(name)
{
}

QStringList
Server::addressList() const
{
    return m_addressList;
}

QStringList
Server::channelList() const
{
    return m_channelList;
}

QString
Server::encoding() const
{
    return m_encoding;
}

bool
Server::isAutoIdentifyEnabled() const
{
    return m_isAutoIdentifyEnabled;
}

bool
Server::isAutoJoinChannelsEnabled() const
{
    return m_isAutoJoinChannelsEnabled;
}

bool
Server::isAutoReconnectionEnabled() const
{
    return m_isAutoReconnectionEnabled;
}

bool
Server::isConnectOnStartupEnabled() const
{
    return m_isConnectOnStartupEnabled;
}

bool
Server::isConnectToRandomServerEnabled() const
{
    return m_isConnectToRandomServerEnabled;
}

bool
Server::isDefaultEncodingEnabled() const
{
    return m_isDefaultEncodingEnabled;
}

bool
Server::isSslEnabled() const
{
    return m_isSslEnabled;
}

QString
Server::name() const
{
    return m_name;
}

int
Server::retryAttemptCount() const
{
    return m_retryAttemptCount;
}

int
Server::retryInterval() const
{
    return m_retryInterval;
}

QString
Server::serviceName() const
{
    return m_serviceName;
}

QString
Server::servicePassword() const
{
    return m_servicePassword;
}

void
Server::setAddressList(const QStringList &addresses)
{
    m_addressList = addresses;
}

void
Server::setAutoIdentify(bool enable)
{
    m_isAutoIdentifyEnabled = enable;
}

void
Server::setAutoJoinChannels(bool enable)
{
    m_isAutoJoinChannelsEnabled = enable;
}

void
Server::setAutoReconnection(bool enable)
{
    m_isAutoReconnectionEnabled = enable;
}

void
Server::setChannelList(const QStringList &channels)
{
    m_channelList = channels;
}

void
Server::setConnectOnStartup(bool enable)
{
    m_isConnectOnStartupEnabled = enable;
}

void
Server::setConnectToRandomServer(bool enable)
{
    m_isConnectToRandomServerEnabled = enable;
}

void
Server::setDefaultEncoding(bool enable)
{
    m_isDefaultEncodingEnabled = enable;
}

void
Server::setEncoding(const QString &encodingName)
{
    m_encoding = encodingName;
}

void
Server::setName(const QString &name)
{
    m_name = name;
}
void
Server::setRetryAttempts(int count)
{
    m_retryAttemptCount = count;
}

void
Server::setRetryInterval(int seconds)
{
    m_retryInterval = seconds;
}

void
Server::setServiceName(const QString &name)
{
    m_serviceName = name;
}

void
Server::setServicePassword(const QString &password)
{
    m_servicePassword = password;
}

void
Server::setSsl(bool enable)
{
    m_isSslEnabled = enable;
}
