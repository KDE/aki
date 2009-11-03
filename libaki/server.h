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

#ifndef SERVER_H
#define SERVER_H

#include "libaki_export.h"
#include <QScopedPointer>
#include <QSharedPointer>
#include <QStringList>

namespace Aki
{
/**
 * Server class to hold information about the server. Not to be
 * confused with ServerConfig.
 */
class ServerPrivate;
class LIBAKI_EXPORT Server
{
public:
    /**
     * Creates a server.
     */
    Server();
    /**
     * Creates a server with a name @p name.
     * @param name Server name.
     */
    explicit Server(const QString &name);
    /**
     * Destroys the object.
     */
    ~Server();
    /*! @copydoc ServerConfig::addressList() */
    QStringList addressList() const;
    /*! @copydoc ServerConfig::channelList() */
    QStringList channelList() const;
    /*! @copydoc ServerConfig::encoding() */
    QString encoding() const;
    /*! @copydoc ServerConfig::isAutoIdentifyEnabled() */
    bool isAutoIdentifyEnabled() const;
    /*! @copydoc ServerConfig::isAutoJoinChannelsEnabled() */
    bool isAutoJoinChannelsEnabled() const;
    /*! @copydoc ServerConfig::isAutoReconnectionEnabled() */
    bool isAutoReconnectionEnabled() const;
    /*! @copydoc ServerConfig::isConnectOnStartupEnabled() */
    bool isConnectOnStartupEnabled() const;
    /*! @copydoc ServerConfig::isConnectToRandomServerEnabled() */
    bool isConnectToRandomServerEnabled() const;
    /*! @copydoc ServerConfig::isDefaultEncodingEnabled() */
    bool isDefaultEncodingEnabled() const;
    /*! @copydoc ServerConfig::isSslEnabled() */
    bool isSslEnabled() const;
    /**
     * Gets the name of the Server
     * @return Server name.
     */
    QString name() const;
    /*! @copydoc ServerConfig::retryAttemptCount() */
    int retryAttemptCount() const;
    /*! @copydoc ServerConfig::retryInterval() */
    int retryInterval() const;
    /*! @copydoc ServerConfig::serviceName() */
    QString serviceName() const;
    /*! @copydoc ServerConfig::servicePassword() */
    QString servicePassword() const;
    /*! @copydoc ServerConfig::setAddressList() */
    void setAddressList(const QStringList &addresses);
    /*! @copydoc ServerConfig::setAutoIdentify() */
    void setAutoIdentify(bool enable);
    /*! @copydoc ServerConfig::setAutoJoinChannels() */
    void setAutoJoinChannels(bool enable);
    /*! @copydoc ServerConfig::setAutoReconnection() */
    void setAutoReconnection(bool enable);
    /*! @copydoc ServerConfig::setChannelList() */
    void setChannelList(const QStringList &channels);
    /*! @copydoc ServerConfig::setConnectOnStartup() */
    void setConnectOnStartup(bool enable);
    /*! @copydoc ServerConfig::setConnectToRandomServer() */
    void setConnectToRandomServer(bool enable);
    /*! @copydoc ServerConfig::setDefaultEncoding() */
    void setDefaultEncoding(bool enable);
    /*! @copydoc ServerConfig::setEncoding() */
    void setEncoding(const QString &encodingName);
    /**
     * Sets the name @p name of the Server.
     * @param name Server name.
     */
    void setName(const QString &name);
    /*! @copydoc ServerConfig::setRetryAttempts() */
    void setRetryAttempts(int count);
    /*! @copydoc ServerConfig::setRetryInterval() */
    void setRetryInterval(int seconds);
    /*! @copydoc ServerConfig::setServiceName() */
    void setServiceName(const QString &name);
    /*! @copydoc ServerConfig::setServicePassword() */
    void setServicePassword(const QString &password);
    /*! @copydoc ServerConfig::setSsl() */
    void setSsl(bool enable);
private:
    friend class ServerPrivate;
    QScopedPointer<ServerPrivate> d;
}; // End of class Server.
typedef QSharedPointer<Server> ServerPointer;
typedef QList<ServerPointer> ServerList;
} // End of namespace Aki.

#endif // SERVER_H
