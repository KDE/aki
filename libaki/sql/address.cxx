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

#include "address.hpp"
#include "private/address_p.hpp"
using namespace Aki;
using namespace Sql;

AKI_REGISTER_SQL_TABLE(Aki::Sql::Address, address)

Address::Address(QObject* parent)
    : Aki::Sql::Table(parent)
{
    _d.reset(new Aki::Sql::AddressPrivate(this));
}

Address::~Address()
{
}

QString
Address::address() const
{
    return _d->address;
}

int
Address::addressServer() const
{
    return _d->addressServer;
}

int
Address::id() const
{
    return _d->id;
}

bool
Address::isSslEnabled() const
{
    return _d->isSsl;
}

QString
Address::password() const
{
    return _d->password;
}

unsigned int
Address::port() const
{
    return _d->port;
}

void
Address::setAddress(const QString& address)
{
    if (_d->address != address) {
        _d->address = address;
    }
}

void
Address::setAddressServer(int addressServer)
{
    if (_d->addressServer != addressServer) {
        _d->addressServer = addressServer;
    }
}

void
Address::setId(int id)
{
    if (_d->id != id) {
        _d->id = id;
    }
}

void
Address::setPassword(const QString& password)
{
    if (_d->password == password) {
        _d->password = password;
    }
}

void
Address::setPort(unsigned int port)
{
    if (_d->port != port) {
        _d->port = port;
    }
}

void
Address::setSsl(bool enable)
{
    if (_d->isSsl == enable) {
        _d->isSsl = enable;
    }
}

#include "sql/address.moc"