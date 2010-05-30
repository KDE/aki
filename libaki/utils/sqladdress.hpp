#ifndef AKI_SQLADDRESS_HPP
#define AKI_SQLADDRESS_HPP

#include "libaki_export.hpp"
#include <QtCore/QObject>

namespace Aki
{
class SqlNetwork;
class SqlAddressPrivate;
class LIBAKI_EXPORT SqlAddress : public QObject
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlAddress*> List;
    virtual ~SqlAddress();
    bool operator==(const Aki::SqlAddress& rhs) const;
    bool operator!=(const Aki::SqlAddress& rhs) const;
    static Aki::SqlAddress* newAddress(const QString& address, const Aki::SqlNetwork* server);
    static QList<Aki::SqlAddress*> addressListForServer(const Aki::SqlNetwork* server);
    static Aki::SqlAddress* findAddress(const QString& address, const Aki::SqlNetwork* server);
    void setAddress(const QString& address);
    QString address() const;
    void setPort(quint16 port);
    quint16 port() const;
    void setSsl(bool enabled);
    bool isSslEnabled() const;
    void setAddressServerId(int id);
    int addressServerId() const;
    int id() const;
    void setPassword(const QString& password);
    QString password() const;
    bool save();
    bool remove();
protected:
    explicit SqlAddress(QObject* parent = 0);
    void setId(int id);
private:
    friend class SqlAddressPrivate;
    QScopedPointer<SqlAddressPrivate> _d;
}; // End of class SqlAddress.
} // End of namespace Aki.

#endif // AKI_SQLADDRESS_HPP
