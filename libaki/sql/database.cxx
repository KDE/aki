/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
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
#include "debughelper.hpp"
#include "private/database_p.hpp"
#include "sql/metatable.hpp"
#include "sql/table.hpp"
#include <QtSql/QSqlQuery>
using namespace Aki;
using namespace Sql;

Database::Database(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::Sql::DatabasePrivate(this));
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

bool
Database::commit()
{
    return _d->db.commit();
}

QString
Database::connectionName() const
{
    return _d->db.connectionName();
}

QString
Database::connectOptions() const
{
    return _d->db.connectOptions();
}

QSqlDatabase
Database::database()
{
    return _d->db;
}

QString
Database::databaseName() const
{
    return _d->db.databaseName();
}

QString
Database::hostName() const
{
    return _d->db.hostName();
}

bool
Database::isOpen() const
{
    return _d->db.isOpen();
}

Aki::Sql::MetaTable
Database::metaTable(const Aki::Sql::Table* table)
{
    foreach (Aki::Sql::MetaTable metaTable, Aki::Sql::DatabasePrivate::tableList) {
        if (table->metaObject()->className() == metaTable.name()) {
            return metaTable;
        }
    }

    return Aki::Sql::MetaTable();
}

bool
Database::open()
{
    return _d->db.open();
}

bool
Database::open(const QString& user, const QString& password)
{
    return _d->db.open(user, password);
}

QString
Database::password() const
{
    return _d->db.password();
}

int
Database::port() const
{
    return _d->db.port();
}

bool
Database::registerClass(Aki::Sql::Table* t)
{
    if (t) {
        if (!Aki::Sql::DatabasePrivate::tableList.contains(t->metaObject()->className())) {
            Aki::Sql::MetaTable table;
            if (table.parseMetaInformation(t)) {
                Aki::Sql::DatabasePrivate::tableList.append(table);
                return true;
            }
        }
    }

    return false;
}

void
Database::removeDatabase(const QString& connectionName)
{
    QSqlDatabase::removeDatabase(connectionName);
}

bool
Database::rollback()
{
    return _d->db.rollback();
}

void
Database::setConnectOptions(const QString& options)
{
    _d->db.setConnectOptions(options);
}

void
Database::setDatabaseName(const QString& name)
{
    _d->db.setDatabaseName(name);
}

void
Database::setHostName(const QString& host)
{
    _d->db.setHostName(host);
}

void
Database::setPassword(const QString& password)
{
    _d->db.setPassword(password);
}

void
Database::setPort(int port)
{
    _d->db.setPort(port);
}

void
Database::setUserName(const QString& name)
{
    _d->db.setUserName(name);
}

bool
Database::transaction()
{
    return _d->db.transaction();
}

QString
Database::userName() const
{
    return _d->db.userName();
}

#include "sql/database.moc"
