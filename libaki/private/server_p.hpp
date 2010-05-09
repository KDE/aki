#ifndef AKI_SERVERPRIVATE_HPP
#define AKI_SERVERPRIVATE_HPP

#include "server.hpp"
#include <QtCore/QMap>
#include <QtCore/QStringList>

namespace Aki
{
class Server;
class ServerPrivate
{
public:
    ServerPrivate();
public:
    Aki::AddressList addressList;
    Aki::ChannelList channelList;
    QByteArray encoding;
    QString name;
    QString serviceName;
    QString servicePassword;
    bool isAutoIdentifyEnabled;
    bool isAutoJoinChannelsEnabled;
    bool isAutoReconnectionEnabled;
    bool isConnectOnStartupEnabled;
    bool isConnectToRandomServerEnabled;
    bool isDefaultEncodingEnabled;
    bool isSslEnabled;
    int retryAttemptCount;
    int retryInterval;
}; // End of class ServerPrivate.
} // End of namespace Aki.

#endif // AKI_SERVERPRIVATE_HPP
