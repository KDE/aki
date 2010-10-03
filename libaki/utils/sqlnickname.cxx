/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "sqlnickname.hpp"
#include "aki.hpp"
#include "utils/sqlidentity.hpp"
#include "private/sqlnickname_p.hpp"
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
SqlNickname::findNickname(const QString& nickname, const Aki::SqlIdentity* identity)
{
    QScopedPointer<Aki::SqlNickname> tmp(new Aki::SqlNickname);
    if (nickname.isEmpty() || nickname.isNull()) {
        return 0;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM Nickname WHERE nickname=? AND nicknameIdentity=? LIMIT 1");
    query.addBindValue(nickname);
    query.addBindValue(identity->id());

    if (!query.exec()) {
        Aki::SqlNicknamePrivate::checkError(query.lastError());
        return 0;
    }

    if (query.size() == 0) {
        return 0;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf("id");
    const int nicknameIndex = record.indexOf("nickname");
    const int nicknameIdentity = record.indexOf("nicknameIdentity");

    while (query.next()) {
        tmp->setNickname(query.value(nicknameIndex).toString());
        tmp->setNicknameIdentityId(query.value(nicknameIdentity).toInt());
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
SqlNickname::newNickname(const QString& nickname, const Aki::SqlIdentity* identity)
{
    if (nickname.isEmpty() || nickname.isNull()) {
        return 0;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Nickname (nickname,nicknameIdentity)"
                  " VALUES(?,?)");

    QScopedPointer<Aki::SqlNickname> tmp(new Aki::SqlNickname);
    tmp->setNickname(nickname);
    tmp->setNicknameIdentityId(identity->id());

    query.addBindValue(tmp->nickname());
    query.addBindValue(tmp->nicknameIdentityId());

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
SqlNickname::nicknameIdentityId() const
{
    return _d->serverId;
}

Aki::SqlNickname::List
SqlNickname::nicknamesForIdentity(const Aki::SqlIdentity* identity)
{
    Aki::SqlNickname::List list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Nickname WHERE nicknameIdentity = ?");
    query.addBindValue(identity->id());

    if (!query.exec()) {
        Aki::SqlNicknamePrivate::checkError(query.lastError());
        return list;
    }

    if (query.size() == 0) {
        return list;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf("id");
    const int nicknameIndex = record.indexOf("nickname");
    const int nicknameServer = record.indexOf("nicknameIdentity");

    while (query.next()) {
        Aki::SqlNickname* nickname = new Aki::SqlNickname;
        nickname->setId(query.value(id).toInt());
        nickname->setNickname(query.value(nicknameIndex).toString());
        nickname->setNicknameIdentityId(query.value(nicknameServer).toInt());
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
SqlNickname::setNicknameIdentityId(int id)
{
    _d->serverId = id;
}

bool
SqlNickname::save()
{
    QSqlQuery query;
    const QString str("UPDATE Nickname SET nickname=?, nicknameIdentity=? WHERE id=?");
    query.prepare(str);
    query.addBindValue(nickname());
    query.addBindValue(nicknameIdentityId());
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
    const QString str("DELETE FROM Nickname WHERE nickname=? AND id=?");
    query.prepare(str);
    query.addBindValue(nickname());
    query.addBindValue(id());

    if (!query.exec()) {
        Aki::SqlNicknamePrivate::checkError(query.lastError());
        return false;
    }

    return true;
}
