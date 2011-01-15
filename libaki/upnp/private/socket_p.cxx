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

#include "socket_p.hpp"
#include "debughelper.hpp"
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#if defined(Q_WS_WIN)
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#endif // defined(Q_WS_WIN)

#include <sys/socket.h>
using namespace Aki;
using namespace Upnp;

SocketPrivate::SocketPrivate(Aki::Upnp::Socket* qq)
    : _q(qq)
{
}

void
SocketPrivate::error(QAbstractSocket::SocketError)
{
    qDebug() << _q->errorString();
}

Aki::Upnp::Router*
SocketPrivate::findDevice(const QUrl& location)
{
    if (!location.isValid()) {
        return 0;
    }

    foreach (Aki::Upnp::Router* router, routerList) {
        if (router->xmlFile() == location) {
            return router;
        }
    }

    return 0;
}

void
SocketPrivate::joinMulticastGroup(int socketDescriptor)
{
    struct ip_mreq mreq;
    memset(&mreq, 0, sizeof(struct ip_mreq));

    inet_aton("239.255.255.250", &mreq.imr_multiaddr);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

#if !defined(Q_WS_WIN)
    if (setsockopt(socketDescriptor, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) < 0) {
#else
    if (setsockopt(socketDescriptor, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<char*>(&mreq),
        sizeof(struct ip_mreq)) < 0) {
#endif // !defined(Q_WS_WIN)
//        DEBUG_TEXT("Unable to join multicast group 239.255.255.250");
    }
}

void
SocketPrivate::leaveMulticastGroup(int socketDescriptor)
{
    struct ip_mreq mreq;
    memset(&mreq, 0, sizeof(struct ip_mreq));

    inet_aton("239.255.255.250", &mreq.imr_multiaddr);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

#if !defined(Q_WS_WIN)
    if (setsockopt(socketDescriptor, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) < 0) {
#else
    if (setsockopt(socketDescriptor, IPPROTO_IP, IP_DROP_MEMBERSHIP, reinterpret_cast<char*>(&mreq),
        sizeof(struct ip_mreq)) < 0) {
#endif // !defined(Q_WS_WIN)
        //DEBUG_TEXT("Unable to leave multicast group 239.255.255.250");
    }
}

Aki::Upnp::Router*
SocketPrivate::parseResponse(const QByteArray& data)
{
    DEBUG_FUNC_NAME;
    
    const QStringList lines = QString::fromAscii(data).split("\r\n");
    QString server;
    QUrl location;

    QString line = lines.first();
    if (!line.contains("HTTP")) {
        if (!line.contains("NOTIFY") && !line.contains("200")) {
            DEBUG_TEXT("Unable to get valid 200 reply or notify");
            return 0;
        }
    } else if (line.contains("M-SEARCH")) {
        DEBUG_TEXT("Ignoring M-SEARCH");
        return 0;
    }

    bool validDevice = false;
    for (int i = 0, c = lines.count(); i < c && !validDevice; ++i) {
        line = lines.value(i);
        if ((line.contains("ST:") || line.contains("NT:")) && line.contains("InternetGatewayDevice")) {
            DEBUG_TEXT("Now have a valid device");
            validDevice = true;
        }
    }

    if (!validDevice) {
        DEBUG_TEXT("Unable to find valid device")
        return 0;
    }

    for (int i = 1, c = lines.count(); i < c; ++i) {
        line = lines.at(i);
        if (line.startsWith("Location", Qt::CaseInsensitive)) {
            location = line.mid(line.indexOf(':') + 1).trimmed();
            if (!location.isValid()) {
                DEBUG_TEXT("Location is invalid");
                return 0;
            } else {
                DEBUG_TEXT2("Location: %1", location.toString());
            }
        } else if (line.startsWith("Server", Qt::CaseInsensitive)) {
            server = line.mid(line.indexOf(':') + 1).trimmed();
            if (server.isEmpty()) {
                DEBUG_TEXT("Server is invalid.");
                return 0;
            } else {
                DEBUG_TEXT2("Server: %1", server);
            }
        }
    }

    if (_q->findDevice(location.toString())) {
        return 0;
    } else {
        DEBUG_TEXT("Detected IGD");
        return new Aki::Upnp::Router(server, location);
    }
    return 0;
}

void
SocketPrivate::readyRead()
{
    DEBUG_FUNC_NAME;
    if (_q->pendingDatagramSize() == 0) {
        DEBUG_TEXT("0 byte UDP packet");
        int fd = _q->socketDescriptor();
        char tmp;
        ::read(fd, &tmp, 1);
        return;
    }

    QByteArray data(_q->pendingDatagramSize(), 0);
    if (_q->readDatagram(data.data(), _q->pendingDatagramSize()) == -1) {
        return;
    }

    qDebug() << QString(data);

    Aki::Upnp::Router* router = parseResponse(data);
    if (router) {
        DEBUG_TEXT("Received valid router");
        _q->connect(router, SIGNAL(xmlFileDownloaded(Aki::Upnp::Router*,bool)),
                    _q, SLOT(xmlFileDownloaded(Aki::Upnp::Router*,bool)));
        router->downloadXmlFile();
        pendingRouterList.append(router);
    } else {
        DEBUG_TEXT("Received invalid router");
    }
}

void
SocketPrivate::xmlFileDownloaded(Aki::Upnp::Router* router, bool success)
{
    pendingRouterList.removeOne(router);
    
    if (!success) {
        router->deleteLater();
    } else {
        if (findDevice(router->xmlFile())) {
            router->deleteLater();
        } else {
            routerList.append(router);
            emit _q->discovered(router);
        }
    }
}
