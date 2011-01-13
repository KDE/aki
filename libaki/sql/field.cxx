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

#include "field.hpp"
#include "private/field_p.hpp"
using namespace Aki;
using namespace Sql;

Field::Field(const QString& field, QVariant::Type type)
    : QSqlField(field, type),
    _d(new Aki::Sql::FieldPrivate)
{
}

Field::Field(const Aki::Sql::Field& other)
    : QSqlField(other),
    _d(other._d)
{
}

Field::~Field()
{
}

bool
Field::operator!=(const Aki::Sql::Field& other) const
{
    return (this != &other);
}

Aki::Sql::Field&
Field::operator=(const Aki::Sql::Field& other)
{
    QSqlField::operator=(other);
    _d = other._d;
    return *this;
}

bool
Field::operator==(const Aki::Sql::Field& other) const
{
    return (this == &other);
}

QString
Field::foreignKey() const
{
    return _d->foreignKey;
}

QString
Field::foreignKeyTable() const
{
    return _d->foreignKeyTable;
}

bool
Field::isPrimaryKey() const
{
    return _d->primaryKey;
}

void
Field::setForeignKey(const QString& key)
{
    Q_ASSERT(!key.isEmpty());
    _d->foreignKey = key;
}

void
Field::setForeignKeyTable(const QString& table)
{
    Q_ASSERT(!table.isEmpty());
    _d->foreignKeyTable = table;
}

void
Field::setAsPrimaryKey(bool isPrimaryKey)
{
    _d->primaryKey = isPrimaryKey;
}
