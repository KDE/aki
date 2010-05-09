#ifndef AKI_SQLADDRESS_P_HPP
#define AKI_SQLADDRESS_P_HPP

#include "server.hpp"
#include <QtCore/QObject>
#include <QtSql/QSqlError>

namespace Aki
{
class SqlAddress;
class SqlAddressPrivate : public QObject
{
    Q_OBJECT
public:
    explicit SqlAddressPrivate(Aki::SqlAddress* qq);
    static void checkError(const QSqlError& error);
public:
    Aki::Address address;
    int id;
    int serverId;
private:
    Aki::SqlAddress* _q;
}; // End of class SqlAddressPrivate.
} // End of namespace Aki.

#endif // AKI_SQLADDRESS_P_HPP
