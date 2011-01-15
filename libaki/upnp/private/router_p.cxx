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

#include "router_p.hpp"
#include "debughelper.hpp"
#include "upnp/parser.hpp"
#include <QtCore/QBuffer>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkRequest>
#include <QtXml/QDomElement>
using namespace Aki;
using namespace Upnp;

RouterPrivate::RouterPrivate(Aki::Upnp::Router* qq)
    : externalIp(QString()),
    xmlFileLocation(QUrl()),
    description(Aki::Upnp::DeviceDescription()),
    _q(qq)
{
    serviceList.clear();
}

void
RouterPrivate::addPortForwarding(const Aki::Upnp::Service& service, quint16 port,
                                 Aki::Upnp::Router::Method method, const QString& description)
{
    QMap<QString, QString> argsList;
    argsList.insert("NewRemoteHost", QString());
    argsList.insert("NewExternalPort", QString::number(port));
    argsList.insert("NewProtocol", (method == Aki::Upnp::Router::Tcp) ? "TCP" : "UDP");
    argsList.insert("NewInternalPort", QString::number(port));

    bool foundLocalIP = false;
    QHostAddress localIP;

    foreach (const QNetworkInterface& iface, QNetworkInterface::allInterfaces()) {
        if ((iface.flags() & QNetworkInterface::IsUp) &&
            (iface.flags() & QNetworkInterface::IsRunning) &&
            (iface.flags() & ~QNetworkInterface::IsLoopBack)) {
            foreach (const QNetworkAddressEntry& entry, iface.addressEntries()) {
                if (entry.netmask().protocol() == QAbstractSocket::IPv4Protocol &&
                    entry.ip().toString() != "127.0.0.1") {
                    foundLocalIP = true;
                    localIP = entry.ip();
                    break;
                }
            }
        }
    }

    if (!foundLocalIP) {
        qDebug() << "Unable to find local ip address. Please submit bug report if this occurs.";
        return;
    }

    argsList.insert("NewInternalClient", localIP.toString());
    argsList.insert("NewEnabled", "1");
    argsList.insert("NewPortMappingDescription", description);
    argsList.insert("NewLeaseDuration", "0");

    const QString action = "AddPortMapping";
    QString query = createQuery(action, service.serviceType() + '#' + action, argsList);
    sendSoapQuery(query, service.serviceType() + '#' + action, service.controlUrl());

    Aki::Upnp::PortForward forward(service, port, method);
    pendingForwardPortsQueue.append(forward);
}

QString
RouterPrivate::createQuery(const QString& action, const QString& service)
{
    QString query;
    query.append("<?xml version=\"1.0\"?>");
    query.append("<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\"");
    query.append("    SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">");
    query.append("    <SOAP-ENV:Body>");
    query.append("        <m:" + action + " xmlns:m=\"" + service + "\" />");
    query.append("    </SOAP-ENV:Body>");
    query.append("</SOAP-ENV:Envelope>");
    return query;
}

QString
RouterPrivate::createQuery(const QString& action, const QString& service,
                           const QMap<QString, QString>& args)
{
    QString query;
    query.append("<?xml version=\"1.0\"?>");
    query.append("<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\"");
    query.append("    SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">");
    query.append("    <SOAP-ENV:Body>");
    query.append("        <m:" + action + " xmlns:m=\"" + service + "\">");

    QMapIterator<QString, QString> argsIter(args);
    while (argsIter.hasNext()) {
        argsIter.next();
        query.append("            <" + argsIter.key() + ">");
        query.append("                " + argsIter.value());
        query.append("            </" + argsIter.key() + ">");
    }

    query.append("        </m:" + action + ">");
    query.append("    </SOAP-ENV:Body>");
    query.append("</SOAP-ENV:Envelope>");
    return query;
}

void
RouterPrivate::removePortForwarding(const Aki::Upnp::Service& service, quint16 port,
                                    Aki::Upnp::Router::Method method)
{
    QMap<QString, QString> argsList;
    argsList.insert("NewRemoteHost", QString());
    argsList.insert("NewExternalPort", QString::number(port));
    argsList.insert("NewProtocol", (method == Aki::Upnp::Router::Tcp) ? "TCP" : "UDP");

    const QString action = "DeletePortMapping";
    QString query = createQuery(action, service.serviceType() + '#' + action, argsList);
    sendSoapQuery(query, service.serviceType() + '#' + action, service.controlUrl());

    Aki::Upnp::PortForward forward(service, port, method);
    pendingUnforwardPortsQueue.append(forward);
}

void
RouterPrivate::reply(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        if (reply->url().toString().contains(".xml")) {
            Aki::Upnp::Parser parser;
            bool status = parser.parse(reply->readAll(), _q);
            requestExternalIp();
            emit _q->xmlFileDownloaded(_q, status);
        } else {
            if (reply->request().rawHeader("SOAPAction").contains("GetExternalIPAddress")) {
                QDomDocument document;
                document.setContent(reply->readAll());
                externalIp = document.elementsByTagName("NewExternalIPAddress").at(0).toElement().text();
            } else if (reply->request().rawHeader("SOAPAction").contains("AddPortMapping")) {
                Aki::Upnp::PortForward forward = pendingForwardPortsQueue.dequeue();
                forwardPortsList.append(forward);
            } else if (reply->request().rawHeader("SOAPAction").contains("DeletePortMapping")) {
                Aki::Upnp::PortForward forward = pendingUnforwardPortsQueue.dequeue();
                forwardPortsList.removeOne(forward);
            }
        }
    } else {
        qDebug() << reply->errorString();
    }

    reply->deleteLater();
}

void
RouterPrivate::requestExternalIp()
{
    foreach (const Aki::Upnp::Service& service, serviceList) {
        if (service.serviceType().contains("WANIPConnection") ||
            service.serviceType().contains("WANPPPConnection")) {
            const QString action = "GetExternalIPAddress";
            const QString query = createQuery(action, service.serviceType());
            sendSoapQuery(query, service.serviceType() + '#' + action, service.controlUrl());
            break;
        }
    }
}

void
RouterPrivate::sendSoapQuery(const QString& query, const QString& action, const QUrl& url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(_q->xmlFile().toEncoded(QUrl::RemovePath)));
    request.setRawHeader("Host", _q->xmlFile().toEncoded(QUrl::RemovePath | QUrl::RemoveScheme).remove(0, 2));
    request.setRawHeader("User-Agent", "AkiIRC/0.0.4");
    request.setHeader(QNetworkRequest::ContentLengthHeader, query.length());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=\"utf-8\"");
    request.setRawHeader("SOAPAction", action.toUtf8());

    QBuffer* buffer = new QBuffer;
    buffer->setData(query.toUtf8());
    QNetworkReply* reply  = manager->sendCustomRequest(request, ("POST " + url.toString() + " HTTP/1.1").toUtf8(), buffer);
    buffer->setParent(reply);
}
