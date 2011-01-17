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
/**
 * The Router class provides access to the router on the network.
 *
 * Allows for basic support of UPnP for port forwarding and getting the
 * external ip address. Making it easier for the end user to be able
 * to receive the information than having to go through the router's http
 * interface to allow this.
 *
 * @note This class shouldn't be constructed manually but instead from
 * the discovered signal of the Socket class.
 */
class LIBAKI_EXPORT Router
    : public QObject
{
    Q_OBJECT
public:
    /**
     *
     */
    enum Method {
        Tcp,
        Udp
    }; // End of enum Method.

    /**
     *
     */
    enum Error {
    }; // End of enum Error.

    /**
     * Creates a new Router object with the @p server name, @p location of the XML file received from
     * the router.
     *
     * @param server Server name.
     * @param location Location of the XML file from the router.
     * @param parent Parent of the object.
     */
    explicit Router(const QString& server, const QUrl& location, QObject* parent = 0);
    /**
     * Destroys the object.
     */
    ~Router();
    /**
     * Adds a port forwarding rule to the router with the @p port number, the protocol @p method and
     * a description for the port forward.
     *
     * @param port Port number. On Unix based OS's eg. FreeBSD, Linux, Mac OS X allows port access above
     * 1024 for users to use anything below requires root access except on Windows where anything is allowed.
     * @param method Protocol method.
     * @param description Description for the port forwarding. Can be empty.
     */
    void addPortForwarding(quint16 port, Aki::Upnp::Router::Method method, const QString& description);
    /**
     * Adds a new @p service to the router.
     *
     * @param service A new service to the router.
     *
     * @note Internal use only
     */
    void addService(const Aki::Upnp::Service& service);
    /**
     * Gets the device description of the router.
     *
     * @return Device description of the router.
     */
    Aki::Upnp::DeviceDescription& description();
    /**
     * Gets the device description of the router.
     *
     * @return Device description of the router.
     */
    const Aki::Upnp::DeviceDescription& description() const;
    /**
     * Requests a download of the XML file for the router.
     * @note Internal use only.
     */
    void downloadXmlFile();
    /**
     * Gets the external ip of the router.
     * @note This is only available after a little while after
     * the router was discovered since it requires time for the request
     * to be received from the router.
     *
     * @return The external ip address of the router; empty QString if called
     * before the router had time to receive the ip address from the router or
     * for unknown reasons.
     */
    const QString& requestExternalIp() const;
    /**
     * Gets the state of if the @p port is forward.
     *
     * @return true if the @p port was forwarded; false otherwise.
     */
    bool isPortForwarded(quint16 port);
    /**
     * Removes a port forwarding rule from the router for the @p port and the @p method.
     *
     * @param port Port number to remove the port forwarding.
     * @param method Protocol method the port has.
     */
    void removePortForwarding(quint16 port, Aki::Upnp::Router::Method method);
    /**
     * Gets the name of the server.
     *
     * @return Name of the server.
     */
    const QString& serverName() const;
    /**
     * Location of the XML file the router provides.
     *
     * @return XML file location.
     */
    const QUrl& xmlFile() const;
Q_SIGNALS:
    /**
     * This signal is emitted when there is an error.
     *
     * @param error Error that was emitted.
     */
    void error(Aki::Upnp::Router::Error error);
    /**
     * This signal is emitted when the XML file was downloaded.
     *
     * @param router Router that emitted this XML file request.
     * @param success true if the XML file was successfully parsed; false otherwise.
     */ 
    void xmlFileDownloaded(Aki::Upnp::Router* router, bool success);
private:
    Q_PRIVATE_SLOT(_d, void reply(QNetworkReply*))
private:
    friend class Aki::Upnp::RouterPrivate;
    QScopedPointer<Aki::Upnp::RouterPrivate> _d;
}; // End of class Router.
} // End of namespace Upnp.
} // End of namespace Aki.

QDebug LIBAKI_EXPORT operator<<(QDebug dbg, const Aki::Upnp::Router* router);

#endif // AKI_UPNP_ROUTER_HPP
