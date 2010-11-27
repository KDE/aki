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
#include "debughelper.hpp"
#include "sql/query.hpp"
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <QtCore/QMetaClassInfo>
#include <QtSql/QSqlDatabase>
using namespace Aki;
using namespace Sql;
using namespace boost;
using namespace type_traits;

Database::Database(QObject* parent)
    : QObject(parent)
{

}

Database::~Database()
{
}

bool
Database::close()
{
    QSqlDatabase::database().close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    return false;
}

template<typename T> bool
Database::create(T* table)
{
    DEBUG_FUNC_NAME;

    typedef is_base_of<T, Aki::Sql::Database> Value;
    BOOST_STATIC_ASSERT(Value::value != false);

    const QMetaObject* object = table->metaObject();
    Q_ASSERT_X((object->indexOfClassInfo("TableName") != -1) || (object->classInfoCount() != 0), "Database::create",
               "Unable to find Q_CLASSINFO(\"TableName\")");

    QString queryStr;
    queryStr.append(QString("CREATE TABLE IF NOT EXISTS %1 (\n").arg(object->classInfo(0).value()));

    for (int i = 1; i < object->classInfoCount(); ++i) {
        QMetaClassInfo info = object->classInfo(i);
        queryStr.append(QString("    %1 %2,\n").arg(info.name(), info.value()));
    }

    queryStr.append(");");

    Aki::Sql::Query query;
    query.query(queryStr);
    if (!query.exec()) {
        DEBUG_TEXT(query.lastError().text());
        return false;
    }

    return true;
}

template<typename T> bool
Database::insert(T* table)
{
    typedef is_base_of<T, Aki::Sql::Database> Value;
    BOOST_STATIC_ASSERT(Value::value != false);

    const QMetaObject* object = table->metaObject();
    Q_ASSERT_X((object->indexOfClassInfo("TableName") != -1) || (object->classInfoCount() != 0), "Database::create",
               "Unable to find Q_CLASSINFO(\"TableName\")");

    QString queryStr;
    queryStr.append(QString("INSERT INTO %1 ").arg(object->classInfo(0).value()));
    QString columns;
    QString values;
    QVariantList valueList;

    const int count = object->classInfoCount();
    for (int i = 1; i < count; ++i) {
        QMetaClassInfo info = object->classInfo(i);
        columns.append(info.name());
        if (i == count) {
            values.append("?);");
        } else {
            values.append("?,");
        }
    }

    columns = '(' + columns + ')';

    Aki::Sql::Query query;

    return false;
}

bool
Database::open(const QString& path)
{
    DEBUG_FUNC_NAME;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()) {
        DEBUG_TEXT("Unable to open database file")
    }

    Aki::Sql::Query query;
    query.query("PRAGMA foreign_keys=ON");
    return query.exec();
}
