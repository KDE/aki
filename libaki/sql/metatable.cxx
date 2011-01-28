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

#include "metatable.hpp"
#include "debughelper.hpp"
#include "private/metatable_p.hpp"
#include "sql/table.hpp"
#include <QtSql/QSqlQuery>
#include <QSqlError>
using namespace Aki;
using namespace Sql;

MetaTable::MetaTable()
    : _d(new Aki::Sql::MetaTablePrivate)
{
}

MetaTable::MetaTable(const Aki::Sql::MetaTable& other)
    : _d(other._d)
{
}

MetaTable::~MetaTable()
{
}

Aki::Sql::MetaTable&
MetaTable::operator=(const Aki::Sql::MetaTable& other)
{
    _d = other._d;
    return *this;
}

bool
MetaTable::createTable()
{
    QString className = _d->name;

    if (className.contains("::")) {
        QString tmp = className.left(className.lastIndexOf("::") + 2);
        className.remove(0, tmp.count());
    }
    QString statement = "CREATE TABLE IF NOT EXISTS " + className + " (\n";

    QListIterator<Aki::Sql::Field> fieldIter(_d->fields);
    while (fieldIter.hasNext()) {
        const Aki::Sql::Field field = fieldIter.next();
        QString fieldText = "    " + field.name() + ' ';
        if (field.isValid()) {
            if (field.type() == QVariant::Bool) {
                fieldText.append("BOOLEAN");
            } else if (field.type() == QVariant::ByteArray) {
                fieldText.append("BLOB");
            } else if (field.type() == QVariant::Date) {
                fieldText.append("DATE");
            } else if (field.type() == QVariant::DateTime) {
                fieldText.append("DATETIME");
            } else if (field.type() == QVariant::String) {
                if (field.length() > 0) {
                    fieldText.append("VARCHAR(" + QString::number(field.length()) + ')');
                } else {
                    fieldText.append("TEXT");
                }
            } else if (field.type() == QVariant::Double) {
                fieldText.append("REAL");
            } else if (field.type() == QVariant::Int ||
                       field.type() == QVariant::LongLong ||
                       field.type() == QVariant::UInt ||
                       field.type() == QVariant::ULongLong) {
                fieldText.append("INTEGER");
            }

            if (field.isPrimaryKey()) {
                fieldText.append(" PRIMARY KEY");
            }

            if (field.isAutoValue()) {
                fieldText.append(" AUTOINCREMENT");
            }

            if (field.requiredStatus() == Aki::Sql::Field::Required) {
                fieldText.append(" NOT NULL");
            }

            if (!field.foreignKeyTable().isEmpty()) {
                Q_ASSERT(!field.foreignKey().isEmpty());
                fieldText.append(QString(",\n    FOREIGN KEY(%1) REFERENCES %2(%3) ON DELETE CASCADE")
                                    .arg(field.name(), field.foreignKeyTable(), field.foreignKey()));
            }

            if (fieldIter.hasNext()) {
                fieldText.append(",\n");
            }

            if (!fieldText.isEmpty()) {
                statement.append(fieldText);
            }
        }
    }

    QSqlQuery query;
    query.prepare(statement + "\n);");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool
MetaTable::dropTable()
{
    DEBUG_FUNC_NAME;
    QSqlQuery query("DROP TABLE " + name());
    if (query.exec()) {
        return true;
    }

    DEBUG_TEXT(query.lastError().text());
    return false;
}

QList<Aki::Sql::Field>
MetaTable::fields()
{
    return _d->fields;
}

QList<Aki::Sql::Field>
MetaTable::fields() const
{
    return fields();
}

QString
MetaTable::name() const
{
    return _d->name;
}

bool
MetaTable::parseMetaInformation(Aki::Sql::Table* table)
{
    Q_ASSERT_X(table, __PRETTY_FUNCTION__, "Unable to parse invalid table");
    _d->name = table->metaObject()->className();
    return _d->parseMetaObjects(table);
}
