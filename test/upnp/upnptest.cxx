/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
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

#include "upnptest.hpp"
#include "upnp/router.hpp"
#include "upnp/socket.hpp"
#include <QtCore/QTimer>

UPnPTest::UPnPTest(QObject* parent)
    : QObject(parent),
    _socket(new Aki::Upnp::Socket(this))
{
    connect(_socket, SIGNAL(discovered(Aki::Upnp::Router*)),
            SLOT(discovered(Aki::Upnp::Router*)));
    _socket->discover();
}

void
UPnPTest::create20Forwards()
{
    if (!_router) {
        return;
    }

    int startPort = 40000;
    for (int i = 0; i < 20; ++i) {
        _router->addPortForwarding(startPort + i, Aki::Upnp::Router::Tcp, QString::number(i));
        qDebug() << "Forwarding Port: " << (startPort + i) << " Method: " << "TCP";
    }

    QTimer::singleShot(15000, this, SLOT(delete20Forwards()));
}

void
UPnPTest::delete20Forwards()
{
    if (!_router) {
        return;
    }

    int startPort = 40000;
    for (int i = 0; i < 20; ++i) {
        _router->removePortForwarding(startPort + i, Aki::Upnp::Router::Tcp);
        qDebug() << "Removing Forwarding Port: " << (startPort + i) << "TCP";
    }

    qDebug() << _router;
}

void
UPnPTest::discovered(Aki::Upnp::Router* router)
{
    _router = router;
    if (_router) {
        QTimer::singleShot(3000, this, SLOT(create20Forwards()));
    }
}

#include "test/upnp/upnptest.moc"
