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

#ifndef AKI_UPNP_PORTFORWARD_HPP
#define AKI_UPNP_PORTFORWARD_HPP

#include "aki.hpp"
#include "upnp/router.hpp"
#include "upnp/service.hpp"

namespace Aki
{
namespace Upnp
{
class PortForwardPrivate;
class PortForward
{
public:
    PortForward();
    explicit PortForward(const Aki::Upnp::Service& service, quint16 port, Aki::Upnp::Router::Method method);
    PortForward(const Aki::Upnp::PortForward& other);
    ~PortForward();
    Aki::Upnp::PortForward& operator=(const Aki::Upnp::PortForward& other);
    bool operator!=(const Aki::Upnp::PortForward& other) const;
    bool operator==(const Aki::Upnp::PortForward& other) const;
    Aki::Upnp::Router::Method method() const;
    quint16 port() const;
    const Aki::Upnp::Service& service() const;
private:
    QSharedDataPointer<Aki::Upnp::PortForwardPrivate> _d;
}; // End of class PortForward.
} // End of namespace Upnp.
} // End of namespace Aki.

#endif // AKI_UPNP_PORTFORWARD_HPP
