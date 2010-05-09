#include "server_p.hpp"
using namespace Aki;

ServerPrivate::ServerPrivate()
    : encoding(QByteArray()),
    name(QString()),
    serviceName(QString()),
    servicePassword(QString()),
    isAutoIdentifyEnabled(false),
    isAutoJoinChannelsEnabled(false),
    isAutoReconnectionEnabled(false),
    isConnectOnStartupEnabled(false),
    isConnectToRandomServerEnabled(false),
    isDefaultEncodingEnabled(false),
    isSslEnabled(false),
    retryAttemptCount(0),
    retryInterval(0)
{
    addressList.clear();
    channelList.clear();
}
