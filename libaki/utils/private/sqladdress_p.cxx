#include "sqladdress_p.hpp"
#include "aki.hpp"
#include "utils/sqladdress.hpp"
using namespace Aki;

SqlAddressPrivate::SqlAddressPrivate(SqlAddress* qq)
    : QObject(),
    id(-1),
    serverId(-1),
    _q(qq)
{
}

void
SqlAddressPrivate::checkError(const QSqlError& error)
{
    if (error.isValid()) {
        qxtLog->error() << QString("Driver: %1").arg(error.driverText());
        qxtLog->error() << QString("Database: %1").arg(error.databaseText());
    }
}
