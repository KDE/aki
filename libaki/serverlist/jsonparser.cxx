#include "jsonparser.hpp"
#include "private/jsonparser_p.hpp"
#include <KDE/KDebug>
#include <qjson/serializer.h>
#include <QtCore/QVariant>
using namespace Aki;

JsonParser::JsonParser(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::JsonParserPrivate(this));
    _d->parser = QSharedPointer<QJson::Parser>(new QJson::Parser);
}

JsonParser::~JsonParser()
{
    _d->serverList.clear();
}

bool
JsonParser::read(QIODevice* device)
{
    Q_UNUSED(device);

    QByteArray jsonData = "{ \"channels\" : { \"autoJoinChannels\" : false, \"channel\" : { \"channel0\" : { \"channel\" : \"#aki\" },\
                          \"channel1\" : { \"channel\" : \"##C++\" }, \"channel10\" : { \"channel\" : \"#kde-cafe\" }, \"channel11\" :\
                           { \"channel\" : \"#kdevelop\" }, \"channel12\" : { \"channel\" : \"#qtwebkit\" }, \"channel13\" : { \"channel\"\
                            : \"#html\" }, \"channel14\" : { \"channel\" : \"#xml\" }, \"channel15\" : { \"channel\" : \"#kde-usability\" },\
                             \"channel16\" : { \"channel\" : \"#cmake\" }, \"channel17\" : { \"channel\" : \"#kde\" }, \"channel2\"\
                              : { \"channel\" : \"#gentoo\" }, \"channel3\" : { \"channel\" : \"#gentoo-chat\" }, \"channel4\" :\
                               { \"channel\" : \"#qt\" }, \"channel5\" : { \"channel\" : \"#kde\" }, \"channel6\" : { \"channel\" : \
                               \"#kde-bindings\" }, \"channel7\" : { \"channel\" : \"#kde-devel\" }, \"channel8\" : { \"channel\" : \
                               \"#python\" }, \"channel9\" : { \"channel\" : \"#amarok\" } } }, \"connectionOptions\" : { \
                               \"connectOnStartup\" : true, \"ssl\" : true }, \"encoding\" : { \"defaultEncoding\" : \"UTF-8\", \
                               \"enable\" : true }, \"identity\" : { \"enable\" : false, \"identity\" : { \"serviceName\" : \
                               \"NickServ\", \"servicePassword\" : \"rockets\" } }, \"name\" : \"Bouncer\", \"reconnection\" : \
                               { \"enabled\" : true }, \"retryAttempts\" : 100, \"retryInterval\" : 10, \"servers\" : \
                               { \"connectToRandomServer\" : false, \"server\" : { \"server0\" : { \"address\" : \"bnc.kollide.net\",\
                                \"port\" : \"7778\" }, \"server1\" : { \"address\" : \"chat.freenode.net\", \"port\" : \"7000\" } } } }";

    bool okay = false;
    QVariantMap result = _d->parser->parse(jsonData, &okay).toMap();
    if (!okay) {
        return okay;
    }

    /*qDebug() << "Name: " << result[QLatin1String("name")];
    QVariantMap channels = result[QLatin1String("channels")].toMap();
    qDebug() << "Auto Join Channels: " << channels[QLatin1String("autoJoinChannels")];
    channels = channels[QLatin1String("channel")].toMap();
    int count = channels.count();
    for (int i = 0; i < count; ++i) {
        QVariantMap channel = channels[QString(QLatin1String("channel%1")).arg(i)].toMap();
        qDebug() << "Channel: " << channel[QLatin1String("channel")];
    }

    QVariantMap connectionOptions = result[QLatin1String("connectionOptions")].toMap();
    qDebug() << "Connect On Startup: " << connectionOptions[QLatin1String("connectOnStartup")];
    qDebug() << "Ssl: " << connectionOptions[QLatin1String("ssl")];

    QVariantMap encoding = result[QLatin1String("encoding")].toMap();
    qDebug() << "Default Encoding: " << encoding[QLatin1String("defaultEncoding")];
    qDebug() << "Use Default Encoding: " << encoding[QLatin1String("enable")];

    QVariantMap identity = result[QLatin1String("identity")].toMap();
    qDebug() << "Use Auto Identify: " << identity[QLatin1String("enable")];
    identity = identity[QLatin1String("identity")].toMap();
    qDebug() << "Service Name: " << identity[QLatin1String("serviceName")];
    qDebug() << "Service Password: " << identity[QLatin1String("servicePassword")];

    QVariantMap reconnectionOptions = result[QLatin1String("reconnection")].toMap();
    qDebug() << "Use Auto Reconnection: " << reconnectionOptions[QLatin1String("enabled")];

    qDebug() << "Retry Attempts: " << result[QLatin1String("retryAttempts")];
    qDebug() << "Retry Interval: " << result[QLatin1String("retryInterval")];

    QVariantMap servers = result[QLatin1String("servers")].toMap();
    qDebug() << "Connect To Random Server: " << servers[QLatin1String("connectToRandomServer")];
    servers = servers[QLatin1String("server")].toMap();
    count = servers.count();
    qDebug() << servers;

    for (int i = 0; i < count; ++i) {
        QVariantMap server = servers[QString(QLatin1String("server%1")).arg(i)].toMap();
        qDebug() << server[QLatin1String("address")];
        qDebug() << server[QLatin1String("port")];
    }*/

    return okay;
}

