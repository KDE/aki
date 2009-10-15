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

#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include "libaki_export.h"
#include "config/configfile.h"

namespace Aki
{
/**
 * Configuration class for the IRC Identity. Able to read and write to the
 * akiserversrc file that stores the identity settings.
 */
class LIBAKI_EXPORT ServerConfig : public ConfigFile
{
    Q_OBJECT
public:
    /**
     * Creates the server configuration.
     * @param parent Parent of the object.
     */
    ServerConfig(QObject *parent = 0);
    /**
     * Destroys the object.
     */
    ~ServerConfig();
    /**
     * Gets a list of service addresses to use for the
     * server.
     * @return Server address list.
     * @sa setAddressList()
     */
    QStringList addressList();
    /**
     * Gets a list of channels to use for joining on
     * start up.
     * @return Channel list.
     * @sa setChannelList()
     */
    QStringList channelList();
    /**
     * Gets the name of the encoding e.g. UTF-8.
     * @return Encoding name.
     * @sa setEncoding()
     */
    QString encoding();
    /**
     * Gets the state of the server if it will identify
     * on connection.
     * @return true if it will identify; false otherwise.
     * @sa setAutoIdentify()
     */
    bool isAutoIdentifyEnabled();
    /**
     * Gets the state of the server if it will automatically
     * join the predefine channels on start up.
     * @return true if it will join the channels; false otherwise.
     * @sa setChannelList()
     * @sa setAutoJoinChannels()
     */
    bool isAutoJoinChannelsEnabled();
    /**
     * Gets the state of the server if it will automatically
     * reconnect to the socket when disconnected.
     * @return true if it will reconnect; false otherwise.
     * @sa setAutoReconnect()
     */
    bool isAutoReconnectionEnabled();
    /**
     * Gets the state of the server if it will automatically
     * connect on the start up of the server.
     * @return true if it will connect on start up; false otherwise.
     * @sa setConnectOnStartup()
     */
    bool isConnectOnStartupEnabled();
    /**
     * Gets the state of the server if it will automatically
     * connect to a random server in the server list.
     * @return true if it connect to random server; false otherwise.
     * @sa setConnectToRandomServer()
     */
    bool isConnectToRandomServerEnabled();
    /**
     * Gets the state of the server if it will use the default
     * encoding.
     * @return true if it will use the default encoding; false
     * it will use the user defined encoding.
     * @sa setDefaultEncoding()
     */
    bool isDefaultEncodingEnabled();
    /**
     * Gets the state of if the server is using SSL.
     * @return true if using SSL; false otherwise.
     * @sa setSsl()
     */
    bool isSslEnabled();
    /**
     * Gets the number of retry attempts that will be
     * attempted before no more reconnections will be
     * made.
     * @return Number of retry attempts.
     */
    int retryAttemptCount();
    /**
     * Gets the retry interval in seconds how much time
     * must pass before another reconnection will be made.
     * @return Number of seconds.
     */
    int retryInterval();
    /**
     * Gets the service name.
     * @return Service name.
     * @sa setServiceName()
     */
    QString serviceName();
    /**
     * Gets the service password.
     * @return Service password.
     * @sa setServicePassword()
     */
    QString servicePassword();
    /**
     * Sets the list of addresses @p addresses the server will use to
     * connect.
     * @param addresses Address list.
     */
    void setAddressList(const QStringList &addresses);
    /**
     * Sets the state @p enable of the server to identify once
     * it is connected.
     * @param enable true to identify; false otherwise.
     */
    void setAutoIdentify(bool enable);
    /**
     * Sets the state @p enable of the server to automatically join
     * the channels upon start up.
     * @param enable true to join channels; false otherwise.
     */
    void setAutoJoinChannels(bool enable);
    /**
     * Sets the state @p enable to reconnect to the server upon disconnection.
     * @param enable true to reconnect; false otherwise.
     */
    void setAutoReconnection(bool enable);
    /**
     * Sets the channel list @p channels that will be used when the server has
     * autoJoinChannels() set to true.
     * @param channels Channel list.
     */
    void setChannelList(const QStringList &channels);
    /**
     * Sets the state @p enable of the server to connect on start up.
     * @param enable true to connect on startup; false otherwise.
     */
    void setConnectOnStartup(bool enable);
    /**
     * Sets the state @p enable of the server to connect to a random server
     * in the address list or follow in a linear order.
     * @param enable true to connect to a random server; false otherwise.
     */
    void setConnectToRandomServer(bool enable);
    /**
     * Sets the state @p enable of the server to use the default encoding
     * or use the user defined encoding.
     * @param enable true to use the default encoding; false to use
     * user defined.
     */
    void setDefaultEncoding(bool enable);
    /**
     * Sets the encoding name @p encodingName of the server to use. This is the user
     * defined encoding name.
     * @param encodingName Name of the encoding type.
     */
    void setEncoding(const QString &encodingName);
    /**
     * Sets the number @p count of how many attempts will be made before the server
     * will give up trying to connect.
     * @param count Number of times to attempt a reconnection.
     */
    void setRetryAttempts(int count);
    /**
     * Sets the retry interval @p seconds in seconds. Determines
     * how long between each attempt will be made.
     * @param seconds Interval in seconds.
     * @sa retryInterval()
     */
    void setRetryInterval(int seconds);
    /**
     * Sets the service name @p name
     * @param name Service name.
     * @sa serviceName()
     */
    void setServiceName(const QString &name);
    /**
     * Sets the service password @p password.
     * @param password Service password.
     * @sa servicePassword()
     */
    void setServicePassword(const QString &password);
    /**
     * Sets the state @p enable of if the server uses
     * SSL to connect.
     * @param enable true if using SSL; false otherwise.
     * @sa isSslEnabled()
     */
    void setSsl(bool enable);
};
} // End of namespace Aki.

#endif // SERVERCONFIG_H
