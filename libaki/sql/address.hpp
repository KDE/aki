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

#ifndef AKI_SQL_ADDRESS_HPP
#define AKI_SQL_ADDRESS_HPP

#include "aki.hpp"
#include "sql/database.hpp"
#include "sql/table.hpp"

namespace Aki
{
namespace Sql
{
class AddressPrivate;
class Address
    : public Aki::Sql::Table
{
    Q_OBJECT
    Q_CLASSINFO("id", "PRIMARY_KEY,AUTO_INCREMENT")
    Q_CLASSINFO("address", "LENGTH=0,NULL=false")
    Q_CLASSINFO("port", "NULL=false")
    Q_CLASSINFO("password", "LENGTH=0,NULL=true")
    Q_CLASSINFO("ssl", "NULL=false")
    Q_CLASSINFO("addressServer","FOREIGN_KEY=Server;id,NULL=false")
    /**
     * This property holds the @p id of the row when it was commited.<br>
     * The @p id is unique. If the @p id is -1, it means it hasn't been committed yet.
     *
     * The default value is -1. 
     */
    Q_PROPERTY(int id READ id WRITE setId)
    /**
     * This property holds the host @p address.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString address READ address WRITE setAddress)
    /**
     * This property holds the host @p port.
     *
     * The default value is 6667.
     */
    Q_PROPERTY(quint16 port READ port WRITE setPort)
    /**
     * This property holds the @p password for the server.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString password READ password WRITE setPassword)
    /**
     * This property holds the @p ssl state of whether to connect using SSL or not.
     *
     * The default value is false.
     */
    Q_PROPERTY(bool ssl READ isSslEnabled WRITE setSsl)
    /**
     * This property holds the Server table's id.
     *
     * The default value is -1.
     */
    Q_PROPERTY(long addressServer READ addressServer WRITE setAddressServer)
public:
    explicit Address(QObject* parent = 0);
    ~Address();
    /**
     * Gets the address for the server.
     *
     * @return Server address.
     */
    QString address() const;
    /**
     * Gets the Server table id assocated with this data.
     *
     * @return -1 if this data isn't committed yet; otherwise it returns
     * the id of the row it was added at. 
     */
    int addressServer() const;
    /**
     * Gets the Address id associated with this data.
     *
     * @return -1 if this data isn't committed yet; otherwise it returns
     * the id of the row it was added at.
     */
    int id() const;
    /**
     * Gets the state of the ssl on whether if this server will connect via
     * SSL.
     *
     * @return true if the server will connect via SSL; false otherwise.
     */
    bool isSslEnabled() const;
    /**
     * Gets the password for this server.
     *
     * @return true if this server requires a password; false otherwise.
     */
    QString password() const;
    /**
     * Gets the port for this address.
     *
     * @return Address port.
     */
    quint16 port() const;
    /**
     * Sets the @p address for to be able to connect to.
     * @note Cannot be nulled.
     *
     * @param address Server address.
     */
    void setAddress(const QString& address);
    void setAddressServer(int addressServer);
    void setId(int id);
    /**
     * Sets the @p password for this address.
     *
     * @param password Password for this address.
     */
    void setPassword(const QString& password);
    /**
     * Sets the @p port for this address.
     *
     * @param port Port number for this address.
     */
    void setPort(quint16 port);
    /**
     * Sets the SSL state for this address.
     *
     * @param enable true if this address is a SSL address; false otherwise.
     */
    void setSsl(bool enable);
private:
    AKI_DECLARE_PRIVATE(Address)
}; // End of class Address.
} // End of namespace Sql.
} // End of namespace Aki.

#endif // AKI_SQL_ADDRESS_HPP
