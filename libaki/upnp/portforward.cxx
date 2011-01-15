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

#include "portforward.hpp"
#include "private/portforward_p.hpp"
using namespace Aki;
using namespace Upnp;

PortForward::PortForward()
    : _d(new Aki::Upnp::PortForwardPrivate)
{
}

PortForward::PortForward(const Aki::Upnp::Service& service, quint16 port, Aki::Upnp::Router::Method method)
    : _d(new Aki::Upnp::PortForwardPrivate)
{
    _d->service = service;
    _d->port = port;
    _d->method = method;
}

PortForward::PortForward(const Aki::Upnp::PortForward& other)
    : _d(other._d)
{
}

PortForward::~PortForward()
{
}

Aki::Upnp::PortForward&
PortForward::operator=(const Aki::Upnp::PortForward& other)
{
    _d = other._d;
    return *this;
}

bool
PortForward::operator!=(const Aki::Upnp::PortForward& other) const
{
    return (service() != other.service() || port() != other.port() || method() != other.method());
}

bool
PortForward::operator==(const Aki::Upnp::PortForward& other) const
{
    return (service() == other.service() && port() == other.port() && method() == other.method());
}

Aki::Upnp::Router::Method
PortForward::method() const
{
    return _d->method;
}

quint16
PortForward::port() const
{
    return _d->port;
}

const Aki::Upnp::Service&
PortForward::service() const
{
    return _d->service;
}
