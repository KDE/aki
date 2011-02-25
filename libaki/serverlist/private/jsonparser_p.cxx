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

#include "jsonparser_p.hpp"
using namespace Aki;

JsonParserPrivate::JsonParserPrivate(Aki::JsonParser* qq)
    : _q(qq)
{
}

Aki::Sql::Address*
JsonParserPrivate::toAddress(const QVariantMap& addressMap, const Aki::Sql::Server* server)
{
    Q_ASSERT(server);
    Q_ASSERT(server->id() != -1);

    Aki::Sql::Address* address = new Aki::Sql::Address;
    address->setAddress(addressMap["address"].toString());
    address->setAddressServer(server->id());
    address->setPassword(addressMap["password"].toString());
    address->setPort(addressMap["port"].toInt());
    address->setPosition(addressMap["position"].toInt());
    address->setSsl(addressMap["ssl"].toBool());

    return address;
}

Aki::Sql::Channel*
JsonParserPrivate::toChannel(const QVariantMap& channelMap, const Aki::Sql::Server* server)
{
    Q_ASSERT(server);
    Q_ASSERT(server->id() != -1);

    Aki::Sql::Channel* channel = new Aki::Sql::Channel;
    channel->setChannelServer(server->id());
    channel->setName(channelMap["name"].toString());
    channel->setPassword(channelMap["password"].toString());

    return channel;
}

QVariantMap
JsonParserPrivate::toMap(const Aki::Sql::Server* server)
{
    QVariantMap map;

    map["autoIdentify"] = server->isAutoIdentify();
    map["autoJoinChannels"] = server->isAutoJoinChannels();
    map["autoReconnect"] = server->isAutoReconnect();
    map["connectOnStartup"] = server->isConnectOnStartup();
    map["connectToRandomServer"] = server->isConnectToRandomServer();
    map["defaultEncoding"] = server->defaultEncoding();
    map["encoding"] = server->encoding();
    map["name"] = server->name();
    map["retryAttemptCount"] = server->retryAttemptCount();
    map["retryInterval"] = server->retryInterval();
    map["serviceName"] = server->serviceName();
    map["servicePassword"] = server->servicePassword();

    return map;
}

QVariantMap
JsonParserPrivate::toMap(const Aki::Sql::Channel* channel)
{
    QVariantMap map;

    map["name"] = channel->name();
    map["password"] = channel->password();

    return map;
}

QVariantMap
JsonParserPrivate::toMap(const Aki::Sql::Address* address)
{
    QVariantMap map;

    map["address"] = address->address();
    map["password"] = address->password();
    map["port"] = address->port();
    map["position"] = address->position();
    map["ssl"] = address->isSslEnabled();

    return map;
}

Aki::Sql::Server*
JsonParserPrivate::toServer(const QVariantMap& serverMap)
{
    Aki::Sql::Server* server = new Aki::Sql::Server;
    server->setAutoIdentify(serverMap["autoIdentify"].toBool());
    server->setAutoJoinChannels(serverMap["autoJoinChannels"].toBool());
    server->setAutoReconnect(serverMap["autoReconnect"].toBool());
    server->setConnectOnStartup(serverMap["connectOnStartup"].toBool());
    server->setConnectToRandomServer(serverMap["connectToRandomServer"].toBool());
    server->setDefaultEncoding(serverMap["defaultEncoding"].toString());
    server->setEncoding(serverMap["encoding"].toString());
    server->setName(serverMap["name"].toString());
    server->setRetryAttemptCount(serverMap["retryAttemptCount"].toInt());
    server->setRetryInterval(serverMap["retryInterval"].toInt());
    server->setServerIdentity(identity->id());
    server->setServiceName(serverMap["serviceName"].toString());
    server->setServicePassword(serverMap["servicePassword"].toString());
    return server;
}
