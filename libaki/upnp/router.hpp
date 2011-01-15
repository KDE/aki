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

#ifndef AKI_UPNP_ROUTER_HPP
#define AKI_UPNP_ROUTER_HPP

#include "aki.hpp"
#include "upnp/devicedescription.hpp"
#include "upnp/service.hpp"
#include <QtCore/QObject>
#include <QtCore/QUrl>

class QNetworkReply;

namespace Aki
{
namespace Upnp
{
class RouterPrivate;
class LIBAKI_EXPORT Router
    : public QObject
{
    Q_OBJECT
public:
    enum Method {
        Tcp,
        Udp
    }; // End of enum Method.

    enum Error {
    }; // End of enum Error.

    explicit Router(const QString& server, const QUrl& location, QObject* parent = 0);
    ~Router();
    void addPortForwarding(quint16 port, Aki::Upnp::Router::Method method, const QString& description);
    void addService(const Aki::Upnp::Service& service);
    Aki::Upnp::DeviceDescription& description();
    const Aki::Upnp::DeviceDescription& description() const;
    void downloadXmlFile();
    const QString& requestExternalIp() const;
    bool isPortForwarded(quint16 port);
    void removePortForwarding(quint16 port, Aki::Upnp::Router::Method method);
    const QString& serverName() const;
    const QUrl& xmlFile() const;
Q_SIGNALS:
    void error(Aki::Upnp::Router::Error error);
    void xmlFileDownloaded(Aki::Upnp::Router* router, bool success);
private:
    Q_PRIVATE_SLOT(_d, void reply(QNetworkReply*));
private:
    friend class Aki::Upnp::RouterPrivate;
    QScopedPointer<Aki::Upnp::RouterPrivate> _d;
}; // End of class Router.
} // End of namespace Upnp.
} // End of namespace Aki.

QDebug LIBAKI_EXPORT operator<<(QDebug dbg, const Aki::Upnp::Router* router);

#endif // AKI_UPNP_ROUTER_HPP
