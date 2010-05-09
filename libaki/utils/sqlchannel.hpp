#ifndef AKI_SQLCHANNEL_HPP
#define AKI_SQLCHANNEL_HPP

#include "libaki_export.hpp"
#include <QtCore/QObject>

namespace Aki
{
class SqlServer;
class SqlChannelPrivate;
class LIBAKI_EXPORT SqlChannel : public QObject
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlChannel*> List;
    virtual ~SqlChannel();
    bool operator==(const Aki::SqlChannel& rhs) const;
    bool operator!=(const Aki::SqlChannel& rhs) const;
    static Aki::SqlChannel* newChannel(const QString& name, const Aki::SqlServer* server);
    static Aki::SqlChannel::List channelListForServer(const Aki::SqlServer* server);
    static Aki::SqlChannel* findChannel(const QString& name, const Aki::SqlServer* server);
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
    friend class SqlChannelPrivate;
    QScopedPointer<SqlChannelPrivate> _d;
}; // End of class SqlChannel.
} // End of namespace Aki.

#endif // AKI_SQLCHANNEL_HPP
