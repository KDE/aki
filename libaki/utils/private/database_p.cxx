#include "database_p.hpp"
#include "aki.hpp"
#include "utils/database.hpp"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
using namespace Aki;

DatabasePrivate::DatabasePrivate(Aki::Database* qq)
    : _q(qq)
{
}

void
DatabasePrivate::checkError(const QSqlError& error)
{
    if (error.isValid()) {
        qxtLog->error() << QString("Driver: %1").arg(error.driverText());
        qxtLog->error() << QString("Database: %1").arg(error.databaseText());
    }
}
