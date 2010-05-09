#ifndef AKI_SQLSERVER_P_HPP
#define AKI_SQLSERVER_P_HPP

#include "server.hpp"
#include <QtCore/QObject>
#include <QtSql/QSqlError>

namespace Aki
{
class SqlServer;
class SqlServerPrivate : public QObject
{
    Q_OBJECT
public:
    explicit SqlServerPrivate(Aki::SqlServer* qq);
    static void checkError(const QSqlError& error);
    static QVariant checkString(const QString& str);
    Aki::Server::Ptr server;
    int id;
    int serverId;
private:
    Aki::SqlServer* _q;
}; // End of class SqlServerPrivate.
} // End of namespace Aki.

#endif // AKI_SQLSERVER_P_HPP
