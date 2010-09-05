#ifndef AKI_SQLCHANNEL_HPP
#define AKI_SQLCHANNEL_HPP

#include "aki.hpp"
#include <QtCore/QObject>

namespace Aki
{
class SqlNetwork;
class SqlChannelPrivate;
class LIBAKI_EXPORT SqlChannel : public QObject
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlChannel*> List;
    virtual ~SqlChannel();
    bool operator==(const Aki::SqlChannel& rhs) const;
    bool operator!=(const Aki::SqlChannel& rhs) const;
    static Aki::SqlChannel* newChannel(const QString& name, const Aki::SqlNetwork* server);
    static Aki::SqlChannel::List channelListForServer(const Aki::SqlNetwork* server);
    static Aki::SqlChannel* findChannel(const QString& name, const Aki::SqlNetwork* server);
    void setChannel(const QString& channel);
    QString channel() const;
    void setPassword(const QString& password);
    QString password() const;
    int id() const;
    void setChannelServerId(int id);
    int channelServerId() const;
    bool save();
    bool remove();
protected:
    explicit SqlChannel(QObject* parent = 0);
    void setId(int id);
private:
    AKI_DECLARE_PRIVATE(SqlChannel)
}; // End of class SqlChannel.
} // End of namespace Aki.

#endif // AKI_SQLCHANNEL_HPP
