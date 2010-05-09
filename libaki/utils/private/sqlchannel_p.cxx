#include "sqlchannel_p.hpp"
#include "aki.hpp"
#include "utils/sqlchannel.hpp"
using namespace Aki;

SqlChannelPrivate::SqlChannelPrivate(SqlChannel* qq)
    : QObject(),
    id(-1),
    serverId(-1),
    _q(qq)
{
}

void
SqlChannelPrivate::checkError(const QSqlError& error)
{
    if (error.isValid()) {
        qxtLog->error() << QString(QLatin1String("Driver: %1")).arg(error.driverText());
        qxtLog->error() << QString(QLatin1String("Database: %1")).arg(error.databaseText());
    }
}
