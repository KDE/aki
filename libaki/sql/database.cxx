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

#include "database.hpp"
#include "private/database_p.hpp"
#include <QtSql/QSqlQuery>
using namespace Aki;
using namespace Sql;

Database::Database(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::Sql::DatabasePrivate(this));
    Aki::Sql::TableList list = Aki::Sql::DatabasePrivate::tableList;
    foreach (Aki::Sql::Table* table, list) {
        _d->parseMetaObjects(table);
    }
}

Database::Database(const QString& type, const QString& connectionName, QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::Sql::DatabasePrivate(this));
    _d->db = QSqlDatabase::addDatabase(type, connectionName);
}

Database::~Database()
{
    if (isOpen()) {
        _d->db.close();
    }
    const QString connectionName = _d->db.connectionName();
    _d.reset();
    removeDatabase(connectionName);
}

void
Database::close()
{
    const QString name = _d->db.connectionName();
    QSqlDatabase::database().close();
}

QSqlDatabase
Database::database()
{
    return _d->db;
}

bool
Database::isOpen() const
{
    return _d->db.isOpen();
}

bool
Database::open(const QString& path) const
{
    _d->db.setDatabaseName(path);
    return _d->db.open();
}

bool
Database::registerClass(Aki::Sql::Table* t)
{
    if (t) {
        if (!Aki::Sql::DatabasePrivate::tableList.contains(t->metaObject()->className())) {
            Aki::Sql::DatabasePrivate::tableList.append(t);
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void Database::removeDatabase(const QString& connectionName)
{
    QSqlDatabase::removeDatabase(connectionName);
}
