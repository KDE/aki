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

#ifndef AKI_JSONPARSER_P_HPP
#define AKI_JSONPARSER_P_HPP

#include "server.hpp"
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace Aki
{
class JsonParser;
class SqlIdentity;
class SqlNetwork;
class JsonParserPrivate
{
public:
    JsonParserPrivate(Aki::JsonParser* qq);
    QVariantMap writeAddresses(Aki::SqlNetwork* network);
    QVariantMap writeAuthentication(Aki::SqlNetwork* network);
    QVariantMap writeChannels(Aki::SqlNetwork* network);
    QVariantMap writeConnectionOptions(Aki::SqlNetwork* network);
    QVariantMap writeEncoding(Aki::SqlNetwork* network);
    QVariantMap writeReconnection(Aki::SqlNetwork* network);
    void writeServer(QVariantMap* map);
public:
    QJson::Parser parser;
    QJson::Serializer serializer;
    Aki::SqlIdentity* identity;
private:
    Aki::JsonParser* _q;
}; // End of class JsonParserPrivate.
} // End of namespace Aki.

#endif // AKI_JSONPARSER_P_HPP
