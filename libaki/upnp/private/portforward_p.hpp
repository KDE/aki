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

#ifndef AKI_UPNP_PORTFORWARD_P_HPP
#define AKI_UPNP_PORTFORWARD_P_HPP

#include "upnp/router.hpp"
#include "upnp/service.hpp"
#include <QtCore/QSharedData>

namespace Aki
{
namespace Upnp
{
class PortForwardPrivate
    : public QSharedData
{
public:
    PortForwardPrivate();
    PortForwardPrivate(const Aki::Upnp::PortForwardPrivate& other);
public:
    Aki::Upnp::Service service;
    Aki::Upnp::Router::Method method;
    quint16 port;
}; // End of class PortForwardPrivate.
} // End of namespace Upnp.
} // End of namespace Aki.

#endif // AKI_UPNP_PORTFORWARD_P_HPP
