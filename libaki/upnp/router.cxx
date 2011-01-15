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

#include "router.hpp"
#include "debughelper.hpp"
#include "parser.hpp"
#include "private/router_p.hpp"
#include <QtNetwork/QNetworkReply>
using namespace Aki;
using namespace Upnp;

Router::Router(const QString& server, const QUrl& location, QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::Upnp::RouterPrivate(this));
    _d->manager = new QNetworkAccessManager(this);
    _d->serverName = server;
    _d->xmlFileLocation = location;
    connect(_d->manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(reply(QNetworkReply*)));
}

Router::~Router()
{
}

void
Router::addPortForwarding(quint16 port, Aki::Upnp::Router::Method method,
                          const QString& description)
{
    DEBUG_FUNC_NAME;
    DEBUG_TEXT3("Attempting to forward port: %1 using Method: %2", port,
                (method == Aki::Upnp::Router::Tcp) ? "TCP" : "UDP");

    bool found = false;

    foreach (const Aki::Upnp::Service& service, _d->serviceList) {
        if (service.serviceType().contains("WANIPConnection") ||
            service.serviceType().contains("WANPPPConnection")) {
            _d->addPortForwarding(service, port, method, description);
            found = true;
        }
    }

    if (!found) {
        DEBUG_TEXT("Unable to find WANIPConnection or WANPPPConnection");
    }
}

void
Router::addService(const Aki::Upnp::Service& service)
{
    _d->serviceList.append(service);
}

Aki::Upnp::DeviceDescription&
Router::description()
{
    return _d->description;
}

const Aki::Upnp::DeviceDescription&
Router::description() const
{
    return _d->description;
}

void
Router::downloadXmlFile()
{
    QNetworkRequest request;
    request.setUrl(xmlFile());
    _d->manager->get(request);
}

bool
Router::isPortForwarded(quint16 port)
{
    QMap<QString, QString> argList;
    argList.insert("NewRemoteHost", QString());
    argList.insert("NewExternalPort", QString::number(port));
    argList.insert("NewProtocol", "TCP");

    const QString action = "GetSpecificPortMappingEntry";
    const QString query = _d->createQuery(action, "", argList);
    _d->sendSoapQuery(query, '#' + action, QUrl());
    return false;
}

void
Router::removePortForwarding(quint16 port, Aki::Upnp::Router::Method method)
{
    DEBUG_FUNC_NAME;
    DEBUG_TEXT3("Attempting to remove forward port: %1 with method: %2", port,
                (method == Aki::Upnp::Router::Tcp) ? "TCP" : "UDP");

    QList<Aki::Upnp::PortForward>::Iterator portForwardIter = _d->forwardPortsList.begin();
    while (portForwardIter != _d->forwardPortsList.end()) {
        Aki::Upnp::PortForward& forward = *portForwardIter;
        if (forward.port() == port &&
            forward.method() == method) {
            _d->removePortForwarding(forward.service(), forward.port(), forward.method());
            portForwardIter = _d->forwardPortsList.erase(portForwardIter);
        } else {
            ++portForwardIter;
        }
    }
}

const QString&
Router::requestExternalIp() const
{
    return _d->externalIp;
}

const QString&
Router::serverName() const
{
    return _d->serverName;
}

const QUrl&
Router::xmlFile() const
{
    return _d->xmlFileLocation;
}

#include "test/upnp/router.moc"
