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

#ifndef AKI_SQL_DATABASE_HPP
#define AKI_SQL_DATABASE_HPP

#include "aki.hpp"
#include <QtCore/QObject>
#include <QtSql/QSqlError>

namespace Aki
{
namespace Sql
{
class LIBAKI_EXPORT Database
    : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject* parent = 0);
    ~Database();
    static bool open(const QString& path);
    static bool close();
    template<typename T> static bool create(T* table);
    template<typename T> static bool insert(T* table);
}; // End of class Database.
} // End of namespace Sql.
} // End of namespace Aki.

#endif // AKI_SQL_DATABASE_HPP
