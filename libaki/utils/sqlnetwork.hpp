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

#ifndef AKI_SQLNETWORK_HPP
#define AKI_SQLNETWORK_HPP

#include "aki.hpp"
#include <QtCore/QMetaType>
#include <QtCore/QObject>

namespace Aki
{
class SqlIdentity;
class SqlNetworkPrivate;
/**
 * Encapsulates the Server table in the configuration database.
 */
class LIBAKI_EXPORT SqlNetwork
    : public QObject
{
    Q_OBJECT
public:
    /**
     * Typedef for QList<Aki::SqlNetwork*>
     */
    typedef QList<Aki::SqlNetwork*> List;
    /**
     * Destroys the object.
     */
    virtual ~SqlNetwork();
    /**
     * Compares two SqlNetwork's if they are the same based on their content.
     *
     * @param rhs Right hand side of the ==.
     *
     * @return true if the two SqlNetwork's contents are the same.
     */
    bool operator==(const Aki::SqlNetwork& rhs) const;
    /**
     * Compares two SqlNetwork's if they are not the same based on their content.
     *
     * @param rhs Right hand side of the !=.
     *
     * @return true if the two SqlNetwork's contents are different.
     */
    bool operator!=(const Aki::SqlNetwork& rhs) const;
    /**
     * Retrives all the servers that are connected to the @p identity.
     * @note You are responsible for the deletion of the pointers in the list.
     *
     * @param identity Sql Identity.
     *
     * @return List of SqlNetwork items if any are found. Empty list otherwise.
     */
    static Aki::SqlNetwork::List networksForIdentity(const Aki::SqlIdentity* identity);
    /**
     * Gets the newly created server with @p name and assigns it the @p identity table id.
     * @note You are responsible for the deletion of the pointer returned.
     *
     * @param name Name of the new SqlNetwork.
     * @param identity Identity the new SqlNetwork will be connected to.
     *
     * @return New create server.
     */
    static Aki::SqlNetwork* newNetwork(const QString& name, const Aki::SqlIdentity* identity);
    /**
     * Gets the server that has the @p name and the @p identity.
     * @note You are responsible for the deletion of the pointer returned.
     *
     * @param name Name of the server to look for.
     * @param identity Identity the server will look for in.
     *
     * @return Network that was found; 0 if the server was not found.
     */
    static Aki::SqlNetwork* findNetwork(const QString& name, const Aki::SqlIdentity* identity);
    /**
     * Sets the @p encoding for this server. Defaults to UTF-8.
     *
     * @param encoding Text encoding name.
     */
    void setEncoding(const QByteArray& encoding);
    /**
     * Gets the encoding of this server.
     *
     * @return Text encoding name.
     */
    QByteArray encoding() const;
    /**
     * Sets the @p enabled state for auto identity.
     *
     * @param enabled true if the server will auto identify
     */
    void setAutoIdentity(bool enabled);
    /**
     * Gets the state of the auto identify of this server.
     *
     * @return true if this server is auto identifying; false otherwise.
     */
    bool isAutoIdentifyEnabled() const;
    /**
     * Sets the @p enabled state for auto joining channels.
     *
     * @param enabled true if the server will auto join channels.
     */
    void setAutoJoinChannels(bool enabled);
    /**
     * Gets the state of the auto join channels of this server.
     *
     * @return true if this server is auto joining channels; false otherwise.
     */
    bool isAutoJoinChannelsEnabled() const;
    /**
     * Sets the @p enabled state for auto reconnecting.
     *
     * @param enabled true if the server will auto connect.
     */
    void setAutoReconnect(bool enabled);
    /**
     * Gets the state of the auto reconnecting of this server.
     *
     * @return true if this server will auto reconnect; false otherwise.
     */
    bool isAutoReconnectEnabled() const;
    /**
     * Sets the @p enabled state of the server if it will connect on startup.
     *
     * @param enabled true if the server will connect on startup.
     */
    void setConnectOnStartup(bool enabled);
    /**
     * Gets the state of if the server will connect on startup.
     *
     * @return true if the server will connect on startup.
     */
    bool isConnectOnStartupEnabled() const;
    /**
     * Sets the @p enabled state of the server to connect to random servers.
     *
     * @param enabled true if the server will connect to random servers.
     */
    void setConnectToRandomServer(bool enabled);
    /**
     * Gets the state of if the server will connect to random servers only
     * if the server count is more than 1.
     *
     * @return true if the server will connect to random servers.
     */
    bool isConnectToRandomServerEnabled() const;
    /**
     * Sets the @p enabled state for using the default encoding or customer for
     * this server.
     *
     * @param enabled true if the server will use the default encoding; false otherwise.
     */
    void setDefaultEncoding(bool enabled);
    /**
     * Gets the state of if the server will be using the default encoding.
     *
     * @return true if the server will be using the default encoding; false
     * if it will be using the custom one.
     *
     * @sa SqlNetwork::encoding()
     * @sa SqlNetwork::setEncoding()
     * @sa SqlNetwork::setDefaultEncoding()
     */
    bool isDefaultEncodingEnabled() const;
    /**
     * Sets the server @p name.
     *
     * @param name Name of the server.
     *
     * @sa SqlNetwork::name()
     */
    void setName(const QString& name);
    /**
     * Gets the name of the server.
     *
     * @return Name of the server.
     *
     * @sa SqlNetwork::setName()
     */
    QString name() const;
    /**
     * Sets the retry attempt @p count for this server. It will
     * attempts connecting until the count is reached. Or if the @p count
     * is 0 it will continue forever.
     *
     * @param count Retry attempt count. 0 for infinite.
     */
    void setRetryAttempts(int count);
    /**
     * Gets the retry attempt count for this server.
     *
     * @return Retry attempt count.
     */
    int retryAttemptCount() const;
    /**
     * Sets the retry interval of this server in @p seconds.
     *
     * @param seconds Retry interval in seconds
     */
    void setRetryInterval(int seconds);
    /**
     * Gets the retry interval of the server.
     *
     * @return Retry interval of the server in seconds.
     */
    int retryInterval() const;
    /**
     * Sets the service @p name for this server.
     *
     * @param name Name of the service.
     */
    void setServiceName(const QString& name);
    /**
     * Gets the service name of this server.
     *
     * @return Service name.
     */
    QString serviceName() const;
    /**
     * Sets the @p password for the service name.
     *
     * @param password Service password.
     */
    void setServicePassword(const QString& password);
    /**
     * Gets the service password of this server.
     *
     * @return Service password
     */
    QString servicePassword() const;
    /**
     * Gets the id of the row this server is in.
     *
     * @return Network row id.
     */
    int id() const;
    /**
     * Sets the @p id of the identity this server is connected with.
     *
     * @param id Id of the Identity this server will belong to.
     */
    void setServerIdentityId(int id);
    /**
     * Gets the id of the identity this server is connected with.
     *
     * @return ID of the Identity this server belongs to.
     */
    int serverIdentityId() const;
    /**
     * Saves any changes the user has made to the SQL database.
     *
     * @return true if it was successful; false otherwise.
     */
    bool save();
    /**
     * Removes the server from the current server from the list.
     *
     * @return true if the server was remvoed; false otherwise.
     */
    bool remove();
protected:
    /**
     * Creates a new SqlNetwork object.
     *
     * @param parent Parent of the object.
     */
    SqlNetwork(QObject* parent = 0);
    /**
     * Sets the @p id for this server.
     * @param id This should never be called. Internal use only.
     */
    void setId(int id);
private:
    AKI_DECLARE_PRIVATE(SqlNetwork)
}; // End of class SqlNetwork.
} // End of namespace Aki.

Q_DECLARE_METATYPE(Aki::SqlNetwork*)

#endif // AKI_SQLSERVER_HPP
