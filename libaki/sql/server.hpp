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

#ifndef AKI_SQL_SERVER_HPP
#define AKI_SQL_SERVER_HPP

#include "sql/database.hpp"
#include "sql/table.hpp"
#include <QtCore/QString>

namespace Aki
{
namespace Sql
{
class ServerPrivate;
/**
 * Server information about a row in the Server SQL table. Determines all the settings
 * for a single given row.
 */
class LIBAKI_EXPORT Server
    : public Aki::Sql::Table
{
    Q_OBJECT
    Q_CLASSINFO("id","PRIMARY_KEY,AUTO_INCREMENT")
    Q_CLASSINFO("encoding","LENGTH=0,NULL=false")
    Q_CLASSINFO("autoIdentify","NULL=false")
    Q_CLASSINFO("autoJoinChannels","NULL=false")
    Q_CLASSINFO("autoReconnect","NULL=false")
    Q_CLASSINFO("connectOnStartup","NULL=false")
    Q_CLASSINFO("connectToRandomServer","NULL=false")
    Q_CLASSINFO("defaultEncoding","NULL=false")
    Q_CLASSINFO("name","LENGTH=0,NULL=false,UNIQUE")
    Q_CLASSINFO("retryAttempts","NULL=false")
    Q_CLASSINFO("retryInterval","NULL=false")
    Q_CLASSINFO("serviceName","LENGTH=20,NULL=true")
    Q_CLASSINFO("servicePassword","LENGTH=60,NULL=true")
    Q_CLASSINFO("serverIdentity","FOREIGN_KEY=Identity;id")
    /**
     * This property holds the @p id of the row when it was commited.<br>
     * The @p id is unique. If the @p id is -1, it means it hasn't been committed yet.
     *
     * The default value is -1.
     */
    Q_PROPERTY(int id READ id WRITE setId)
    /**
     * This propery holds the @p encoding that the user uses instead of the default value.
     *
     * The default value is UTF-8.
     */
    Q_PROPERTY(QString encoding READ encoding WRITE setEncoding)
    /**
     * This property holds the state of whether to auto identify or not.
     *
     * The default value is false.
     */
    Q_PROPERTY(bool autoIdentify READ isAutoIdentify WRITE setAutoIdentify)
    /**
     * This property holds the state of whether to auto join channels on connect or not.
     *
     * The default value is false.
     */
    Q_PROPERTY(bool autoJoinChannels READ isAutoJoinChannels WRITE setAutoJoinChannels)
    /**
     * This property holds the state of whether to auto reconnect on disconnection or not.
     *
     * The default value is false.
     */
    Q_PROPERTY(bool autoReconnect READ isAutoReconnect WRITE setAutoReconnect)
    /**
     * This property holds the state of whether to automatically connect on startup or not.
     *
     * The default value is false.
     */
    Q_PROPERTY(bool connectOnStartup READ isConnectOnStartup WRITE setConnectOnStartup)
    /**
     * This property holds the state of whether to randomly connect to a group of addresses or not.
     * This will have no effect if there is 0 or 1 servers listed in the list.
     *
     * The default value is false.
     */
    Q_PROPERTY(bool connectToRandomServer READ isConnectToRandomServer WRITE setConnectToRandomServer)
    /**
     * This property holds the name of the default @p defaultEncoding.
     *
     * The default value is UTF-8.
     */
    Q_PROPERTY(QString defaultEncoding READ defaultEncoding WRITE setDefaultEncoding)
    /**
     * This property holds the name of the server.
     * This can be anything you want it to be.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString name READ name WRITE setName)
    /**
     * This property holds the numbers of retry attempts to try before giving up.
     *
     * The default value is 0 (unlimited).
     */
    Q_PROPERTY(int retryAttempts READ retryAttemptCount WRITE setRetryAttemptCount)
    /**
     * This property holds the number of intervals in seconds. This is used each
     * to determine how long to wait for the next retry.
     *
     * The default value is 10 (seconds).
     */
    Q_PROPERTY(int retryInterval READ retryInterval WRITE setRetryInterval)
    /**
     * This property holds the name of the service the server uses for authentication.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString serviceName READ serviceName WRITE setServiceName)
    /**
     * This property holds the password for the service the server uses for authentication.
     * This is the password for your username, if your server supports it.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString servicePassword READ servicePassword WRITE setServicePassword)
    /**
     * This property holds the Identity table's id.
     *
     * The default value is -1.
     */
    Q_PROPERTY(int serverIdentity READ serverIdentity WRITE setServerIdentity)
public:
    Q_INVOKABLE explicit Server(QObject* parent = 0);
    ~Server();
    /**
     * Gets the default encoding.
     *
     * @return Default encoding type.
     */
    QString defaultEncoding() const;
    /**
     * Gets the id of the row this data.
     *
     * @return -1 if the data has not yet been committed else the id at which it was committed.
     */
    int id() const;
    /**
     * Gets the custom encoding the user wants to use over the default encoding.
     *
     * @return Custom encoding type.
     */
    QString encoding() const;
    /**
     * Gets the state of auto identifying.
     *
     * @return true if auto identify; false otherwise.
     */
    bool isAutoIdentify() const;
    /**
     * Gets the state of auto joining channels on connection.
     *
     * @return true if auto joining channels; false otherwise.
     */
    bool isAutoJoinChannels() const;
    /**
     * Gets the state of auto reconnecting if disconnected.
     *
     * @return true if auto reconnecting; false otherwise.
     */
    bool isAutoReconnect() const;
    /**
     * Gets the state of connecting on startup.
     *
     * @return true if connecting on startup; false otherwise.
     */
    bool isConnectOnStartup() const;
    /**
     * Gets the state of connecting to random server.
     *
     * @return true if connecting to a random server; false otherwise.
     */
    bool isConnectToRandomServer() const;
    /**
     * Gets the name of the server.
     *
     * @return Name of the server.
     */
    QString name() const;
    /**
     * Gets the number of attempts the server will attempt reconnection before finally
     * giving up.
     *
     * @return Number of retry attempts.
     */
    int retryAttemptCount() const;
    /**
     * Gets the interval in seconds on how long to wait between each retry.
     *
     * @return Interval in seconds.
     */
    int retryInterval() const;
    /**
     * Gets the Identity table id assocated with this data.
     *
     * @return -1 if it this data isn't committed yet; otherwise it returns
     * the id of the row it was added at.
     */
    int serverIdentity() const;
    /**
     * Gets the service username for the server.
     *
     * @return Server username.
     */
    QString serviceName() const;
    /**
     * Gets the service password for the server.
     *
     * @return Service password.
     */
    QString servicePassword() const;
    /**
     * Sets the state of the server to automatically identify to the server when connected.
     * @note this only works if serviceName and servicePassword is set.
     *
     * @param enable true to auto identify; false otherwise.
     */
    void setAutoIdentify(bool enable);
    /**
     * Sets the state of the server whether to automatically join the channels upon
     * connection/reconnection.
     *
     * @param enable true to automatically join channels; false otherwise.
     */
    void setAutoJoinChannels(bool enable);
    /**
     * Sets the state of the server whether to connect automatically if disconnected
     * by the server but not by user interaction.
     *
     * @param enable true to automatically reconnect; false otherwise.
     */
    void setAutoReconnect(bool enable);
    /**
     * Sets the state of the server whether to connect on startup.
     *
     * @param enable true if connecting on startup; false otherwise.
     */
    void setConnectOnStartup(bool enable);
    /**
     * Sets the state of whether to randomly connect to a group of addresses or not.
     * This will have no effect if there is 0 or 1 servers listed in the list.
     *
     * @param enable true to enable random connect of address; false otherwise.
     */
    void setConnectToRandomServer(bool enable);
    /**
     * Sets the default @p encoding for the server. This is used
     * when the user does not want to use a custom one.
     * @note This shouldn't really be called as the default value is good enough.
     *
     * @param encoding Name of the encoding to use.
     */
    void setDefaultEncoding(const QString& encoding);
    /**
     * Sets the custom @p encoding for the server. This is used
     * for sending to the server.
     *
     * @param encoding Name of the encoding to use.
     */
    void setEncoding(const QString& encoding);
    void setId(int id);
    /**
     * Sets the @p name of the server.
     *
     * @param name Name of the server.
     */
    void setName(const QString& name);
    /**
     * Sets the numbers of retry attempts to try before giving up.
     *
     * @param count Retry attempt count.
     */
    void setRetryAttemptCount(int count);
    /**
     * Sets the number of intervals in @p seconds. This is used each
     * to determine how long to wait for the next retry.
     *
     * @param seconds Number of seconds for each retry interval.
     */
    void setRetryInterval(int seconds);
    void setServerIdentity(int id);
    /**
     * Sets the @p name of the service the server uses for authentication.
     *
     * @param name Name of the service the server uses (X, Nickserv, etc.)
     */
    void setServiceName(const QString& name);
    /**
     * Sets the password for the service the server uses for authentication.
     * This is the password for your username, if your server supports it.
     *
     * @param password Service password for your username.
     */
    void setServicePassword(const QString& password);
private:
    QSharedDataPointer<Aki::Sql::ServerPrivate> _d;
}; // End of class Server.
} // End of namespace Sql.
} // End of namespace Aki.

#endif // AKI_SQL_SERVER_HPP
