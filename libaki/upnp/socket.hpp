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

#ifndef AKI_UPNP_SOCKET_HPP
#define AKI_UPNP_SOCKET_HPP

#include "aki.hpp"
#include <QtNetwork/QUdpSocket>

namespace Aki
{
namespace Upnp
{
class Router;
class SocketPrivate;
/**
 * The Socket class provides a socket for communication with the users router.
 *
 * If the end user has a router that supports UPnP, this will download an XML
 * file that receives all the available services so that the user can be able
 * to do port forwarding so they won't have to manually deal with it. As the
 * end user probably doesn't understand how to navigate the router's configuration
 * page.
 *
 * @code
 * Aki::Upnp::Socket* socket = new Aki::Upnp::Socket(this);
 * socket->discover();
 * @endcode
 */
class LIBAKI_EXPORT Socket
    : public QUdpSocket
{
    Q_OBJECT
public:
    /**
     * Creates a new Socket object.
     *
     * @param parent Parent of the object.
     */
    explicit Socket(QObject* parent = 0);
    /**
     * Destroys the Socket object.
     */
    ~Socket();
    /**
     * Get the number of available devices that the router can successfully
     * port forward on.
     *
     * @return Number of successful routers that allow port forwarding.
     */
    int deviceCount() const;
    /**
     * Gets a router from its @p name.
     *
     * @param name Name of the router
     *
     * @return A router object if a router was found with the @p name, 0 if
     * the router was not successfully found.
     */
    Aki::Upnp::Router* findDevice(const QString& name);
    /**
     * Loads a list of already found routers from the @p file.
     *
     * @param file Location of previously saved routers.
     */
    void loadRouters(const QString& file);
    /**
     * Saves a list of already found routers with the @p file
     *
     * @param file Filename with location.
     */
    void saveRouters(const QString& file);
public Q_SLOTS:
    /**
     * Attempt at discovering the services on the router.
     *
     * @note This must be called before you can do anything else.
     */
    void discover();
Q_SIGNALS:
    /**
     * This signal is emitted for every Router that is found on the router.
     * You should store the @p router if you want to be able to use the @p router
     * at anytime other than just when this signal is emitted because afterwards you
     * will not be able to access it again without calling discover function again.
     *
     * @param router Router that was discovered.
     */
    void discovered(Aki::Upnp::Router* router);
private:
    Q_PRIVATE_SLOT(_d, void error(QAbstractSocket::SocketError))
    Q_PRIVATE_SLOT(_d, void readyRead())
    Q_PRIVATE_SLOT(_d, void xmlFileDownloaded(Aki::Upnp::Router* router, bool success))
private:
    friend class Aki::Upnp::SocketPrivate;
    QScopedPointer<Aki::Upnp::SocketPrivate> _d;
}; // End of class Socket.
} // End of namespace Upnp.
} // End of namespace Aki.

#endif // AKI_UPNP_SOCKET_HPP
