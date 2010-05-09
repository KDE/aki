#ifndef AKI_JSONPARSER_P_HPP
#define AKI_JSONPARSER_P_HPP

#include "server.hpp"
#include <qjson/parser.h>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QVariant>

namespace Aki
{
class JsonParser;
class JsonParserPrivate : public QObject
{
    Q_OBJECT
public:
    JsonParserPrivate(Aki::JsonParser* qq);
    void writeNetworkName(const Aki::Server::Ptr& server, QVariantMap* map);
    void writeReconnection(const Aki::Server::Ptr& server, QVariantMap* map);
    void writeRetryInterval(const Aki::Server::Ptr& server, QVariantMap* map);
    void writeRetryAttempts(const Aki::Server::Ptr& server, QVariantMap* map);
    void writeConnectionOptions(const Aki::Server::Ptr& server, QVariantMap* map);
    void writeServers(const Aki::Server::Ptr& server, QVariantMap* map);
    void writeChannels(const Aki::Server::Ptr& server, QVariantMap* map);
    void writeEncoding(const Aki::Server::Ptr& server, QVariantMap* map);
    void writeIdentity(const Aki::Server::Ptr& server, QVariantMap* map);
    void readNetwork(QVariantMap serverMap);
    void readNetworkName(Aki::Server::Ptr& server, QVariantMap serverMap);
public:
    QSharedPointer<QJson::Parser> parser;
    Aki::Server::List serverList;
private:
    Aki::JsonParser* _q;
}; // End of class JsonParserPrivate.
} // End of namespace Aki.

#endif // AKI_JSONPARSER_P_HPP
