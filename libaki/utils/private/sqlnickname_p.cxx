#include "sqlnickname_p.hpp"
#include "aki.hpp"
#include "utils/sqlnickname.hpp"
using namespace Aki;

SqlNicknamePrivate::SqlNicknamePrivate(Aki::SqlNickname* qq)
    : QObject(),
    nickname(QString()),
    id(-1),
    serverId(-1),
    _q(qq)
{
}

void
SqlNicknamePrivate::checkError(const QSqlError& error)
{
    if (error.isValid()) {
        qxtLog->error() << QString("Driver: %1").arg(error.driverText());
        qxtLog->error() << QString("Database: %1").arg(error.databaseText());
    }
}