Aki::Server::List
JsonParser::servers() const
{
    return _d->serverList;
}

void
JsonParser::setServerList(const Aki::Server::List& servers)
{
    _d->serverList = servers;
}

bool
JsonParser::write(QIODevice* device)
{
    Q_UNUSED(device);
    Aki::Server::Ptr server(new Aki::Server(QLatin1String("Bouncer")));
//    server->setAddressList(QStringList() << QLatin1String("bnc.kollide.net/7778") << QLatin1String("chat.freenode.net/7000"));
    server->setAutoIdentify(false);
    server->setAutoJoinChannels(false);
    server->setAutoReconnection(true);
//     server->setChannelList(QStringList() << QLatin1String("aki") << QLatin1String("##C++") << QLatin1String("#gentoo")
//     << QLatin1String("#gentoo-chat") << QLatin1String("#qt") << QLatin1String("#kde") << QLatin1String("#kde-bindings")
//     << QLatin1String("#kde-devel") << QLatin1String("#python") << QLatin1String("#amarok") << QLatin1String("#kde-cafe")
//     << QLatin1String("#kdevelop") << QLatin1String("#qtwebkit") << QLatin1String("#html") << QLatin1String("#xml")
//     << QLatin1String("#kde-usability") << QLatin1String("#cmake") << QLatin1String("#kde"));
    server->setConnectOnStartup(true);
    server->setConnectToRandomServer(false);
    server->setDefaultEncoding(true);
    server->setEncoding("UTF-8");
    server->setRetryAttempts(100);
    server->setRetryInterval(10);
    server->setServiceName(QLatin1String("NickServ"));
    server->setServicePassword(QLatin1String("rockets"));
    _d->serverList << server;

    QVariantMap jsonMap;
    foreach (const Aki::Server::Ptr& s, _d->serverList) {
        _d->writeNetworkName(s, &jsonMap);
        _d->writeReconnection(s, &jsonMap);
        _d->writeRetryInterval(s, &jsonMap);
        _d->writeRetryAttempts(s, &jsonMap);
        _d->writeConnectionOptions(s, &jsonMap);
        _d->writeServers(s, &jsonMap);
        _d->writeChannels(s, &jsonMap);
        _d->writeEncoding(s, &jsonMap);
        _d->writeIdentity(s, &jsonMap);
    }
    //QJson::Serializer serializer;
    //qDebug() << serializer.serialize(jsonMap);
    return true;
}
