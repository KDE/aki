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
class JsonParserPrivate : public QObject
{
    Q_OBJECT
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
