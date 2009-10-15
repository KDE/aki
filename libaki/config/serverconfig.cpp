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

#include "serverconfig.h"
#include <KConfigGroup>
#include <QTextCodec>
using namespace Aki;

ServerConfig::ServerConfig(QObject *parent)
    : Aki::ConfigFile("akiserversrc", parent)
{
}

ServerConfig::~ServerConfig()
{
}

QStringList
ServerConfig::addressList()
{
    
    QString tmp = currentGroup().readEntry("AddressList", "chat.freenode.net/6667");
    return tmp.split(QChar(','), QString::SkipEmptyParts);
}

QStringList
ServerConfig::channelList()
{
    QString tmp = currentGroup().readEntry("ChannelList", "#aki");
    return tmp.split(QChar(','), QString::SkipEmptyParts);
}

QString
ServerConfig::encoding()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    if (!codec) {
        codec = QTextCodec::codecForName("ISO-8859-15");
        if (!codec) {
            codec = QTextCodec::codecForName("ISO-8859-1");
        }
    }

    return currentGroup().readEntry("DefaultEncoding", codec->name());
}

bool
ServerConfig::isAutoIdentifyEnabled()
{
    return currentGroup().readEntry("AutoIdentifyEnabled", false);
}

bool
ServerConfig::isAutoJoinChannelsEnabled()
{
    return currentGroup().readEntry("AutoJoinChannelsEnabled", true);
}

bool
ServerConfig::isAutoReconnectionEnabled()
{
    return currentGroup().readEntry("AutoReconnectionEnabled", true);
}

bool
ServerConfig::isConnectOnStartupEnabled()
{
    return currentGroup().readEntry("ConnectOnStartupEnabled", true);
}

bool
ServerConfig::isConnectToRandomServerEnabled()
{
    return currentGroup().readEntry("ConnectToRandomServerEnabled", false);
}

bool
ServerConfig::isDefaultEncodingEnabled()
{
    return currentGroup().readEntry("DefaultEncodingEnabled", true);
}

bool
ServerConfig::isSslEnabled()
{
    return currentGroup().readEntry("SslEnabled", false);
}

int
ServerConfig::retryAttemptCount()
{
    return currentGroup().readEntry("RetryAttemptCount", 10);
}

int
ServerConfig::retryInterval()
{
    return currentGroup().readEntry("RetryInterval", 10);
}

QString
ServerConfig::serviceName()
{
    return currentGroup().readEntry("ServiceName", "NickServ");
}

QString
ServerConfig::servicePassword()
{
    return currentGroup().readEntry("ServicePassword");
}

void
ServerConfig::setAddressList(const QStringList &addresses)
{
    QString tmp = addresses.join(QChar(','));
    currentGroup().writeEntry("AddressList", tmp);
}

void
ServerConfig::setAutoIdentify(bool enable)
{
    currentGroup().writeEntry("AutoIdentifyEnabled", enable);
}

void
ServerConfig::setAutoJoinChannels(bool enable)
{
    currentGroup().writeEntry("AutoJoinChannelsEnabled", enable);
}

void
ServerConfig::setAutoReconnection(bool enable)
{
    currentGroup().writeEntry("AutoReconnectionEnabled", enable);
}

void
ServerConfig::setChannelList(const QStringList &channels)
{
    QString channelList = channels.join(QChar(','));
    currentGroup().writeEntry("ChannelList", channelList);
}

void
ServerConfig::setConnectOnStartup(bool enable)
{
    currentGroup().writeEntry("ConnectOnStartupEnabled", enable);
}

void
ServerConfig::setConnectToRandomServer(bool enable)
{
    currentGroup().writeEntry("ConnectToRandomServerEnabled", enable);
}

void
ServerConfig::setDefaultEncoding(bool enable)
{
    currentGroup().writeEntry("DefaultEncodingEnabled", enable);
}

void
ServerConfig::setEncoding(const QString &encodingName)
{
    currentGroup().writeEntry("DefaultEncoding", encodingName);
}

void
ServerConfig::setRetryAttempts(int count)
{
    currentGroup().writeEntry("RetryAttemptCount", count);
}

void
ServerConfig::setRetryInterval(int seconds)
{
    currentGroup().writeEntry("RetryInterval", seconds);
}

void
ServerConfig::setServiceName(const QString &name)
{
    currentGroup().writeEntry("ServiceName", name);
}

void
ServerConfig::setServicePassword(const QString &password)
{
    currentGroup().writeEntry("ServicePassword", password);
}

void
ServerConfig::setSsl(bool enable)
{
    currentGroup().writeEntry("SslEnabled", enable);
}
