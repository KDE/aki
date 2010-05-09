#include "sqlnickname.hpp"
#include "aki.hpp"
#include "private/sqlnickname_p.hpp"
#include "utils/sqlserver.hpp"
#include <QtCore/QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
using namespace Aki;

SqlNickname::SqlNickname(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::SqlNicknamePrivate(this));
}

SqlNickname::~SqlNickname()
{
}

bool
SqlNickname::operator!=(const Aki::SqlNickname& rhs) const
{
    return !(*this == rhs);
}

bool
SqlNickname::operator==(const Aki::SqlNickname& rhs) const
{
    return (id() == rhs.id());
}

Aki::SqlNickname*
SqlNickname::findNickname(const QString& nickname, const Aki::SqlServer* server)
{
    QScopedPointer<Aki::SqlNickname> tmp(new Aki::SqlNickname);
    if (nickname.isEmpty() || nickname.isNull()) {
        return 0;
    }

    QSqlQuery query;
    query.prepare(QLatin1String("SELECT * FROM Nickname WHERE nickname=? AND nicknameServer=? LIMIT 1"));
    query.addBindValue(nickname);
    query.addBindValue(server->id());

    if (!query.exec()) {
        Aki::SqlNicknamePrivate::checkError(query.lastError());
        return 0;
    }

    if (query.size() == 0) {
        return 0;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf(QLatin1String("id"));
    const int nicknameIndex = record.indexOf(QLatin1String("nickname"));
    const int nicknameServer = record.indexOf(QLatin1String("nicknameServer"));

    while (query.next()) {
        tmp->setNickname(query.value(nicknameIndex).toString());
        tmp->setNicknameServerId(query.value(nicknameServer).toInt());
        tmp->setId(query.value(id).toInt());
        return tmp.take();
    }

    return 0;
}

int
SqlNickname::id() const
{
    return _d->id;
}

Aki::SqlNickname*
SqlNickname::newNickname(const QString& nickname, const Aki::SqlServer* server)
{
    if (nickname.isEmpty() || nickname.isNull()) {
        return 0;
    }

    QSqlQuery query;
    query.prepare(QLatin1String("INSERT INTO Nickname (nickname,nicknameServer)"
                                " VALUES(?,?)"));

    QScopedPointer<Aki::SqlNickname> tmp(new Aki::SqlNickname);
    tmp->setNickname(nickname);
    tmp->setNicknameServerId(server->id());

    query.addBindValue(tmp->nickname());
    query.addBindValue(tmp->nicknameServerId());

    if (!query.exec()) {
        Aki::SqlNicknamePrivate::checkError(query.lastError());
        return 0;
    }

    tmp->setId(query.lastInsertId().toInt());
    return tmp.take();
}

QString
SqlNickname::nickname() const
{
    return _d->nickname;
}

int
SqlNickname::nicknameServerId() const
{
    return _d->serverId;
}

Aki::SqlNickname::List
SqlNickname::nicknamesForServer(const Aki::SqlServer* server)
{
    Aki::SqlNickname::List list;
    QSqlQuery query;
    query.prepare(QLatin1String("SELECT * FROM Server WHERE serverIdentity = ?"));
    query.addBindValue(server->id());

    if (!query.exec()) {
        Aki::SqlNicknamePrivate::checkError(query.lastError());
        return list;
    }

    if (query.size() == 0) {
        return list;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf(QLatin1String("id"));
    const int nicknameIndex = record.indexOf(QLatin1String("nickname"));
    const int nicknameServer = record.indexOf(QLatin1String("nicknameServer"));

    while (query.next()) {
        Aki::SqlNickname* nickname = new Aki::SqlNickname;
        nickname->setId(query.value(id).toInt());
        nickname->setNickname(query.value(nicknameIndex).toString());
        nickname->setNicknameServerId(query.value(nicknameServer).toInt());
        list.append(nickname);
    }

    return list;
}

void
SqlNickname::setId(int id)
{
    _d->id = id;
}

void
SqlNickname::setNickname(const QString& name)
{
    _d->nickname = name;
}

void
SqlNickname::setNicknameServerId(int id)
{
    _d->serverId = id;
}

bool
SqlNickname::save()
{
    QSqlQuery query;
    const QString str = QLatin1String("UPDATE Nickname SET nickname=?, nicknameServer=? WHERE id=?");
    query.prepare(str);
    query.addBindValue(nickname());
    query.addBindValue(nicknameServerId());
    query.addBindValue(id());

    if (!query.exec()) {
        Aki::SqlNicknamePrivate::checkError(query.lastError());
        return false;
    }

    return true;
}

bool
SqlNickname::remove()
{
    QSqlQuery query;
    const QString str = QLatin1String("DELETE FROM Nickname WHERE nickname=? AND id=?");
    query.prepare(str);
    query.addBindValue(nickname());
    query.addBindValue(nicknameServerId());

    if (!query.exec()) {
        Aki::SqlNicknamePrivate::checkError(query.lastError());
        return false;
    }

    return true;
}
