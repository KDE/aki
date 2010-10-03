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

#ifndef AKI_SQLADDRESS_HPP
#define AKI_SQLADDRESS_HPP

#include "aki.hpp"
#include <QtCore/QObject>

namespace Aki
{
class SqlNetwork;
class SqlAddressPrivate;
class LIBAKI_EXPORT SqlAddress : public QObject
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlAddress*> List;
    virtual ~SqlAddress();
    bool operator==(const Aki::SqlAddress& rhs) const;
    bool operator!=(const Aki::SqlAddress& rhs) const;
    static Aki::SqlAddress* newAddress(const QString& address, const Aki::SqlNetwork* server);
    static QList<Aki::SqlAddress*> addressListForServer(const Aki::SqlNetwork* server);
    static Aki::SqlAddress* findAddress(const QString& address, const Aki::SqlNetwork* server);
    void setAddress(const QString& address);
    QString address() const;
    void setPort(quint16 port);
    quint16 port() const;
    void setSsl(bool enabled);
    bool isSslEnabled() const;
    void setAddressServerId(int id);
    int addressServerId() const;
    int id() const;
    void setPassword(const QString& password);
    QString password() const;
    bool save();
    bool remove();
protected:
    explicit SqlAddress(QObject* parent = 0);
    void setId(int id);
private:
    AKI_DECLARE_PRIVATE(SqlAddress)
}; // End of class SqlAddress.
} // End of namespace Aki.

#endif // AKI_SQLADDRESS_HPP
