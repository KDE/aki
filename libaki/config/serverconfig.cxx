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

#include "serverconfig.hpp"
#include <QtCore/QTextCodec>
using namespace Aki;

ServerConfig::ServerConfig(QObject* parent)
    : ConfigFile("akiserversrc", parent)
{
}

ServerConfig::~ServerConfig()
{
}

QStringList
ServerConfig::addressList()
{
    QString tmp = currentGroup().readEntry("AddressList", QString());
    if (tmp.isEmpty() || tmp.isNull()) {
        return QStringList();
    }
    return tmp.split(QLatin1Char(','), QString::SkipEmptyParts);
}

QStringList
ServerConfig::channelList()
{
    QString tmp = currentGroup().readEntry("ChannelList", QString());
    if (tmp.isEmpty() || tmp.isNull()) {
        return tmp.split(QLatin1Char(','), QString::SkipEmptyParts);
    }
    return QStringList();
}

QByteArray
ServerConfig::encoding()
{
    QTextCodec* codec = 0;
    if (!(codec = QTextCodec::codecForName("UTF-8"))) {
        if (!(codec = QTextCodec::codecForName("ISO-8859-15"))) {
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
ServerConfig::isAutoReconnectEnabled()
{
    return currentGroup().readEntry("AutoReconnectEnabled", true);
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
    return currentGroup().readEntry("ServiceName");
}

QString
ServerConfig::servicePassword()
{
    return currentGroup().readEntry("ServicePassword");
}

void
ServerConfig::setAddressList(const QStringList& addresses)
{
    QString addressList = addresses.join(",");
    currentGroup().writeEntry("AddressList", addressList);
}

void
ServerConfig::setAutoIdentify(bool enabled)
{
    currentGroup().writeEntry("AutoIdentifyEnabled", enabled);
}

void
ServerConfig::setAutoJoinChannels(bool enabled)
{
    currentGroup().writeEntry("AutoJoinChannelsEnabled", enabled);
}

void
ServerConfig::setAutoReconnect(bool enabled)
{
    currentGroup().writeEntry("AutoReconnectEnabled", enabled);
}

void
ServerConfig::setChannelList(const QStringList& channels)
{
    QString channelList = channels.join(",");
    currentGroup().writeEntry("ChannelList", channelList);
}

void
ServerConfig::setConnectOnStartup(bool enabled)
{
    currentGroup().writeEntry("ConnectOnStartupEnabled", enabled);
}

void
ServerConfig::setConnectToRandomServer(bool enabled)
{
    currentGroup().writeEntry("ConnectToRandomServerEnabled", enabled);
}

void
ServerConfig::setDefaultEncoding(bool enabled)
{
    currentGroup().writeEntry("DefaultEncodingEnabled", enabled);
}

void
ServerConfig::setEncoding(const QByteArray& encoding)
{
    currentGroup().writeEntry("DefaultEncoding", encoding);
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
ServerConfig::setServiceName(const QString& name)
{
    currentGroup().writeEntry("ServiceName", name);
}

void
ServerConfig::setServicePassword(const QString& password)
{
    currentGroup().writeEntry("ServicePassword", password);
}

void
ServerConfig::setSsl(bool enabled)
{
    currentGroup().writeEntry("SslEnabled", enabled);
}

#include "config/serverconfig.moc"
