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

#ifndef AKI_SERVERCONFIG_HPP
#define AKI_SERVERCONFIG_HPP

#include "aki.hpp"
#include "configfile.hpp"

namespace Aki
{
class LIBAKI_EXPORT ServerConfig
    : public Aki::ConfigFile
{
    Q_OBJECT
public:
    explicit ServerConfig(QObject* parent = 0);
    ~ServerConfig();
    QStringList addressList();
    QStringList channelList();
    QByteArray encoding();
    bool isAutoIdentifyEnabled();
    bool isAutoJoinChannelsEnabled();
    bool isAutoReconnectEnabled();
    bool isConnectOnStartupEnabled();
    bool isConnectToRandomServerEnabled();
    bool isDefaultEncodingEnabled();
    bool isSslEnabled();
    int retryAttemptCount();
    int retryInterval();
    QString serviceName();
    QString servicePassword();
    void setAddressList(const QStringList& addresses);
    void setAutoIdentify(bool enabled);
    void setAutoJoinChannels(bool enabled);
    void setAutoReconnect(bool enabled);
    void setChannelList(const QStringList& channels);
    void setConnectOnStartup(bool enabled);
    void setConnectToRandomServer(bool enabled);
    void setDefaultEncoding(bool enabled);
    void setEncoding(const QByteArray& encoding);
    void setRetryAttempts(int count);
    void setRetryInterval(int seconds);
    void setServiceName(const QString& name);
    void setServicePassword(const QString& password);
    void setSsl(bool enabled);
}; // End of class ServerConfig.
} // End of namespace Aki.

#endif // AKI_SERVERCONFIG_HPP
