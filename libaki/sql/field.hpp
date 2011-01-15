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

#ifndef AKI_SQL_FIELD_HPP
#define AKI_SQL_FIELD_HPP

#include "aki.hpp"
#include <QtSql/QSqlField>

namespace Aki
{
namespace Sql
{
class FieldPrivate;
class Field
    : public QSqlField
{
public:
    explicit Field(const QString& field, QVariant::Type type = QVariant::Invalid);
    Field(const Aki::Sql::Field& other);
    ~Field();
    Aki::Sql::Field& operator=(const Aki::Sql::Field& other);
    bool operator!=(const Aki::Sql::Field& other) const;
    bool operator==(const Aki::Sql::Field& other) const;
    QString foreignKey() const;
    QString foreignKeyTable() const;
    bool isPrimaryKey() const;
    void setForeignKey(const QString& key);
    void setForeignKeyTable(const QString& table);
    void setAsPrimaryKey(bool isPrimaryKey);
private:
    QSharedDataPointer<Aki::Sql::FieldPrivate> _d;
}; // End of class Field.
} // End of namespace Sql.
} // End of namespace Aki.

#endif // AKI_SQL_FIELD_HPP
