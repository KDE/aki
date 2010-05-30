#include "sqlnetwork_p.hpp"
#include "aki.hpp"
#include "utils/sqlserver.hpp"
using namespace Aki;

SqlNetworkPrivate::SqlNetworkPrivate(SqlNetwork* qq)
    : QObject(),
    server(0),
    id(-1),
    serverId(-1),
    _q(qq)
{
}

void
SqlNetworkPrivate::checkError(const QSqlError& error)
{
    if (error.isValid()) {
        qxtLog->error() << QString("Driver: %1").arg(error.driverText());
        qxtLog->error() << QString("Database: %1").arg(error.databaseText());
    }
}

QVariant
SqlNetworkPrivate::checkString(const QString& str)
{
    if (str.isEmpty() || str.isNull()) {
        return QVariant(QVariant::String);
    }
    return str;
}
