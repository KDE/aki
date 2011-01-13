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

#ifndef AKI_SQL_TABLE_HPP
#define AKI_SQL_TABLE_HPP

#include "aki.hpp"
#include <QtCore/QObject>

namespace Aki
{
namespace Sql
{
class TablePrivate;
class LIBAKI_EXPORT Table
    : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates a new 
     */
    explicit Table(QObject* parent = 0);
    /**
     * Destroys the object but still keeps the record in the database.
     */
    virtual ~Table();
protected:
    /**
     * Creates the table.
     *
     * @internal Internal use only.
     */
    Q_INVOKABLE bool create();
    /**
     * Drops the table and all the records from the table.
     *
     * @internal Internal use only.
     */
    Q_INVOKABLE bool remove();
private:
    AKI_DECLARE_PRIVATE(Table)
}; // End of class Table.
} // End of namespace Sql.
} // End of namespace Aki.

#endif // AKI_SQL_TABLE_HPP
