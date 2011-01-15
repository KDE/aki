/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
 * Copyright 2005-2007  Joris Guisson <joris.guisson@gmail.com>
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

#include "devicedescription.hpp"
#include "private/devicedescription_p.hpp"
using namespace Aki;
using namespace Upnp;

DeviceDescription::DeviceDescription()
    : _d(new Aki::Upnp::DeviceDescriptionPrivate)
{
}

DeviceDescription::DeviceDescription(const Aki::Upnp::DeviceDescription& other)
    : _d(other._d)
{
}

DeviceDescription::~DeviceDescription()
{
}

Aki::Upnp::DeviceDescription&
DeviceDescription::operator=(const Aki::Upnp::DeviceDescription& other)
{
    _d = other._d;
    return *this;
}

const QString&
DeviceDescription::friendlyName() const
{
    return _d->friendlyName;
}

const QString&
DeviceDescription::manufacturer() const
{
    return _d->manufacturer;
}

const QString&
DeviceDescription::modelDescription() const
{
    return _d->modelDescription;
}

const QString&
DeviceDescription::modelName() const
{
    return _d->modelName;
}

const QString&
DeviceDescription::modelNumber() const
{
    return _d->modelNumber;
}

void
DeviceDescription::setFriendlyName(const QString& name)
{
    _d->friendlyName = name;
}

void
DeviceDescription::setManufacturer(const QString& name)
{
    _d->manufacturer = name;
}

void
DeviceDescription::setModelDescription(const QString& description)
{
    _d->modelDescription = description;
}

void
DeviceDescription::setModelName(const QString& name)
{
    _d->modelName = name;
}

void
DeviceDescription::setModelNumber(const QString& modelNumber)
{
    _d->modelNumber = modelNumber;
}
