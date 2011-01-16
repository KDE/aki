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

#ifndef UPNPTEST_HPP
#define UPNPTEST_HPP

#include <QtCore/QObject>

namespace Aki
{
namespace Upnp
{
class Router;
class Socket;
} // End of namespace Upnp.
} // End of namespace Aki.

class UPnPTest
    : public QObject
{
    Q_OBJECT
public:
    explicit UPnPTest(QObject* parent = 0);
private Q_SLOTS:
    void discovered(Aki::Upnp::Router* router);
    void create20Forwards();
    void delete20Forwards();
private:
    Aki::Upnp::Socket* _socket;
    Aki::Upnp::Router* _router;
}; // End of class UPnPTest.

#endif // UPNPTEST_HPP
