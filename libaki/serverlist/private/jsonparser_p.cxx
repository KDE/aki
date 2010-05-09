#include "jsonparser_p.hpp"
#include "serverlist/jsonparser.hpp"
using namespace Aki;

JsonParserPrivate::JsonParserPrivate(JsonParser* qq)
    : QObject(),
    parser(0),
    _q(qq)
{
    serverList.clear();
}

void
JsonParserPrivate::writeNetworkName(const Aki::Server::Ptr& server, QVariantMap* map)
{
    map->insert(QLatin1String("name"), server->name());
}

void
JsonParserPrivate::writeReconnection(const Aki::Server::Ptr& server, QVariantMap* map)
{
    QVariantMap enableMap;
    enableMap.insert(QLatin1String("enabled"), server->isAutoReconnectionEnabled());
    map->insert(QLatin1String("reconnection"), enableMap);
}

void
JsonParserPrivate::writeRetryInterval(const Aki::Server::Ptr& server, QVariantMap* map)
{
    map->insert(QLatin1String("retryInterval"), server->retryInterval());
}

void
JsonParserPrivate::writeRetryAttempts(const Aki::Server::Ptr& server, QVariantMap* map)
{
    map->insert(QLatin1String("retryAttempts"), server->retryAttemptCount());
}

void
JsonParserPrivate::writeConnectionOptions(const Aki::Server::Ptr& server, QVariantMap* map)
{
    QVariantMap connectionMap;
    connectionMap.insert(QLatin1String("connectOnStartup"), server->isConnectOnStartupEnabled());
    connectionMap.insert(QLatin1String("ssl"), server->isSslEnabled());
    map->insert(QLatin1String("connectionOptions"), connectionMap);
}

void
JsonParserPrivate::writeServers(const Aki::Server::Ptr& server, QVariantMap* map)
{
//     QVariantMap serversMap;
//     serversMap.insert(QLatin1String("connectToRandomServer"), server->isConnectToRandomServerEnabled());
// 
//     QVariantMap serverMap;
//     QStringListIterator serverIter(server->addressList());
//     int i = 0;
//     while (serverIter.hasNext()) {
//         QVariantMap server;
//         const QStringList split = serverIter.next().split(QLatin1Char('/'));
//         server.insert(QLatin1String("address"), split.at(0));
//         server.insert(QLatin1String("port"), split.at(1));
//         serverMap.insert(QString(QLatin1String("server%1")).arg(i), server);
//         ++i;
//     }
// 
//     serversMap.insert(QLatin1String("server"), serverMap);
//     map->insert(QLatin1String("servers"), serversMap);
}

void
JsonParserPrivate::writeChannels(const Aki::Server::Ptr& server, QVariantMap* map)
{
//     QVariantMap channelsMap;
//     channelsMap.insert(QLatin1String("autoJoinChannels"), server->isConnectToRandomServerEnabled());
// 
//     QVariantMap channelMap;
//     QStringListIterator serverIter(server->channelList());
//     int i = 0;
//     while (serverIter.hasNext()) {
//         QVariantMap channel;
//         channel.insert(QLatin1String("channel"), serverIter.next());
//         channelMap.insert(QString(QLatin1String("channel%1")).arg(i), channel);
//         ++i;
//     }
// 
//     channelsMap.insert(QLatin1String("channel"), channelMap);
//     map->insert(QLatin1String("channels"), channelsMap);
}

void
JsonParserPrivate::writeEncoding(const Aki::Server::Ptr& server, QVariantMap* map)
{
    QVariantMap encodingMap;
    encodingMap.insert(QLatin1String("enable"), server->isDefaultEncodingEnabled());
    encodingMap.insert(QLatin1String("defaultEncoding"), server->encoding());
    map->insert(QLatin1String("encoding"), encodingMap);
}

void
JsonParserPrivate::writeIdentity(const Aki::Server::Ptr& server, QVariantMap* map)
{
    QVariantMap identityMap;
    identityMap.insert(QLatin1String("enable"), server->isAutoIdentifyEnabled());
    QVariantMap identity;
    identity.insert(QLatin1String("serviceName"), server->serviceName());
    identity.insert(QLatin1String("servicePassword"), server->servicePassword());
    identityMap.insert(QLatin1String("identity"), identity);
    map->insert(QLatin1String("identity"), identityMap);
}

void
JsonParserPrivate::readNetwork(QVariantMap serverMap)
{
    const int count = serverMap.count();

    for (int i = 0; i < count; ++i) {
        Aki::Server::Ptr ptr(new Aki::Server);

        QVariantMap server = serverMap[QString(QLatin1String("server%1")).arg(i)].toMap();
        readNetworkName(ptr, server);
    }
}

void
JsonParserPrivate::readNetworkName(Server::Ptr& server, QVariantMap serverMap)
{
    server->setName(serverMap[QLatin1String("name")].toString());
}
