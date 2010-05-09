#include "sqlidentity_p.hpp"
#include "aki.hpp"
using namespace Aki;

SqlIdentityPrivate::SqlIdentityPrivate(Aki::SqlIdentity* qq)
    : QObject(),
    identity(0),
    id(-1),
    _q(qq)
{
}

void
SqlIdentityPrivate::checkError(const QSqlError& error)
{
    if (error.isValid()) {
        qxtLog->error() << QString(QLatin1String("Driver: %1")).arg(error.driverText());
        qxtLog->error() << QString(QLatin1String("Database: %1")).arg(error.databaseText());
    }
}

QVariant
SqlIdentityPrivate::checkString(const QString& str)
{
    if (str.isEmpty() || str.isNull()) {
        return QVariant(QVariant::String);
    }
    return str;
}
