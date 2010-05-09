#ifndef AKI_SERVER_HPP
#define AKI_SERVER_HPP

#include "libaki_export.hpp"
#include <QtCore/QMap>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>
#include <boost/tuple/tuple.hpp>

namespace Aki
{
typedef boost::tuple<QString,QString> Channel;
typedef boost::tuple<QString,quint16,QString,bool> Address;
typedef QList<Aki::Channel> ChannelList;
typedef QList<Aki::Address> AddressList;

class ServerPrivate;
class LIBAKI_EXPORT Server
{
public:
    typedef QSharedPointer<Aki::Server> Ptr;
    typedef QList<Aki::Server::Ptr> List;

    Server();
    explicit Server(const QString& name);
    ~Server();
    Aki::AddressList addressList() const;
    Aki::ChannelList channelList() const;
    QByteArray encoding() const;
    bool isAutoIdentifyEnabled() const;
    bool isAutoJoinChannelsEnabled() const;
    bool isAutoReconnectionEnabled() const;
    bool isConnectOnStartupEnabled() const;
    bool isConnectToRandomServerEnabled() const;
    bool isDefaultEncodingEnabled() const;
    bool isSslEnabled() const;
    QString name() const;
    int retryAttemptCount() const;
    int retryInterval() const;
    QString serviceName() const;
    QString servicePassword() const;
    void setAddressList(const Aki::AddressList& addresses);
    void setAutoIdentify(bool enabled);
    void setAutoJoinChannels(bool enabled);
    void setAutoReconnection(bool enabled);
    void setChannelList(const Aki::ChannelList& channels);
    void setConnectOnStartup(bool enabled);
    void setConnectToRandomServer(bool enabled);
    void setDefaultEncoding(bool enabled);
    void setEncoding(const QByteArray& encoding);
    void setName(const QString& name);
    void setRetryAttempts(int count);
    void setRetryInterval(int seconds);
    void setServiceName(const QString& name);
    void setServicePassword(const QString& password);
    void setSsl(bool enable);
private:
    friend class ServerPrivate;
    QScopedPointer<ServerPrivate> _d;
}; // End of class Server.
} // End of namespace Aki.

Q_DECLARE_METATYPE(Aki::Server*)
Q_DECLARE_METATYPE(Aki::Channel)
Q_DECLARE_METATYPE(Aki::Address)
Q_DECLARE_METATYPE(Aki::ChannelList)
Q_DECLARE_METATYPE(Aki::AddressList)

#endif // AKI_SERVER_HPP
