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

#include "jsonparser_p.hpp"
#include "serverlist/jsonparser.hpp"
#include "utils/sqladdress.hpp"
#include "utils/sqlchannel.hpp"
#include "utils/sqlidentity.hpp"
#include "utils/sqlnetwork.hpp"
#include "utils/sqlnickname.hpp"
#include <qjson/parser.h>
using namespace Aki;

JsonParserPrivate::JsonParserPrivate(JsonParser* qq)
    : QObject(),
    _q(qq)
{
}

QVariantMap
JsonParserPrivate::writeAddresses(SqlNetwork* network)
{
    QVariantMap addressMap;

    Aki::SqlAddress::List addressList = Aki::SqlAddress::addressListForServer(network);
    foreach (Aki::SqlAddress* address, addressList) {
        QVariantMap addy;
        addy["address"] = address->address();
        addy["password"] = address->password();
        addy["port"] = address->port();
        addy["ssl"] = address->isSslEnabled();
        addressMap.insertMulti("address", addy);
    }
    qDeleteAll(addressList);

    return addressMap;
}

QVariantMap
JsonParserPrivate::writeAuthentication(SqlNetwork* network)
{
    QVariantMap authenticationMap;
    authenticationMap["enable"] = network->isAutoIdentifyEnabled();
    authenticationMap["serviceName"] = network->serviceName();
    authenticationMap["servicePassword"] = network->servicePassword();
    return authenticationMap;
}

QVariantMap
JsonParserPrivate::writeChannels(SqlNetwork* network)
{
    QVariantMap channelMap;

    Aki::SqlChannel::List channelList = Aki::SqlChannel::channelListForServer(network);
    foreach (Aki::SqlChannel* channel, channelList) {
        QVariantMap channy;
        channy["channel"] = channel->channel();
        channy["password"] = channel->password();
        channelMap.insertMulti("channel", channy);
    }
    qDeleteAll(channelList);

    return channelMap;
}

QVariantMap
JsonParserPrivate::writeConnectionOptions(SqlNetwork* network)
{
    QVariantMap connectionOptions;
    connectionOptions["connectOnStartup"] = network->isConnectOnStartupEnabled();
    return connectionOptions;
}

QVariantMap
JsonParserPrivate::writeEncoding(SqlNetwork* network)
{
    QVariantMap encodingMap;
    encodingMap["enable"] = !network->isDefaultEncodingEnabled();
    encodingMap["encoding"] = network->encoding();
    return encodingMap;
}

QVariantMap
JsonParserPrivate::writeReconnection(SqlNetwork* network)
{
    QVariantMap reconnection;
    reconnection["enable"] = network->isAutoReconnectEnabled();
    reconnection["retryInterval"] = network->retryInterval();
    reconnection["retryAttempts"] = network->retryAttemptCount();
    return reconnection;
}

void
JsonParserPrivate::writeServer(QVariantMap* map)
{
    Aki::SqlNetwork::List networkList = Aki::SqlNetwork::networksForIdentity(identity);
    if (networkList.isEmpty()) {
        return;
    }

    foreach (Aki::SqlNetwork* network, networkList) {
        QVariantMap networkMap;
        networkMap["name"] = network->name();
        networkMap["reconnection"] = writeReconnection(network);
        networkMap["connectionOptions"] = writeConnectionOptions(network);
        networkMap["networkAddresses"] = writeAddresses(network);
        networkMap["connectToRandomServer"] = network->isConnectToRandomServerEnabled();
        networkMap["networkChannels"] = writeChannels(network);
        networkMap["autoJoinChannels"] = network->isAutoJoinChannelsEnabled();
        networkMap["encoding"] = writeEncoding(network);
        networkMap["authentication"] = writeAuthentication(network);
        map->insertMulti("network", networkMap);
    }
    qDeleteAll(networkList);
}
