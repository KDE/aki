#ifndef AKI_SERVERCONFIG_HPP
#define AKI_SERVERCONFIG_HPP

#include "aki.hpp"
#include "configfile.hpp"

namespace Aki
{
class LIBAKI_EXPORT ServerConfig : public Aki::ConfigFile
{
    Q_OBJECT
public:
    explicit ServerConfig(QObject* parent = 0);
    ~ServerConfig();
    QStringList addressList();
    QStringList channelList();
    QByteArray encoding();
    bool isAutoIdentifyEnabled();
    bool isAutoJoinChannelsEnabled();
    bool isAutoReconnectEnabled();
    bool isConnectOnStartupEnabled();
    bool isConnectToRandomServerEnabled();
    bool isDefaultEncodingEnabled();
    bool isSslEnabled();
    int retryAttemptCount();
    int retryInterval();
    QString serviceName();
    QString servicePassword();
    void setAddressList(const QStringList& addresses);
    void setAutoIdentify(bool enabled);
    void setAutoJoinChannels(bool enabled);
    void setAutoReconnect(bool enabled);
    void setChannelList(const QStringList& channels);
    void setConnectOnStartup(bool enabled);
    void setConnectToRandomServer(bool enabled);
    void setDefaultEncoding(bool enabled);
    void setEncoding(const QByteArray& encoding);
    void setRetryAttempts(int count);
    void setRetryInterval(int seconds);
    void setServiceName(const QString& name);
    void setServicePassword(const QString& password);
    void setSsl(bool enabled);
}; // End of class ServerConfig.
} // End of namespace Aki.

#endif // AKI_SERVERCONFIG_HPP
