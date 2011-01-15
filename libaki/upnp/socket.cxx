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

#include "socket.hpp"
#include "debughelper.hpp"
#include "router.hpp"
#include "private/socket_p.hpp"
#include <QtCore/QFile>
using namespace Aki;
using namespace Upnp;

Socket::Socket(QObject* parent)
    : QUdpSocket(parent),
    _d(new Aki::Upnp::SocketPrivate(this))
{
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            SLOT(error(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(readyRead()),
            SLOT(readyRead()));

    for (int i = 0; i < 15; ++i) {
        if (bind(1900 + i, QUdpSocket::ShareAddress)) {
            break;
        }
    }

    _d->joinMulticastGroup(socketDescriptor());
}

Socket::~Socket()
{
    qDeleteAll(_d->pendingRouterList);
    qDeleteAll(_d->routerList);
    _d->leaveMulticastGroup(socketDescriptor());
}

int
Socket::deviceCount() const
{
    return _d->routerList.count();
}

void
Socket::discover()
{
    QByteArray data;
    data.append("M-SEARCH * HTTP/1.1\r\n");
    data.append("HOST: 239.255.255.250:1900\r\n");
    data.append("ST:urn:schemas-upnp-org:device:InternetGatewayDevice:1\r\n");
    data.append("MAN:\"ssdp:discover\"\r\n");
    data.append("MX:3\r\n\r\n");
    writeDatagram(data.constData(), data.length(), QHostAddress("239.255.255.250"), 1900);
}

Aki::Upnp::Router*
Socket::findDevice(const QString& name)
{
    QUrl location(name);
    return _d->findDevice(location);
}

void
Socket::loadRouters(const QString& file)
{
    DEBUG_FUNC_NAME;

    QFile f(file);
    if (!f.open(QIODevice::ReadOnly)) {
        DEBUG_TEXT2("Unable to open file: %1", file);
        DEBUG_TEXT2("Error: %1", f.errorString());
        return;
    }

    QTextStream stream(&f);
    while (!stream.atEnd()) {
        QString server = stream.readLine();
        DEBUG_TEXT2("Read Server: %1", server);
        QString location = stream.readLine();
        DEBUG_TEXT2("Read Location: %1", location);

        Aki::Upnp::Router* router = new Aki::Upnp::Router(server, location);
        connect(router, SIGNAL(xmlFileDownloaded(Aki::Upnp::Router*,bool)),
                SLOT(xmlFileDownloaded(Aki::Upnp::Router*,bool)));
        router->downloadXmlFile();
        _d->pendingRouterList.append(router);
    }
}

void
Socket::saveRouters(const QString& file)
{
    DEBUG_FUNC_NAME;

    QFile f(file);
    if (!f.open(QIODevice::WriteOnly)) {
        DEBUG_TEXT2("Unable to open file: %1", file);
        DEBUG_TEXT2("Error: %1", f.errorString());
        return;
    }

    QTextStream stream(&f);
    foreach (Aki::Upnp::Router* router, _d->routerList) {
        stream << router->serverName() << endl;
        stream << router->xmlFile().toString() << endl;
    }

    f.close();
}

#include "test/upnp/socket.moc"
