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

#ifndef AKI_UPNP_ROUTER_P_HPP
#define AKI_UPNP_ROUTER_P_HPP

#include "aki.hpp"
#include "upnp/portforward.hpp"
#include "upnp/router.hpp"
#include <QtCore/QQueue>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

namespace Aki
{
namespace Upnp
{
class RouterPrivate
{
public:
    RouterPrivate(Aki::Upnp::Router* qq);
    void addPortForwarding(const Aki::Upnp::Service& service, quint16 port, Aki::Upnp::Router::Method method,
                           const QString& description);
    QString createQuery(const QString& action, const QString& service);
    QString createQuery(const QString& action, const QString& service, const QMap<QString, QString>& args);
    void removePortForwarding(const Aki::Upnp::Service& service, quint16 port, Aki::Upnp::Router::Method method);
    void reply(QNetworkReply* reply);
    void requestExternalIp();
    void sendSoapQuery(const QString& query, const QString& action, const QUrl& url);
public:
    QNetworkAccessManager* manager;
    QList<Aki::Upnp::Service> serviceList;
    QList<Aki::Upnp::PortForward> forwardPortsList;
    QQueue<Aki::Upnp::PortForward> pendingForwardPortsQueue;
    QQueue<Aki::Upnp::PortForward> pendingUnforwardPortsQueue;
    QString externalIp;
    QString serverName;
    QUrl xmlFileLocation;
    Aki::Upnp::DeviceDescription description;
private:
    Aki::Upnp::Router* _q;
}; // End of class RouterPrivate.
} // End of namespace Upnp.
} // End of namespace Aki.

#endif // AKI_UPNP_ROUTER_P_HPP
