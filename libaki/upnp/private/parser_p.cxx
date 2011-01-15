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

#include "parser_p.hpp"
#include "debughelper.hpp"
using namespace Aki;
using namespace Upnp;

ParserPrivate::ParserPrivate()
    : QSharedData(),
    router(0),
    isWANIPorWANPPP(false),
    isWANConnection(false)
{
}

ParserPrivate::ParserPrivate(const Aki::Upnp::ParserPrivate& other)
    : QSharedData(other),
    router(other.router),
    isWANIPorWANPPP(other.isWANIPorWANPPP),
    isWANConnection(other.isWANConnection)
{
}

bool
ParserPrivate::readDevice()
{
    DEBUG_FUNC_NAME;
    Q_ASSERT(xml.isStartElement() && xml.name() == "device");
    while (xml.readNextStartElement()) {
        if (xml.name() == "deviceType") {
            if (isWANConnection) {
                isWANConnection = false;
                isWANIPorWANPPP = false;
            }
            const QString name = xml.readElementText();
            if (name.contains("WANConnectionDevice")) {
                isWANConnection = true;
            } else {
                isWANConnection = false;
            }
        } else if (xml.name() == "friendlyName") {
            if (isWANConnection) {
                router->description().setFriendlyName(xml.readElementText());
            } else {
                xml.skipCurrentElement();
            }
        } else if (xml.name() == "manufacturer") {
            if (isWANConnection) {
                router->description().setManufacturer(xml.readElementText());
            } else {
                xml.skipCurrentElement();
            }
        } else if (xml.name() == "modelDescription") {
            if (isWANConnection) {
                router->description().setModelDescription(xml.readElementText());
            } else {
                xml.skipCurrentElement();
            }
        } else if (xml.name() == "modelName") {
            if (isWANConnection) {
                router->description().setModelName(xml.readElementText());
            } else {
                xml.skipCurrentElement();
            }
        } else if (xml.name() == "modelNumber") {
            router->description().setModelNumber(xml.readElementText());
        } else if (xml.name() == "serviceList") {
            readServiceList();
        } else if (xml.name() == "deviceList") {
            readDeviceList();
        } else {
            xml.skipCurrentElement();
        }
    }
    return true;
}

bool
ParserPrivate::readDeviceList()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "deviceList");
    while (xml.readNextStartElement()) {
        if (xml.name() == "device") {
            readDevice();
        }
    }
    return true;
}

bool
ParserPrivate::readRoot()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "root");
    while (xml.readNextStartElement()) {
        if (xml.name() == "specVersion") {
            xml.skipCurrentElement();
        } else if (xml.name() == "device") {
            readDevice();
        }
    }
    return true;
}

bool
ParserPrivate::readService()
{
    DEBUG_FUNC_NAME;
    Q_ASSERT(xml.isStartElement() && xml.name() == "service");
    while (xml.readNextStartElement()) {
        if (xml.name() == "serviceType") {
            service.setServiceType(xml.readElementText());
        } else if (xml.name() == "serviceId") {
            service.setServiceId(xml.readElementText());
        } else if (xml.name() == "SCPDURL") {
            service.setScpdUrl(QUrl(xml.readElementText()));
        } else if (xml.name() == "controlURL") {
            const QString text = xml.readElementText();
            service.setControlUrl(QUrl(text));
        } else if (xml.name() == "eventSubURL") {
            service.setEventSubUrl(QUrl(xml.readElementText()));
        }
    }

    router->addService(service);
    service.clear();

    return true;
}

bool
ParserPrivate::readServiceList()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "serviceList");
    while (xml.readNextStartElement()) {
        if (xml.name() == "service") {
            readService();
        }
    }
    return true;
}
