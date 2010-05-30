#ifndef AKI_SQLNETWORK_P_HPP
#define AKI_SQLNETWORK_P_HPP

#include "server.hpp"
#include <QtCore/QObject>
#include <QtSql/QSqlError>

namespace Aki
{
class SqlNetwork;
class SqlNetworkPrivate : public QObject
{
    Q_OBJECT
public:
    explicit SqlNetworkPrivate(Aki::SqlNetwork* qq);
    static void checkError(const QSqlError& error);
    static QVariant checkString(const QString& str);
    Aki::Server::Ptr server;
    int id;
    int serverId;
private:
    Aki::SqlNetwork* _q;
}; // End of class SqlNetworkPrivate.
} // End of namespace Aki.

#endif // AKI_SQLNETWORK_P_HPP
