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

#ifndef AKI_UPNP_PARSER_HPP
#define AKI_UPNP_PARSER_HPP

#include "aki.hpp"
#include <QtCore/QUrl>

class QXmlStreamReader;

namespace Aki
{
namespace Upnp
{
class Router;
class ParserPrivate;
class Parser
{
public:
    Parser();
    Parser(const Aki::Upnp::Parser& other);
    ~Parser();
    Aki::Upnp::Parser& operator=(const Aki::Upnp::Parser& other);
    bool parse(const QString& file, Aki::Upnp::Router* router);
    bool parse(const QByteArray& data, Aki::Upnp::Router* router);
private:
    QSharedDataPointer<Aki::Upnp::ParserPrivate> _d;
}; // End of class Parser.
} // End of namespace Upnp.
} // End of namespace Aki.

#endif // AKI_UPNP_PARSER_HPP
