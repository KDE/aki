#include "sqlserver_p.hpp"
#include "aki.hpp"
#include "utils/sqlserver.hpp"
using namespace Aki;

SqlServerPrivate::SqlServerPrivate(SqlServer* qq)
    : QObject(),
    server(0),
    id(-1),
    serverId(-1),
    _q(qq)
{
}

void
SqlServerPrivate::checkError(const QSqlError& error)
{
    if (error.isValid()) {
        qxtLog->error() << QString(QLatin1String("Driver: %1")).arg(error.driverText());
        qxtLog->error() << QString(QLatin1String("Database: %1")).arg(error.databaseText());
    }
}

QVariant
SqlServerPrivate::checkString(const QString& str)
{
    if (str.isEmpty() || str.isNull()) {
        return QVariant(QVariant::String);
    }
    return str;
}
