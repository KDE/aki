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

#ifndef AKI_UPNP_SERVICE_HPP
#define AKI_UPNP_SERVICE_HPP

#include "aki.hpp"
#include <QtCore/QUrl>

namespace Aki
{
namespace Upnp
{
class ServicePrivate;
class LIBAKI_EXPORT Service
{
public:
    Service();
    Service(const Aki::Upnp::Service& other);
    ~Service();
    Aki::Upnp::Service& operator=(const Aki::Upnp::Service& other);
    bool operator!=(const Aki::Upnp::Service& other) const;
    bool operator==(const Aki::Upnp::Service& other) const;
    const QUrl& controlUrl() const;
    void clear();
    const QUrl& eventSubUrl() const;
    const QUrl& scpdUrl() const;
    const QString& serviceId() const;
    const QString& serviceType() const;
    void setControlUrl(const QUrl& url);
    void setEventSubUrl(const QUrl& url);
    void setScpdUrl(const QUrl& url);
    void setServiceId(const QString& id);
    void setServiceType(const QString& type);
private:
    QSharedDataPointer<Aki::Upnp::ServicePrivate> _d;
}; // End of class Service.
} // End of namespace Upnp
} // End of namespace Aki.

#endif // AKI_UPNP_SERVICE_HPP
