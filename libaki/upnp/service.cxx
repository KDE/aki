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

#include "service.hpp"
#include "private/service_p.hpp"
using namespace Aki;
using namespace Upnp;

Service::Service()
    : _d(new Aki::Upnp::ServicePrivate)
{
}

Service::Service(const Aki::Upnp::Service& other)
    : _d(other._d)
{
}

Service::~Service()
{
}

Aki::Upnp::Service&
Service::operator=(const Aki::Upnp::Service& other)
{
    _d = other._d;
    return *this;
}

bool
Service::operator!=(const Aki::Upnp::Service& other) const
{
    return (controlUrl() != other.controlUrl() ||
            eventSubUrl() != other.eventSubUrl() ||
            scpdUrl() != other.scpdUrl() ||
            serviceId() != other.serviceId() ||
            serviceType() != other.serviceType());
}

bool
Service::operator==(const Aki::Upnp::Service& other) const
{
    return (controlUrl() == other.controlUrl() &&
            eventSubUrl() == other.eventSubUrl() &&
            scpdUrl() == other.scpdUrl() &&
            serviceId() == other.serviceId() &&
            serviceType() == other.serviceType());
}

const QUrl&
Service::controlUrl() const
{
    return _d->controlUrl;
}

void
Service::clear()
{
    setControlUrl(QUrl());
    setEventSubUrl(QUrl());
    setScpdUrl(QUrl());
    setServiceId(QString());
    setServiceType(QString());
}

const QUrl&
Service::eventSubUrl() const
{
    return _d->eventSubUrl;
}

const QUrl&
Service::scpdUrl() const
{
    return _d->scpdUrl;
}

const QString&
Service::serviceId() const
{
    return _d->serviceId;
}

const QString&
Service::serviceType() const
{
    return _d->serviceType;
}

void
Service::setControlUrl(const QUrl& url)
{
    _d->controlUrl = url;
}

void
Service::setEventSubUrl(const QUrl& url)
{
    _d->eventSubUrl = url;
}

void
Service::setScpdUrl(const QUrl& url)
{
    _d->scpdUrl = url;
}

void
Service::setServiceId(const QString& id)
{
    _d->serviceId = id;
}

void
Service::setServiceType(const QString& type)
{
    _d->serviceType = type;
}
