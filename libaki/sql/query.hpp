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

#ifndef AKI_SQL_QUERY_HPP
#define AKI_SQL_QUERY_HPP

#include "aki.hpp"
#include <QtCore/QSharedDataPointer>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

namespace Aki
{
namespace Sql
{
class QueryPrivate;
class LIBAKI_EXPORT Query
{
public:
    Query();
    Query(const Aki::Sql::Query& other);
    ~Query();
    void clear();
    bool exec();
    QSqlError lastError() const;
    QVariant lastInsertId() const;
    QString lastQuery() const;
    int numRowsAffected() const;
    void query(const QString& str);
    void query(const QString& str, const QVariantList& values);
    void query(const QString& str, const QVariant& a1);
    void query(const QString& str, const QVariant& a1, const QVariant& a2);
    void query(const QString& str, const QVariant& a1, const QVariant& a2,
               const QVariant& a3);
    void query(const QString& str, const QVariant& a1, const QVariant& a2,
               const QVariant& a3, const QVariant& a4);
    void query(const QString& str, const QVariant& a1, const QVariant& a2,
               const QVariant& a3, const QVariant& a4, const QVariant& a5);
    void query(const QString& str, const QVariant& a1, const QVariant& a2,
               const QVariant& a3, const QVariant& a4, const QVariant& a5,
               const QVariant& a6);
    void query(const QString& str, const QVariant& a1, const QVariant& a2,
               const QVariant& a3, const QVariant& a4, const QVariant& a5,
               const QVariant& a6, const QVariant& a7);
    void query(const QString& str, const QVariant& a1, const QVariant& a2,
               const QVariant& a3, const QVariant& a4, const QVariant& a5,
               const QVariant& a6, const QVariant& a7, const QVariant& a8);
    void query(const QString& str, const QVariant& a1, const QVariant& a2,
               const QVariant& a3, const QVariant& a4, const QVariant& a5,
               const QVariant& a6, const QVariant& a7, const QVariant& a8,
               const QVariant& a9);
    void query(const QString& str, const QVariant& a1, const QVariant& a2,
               const QVariant& a3, const QVariant& a4, const QVariant& a5,
               const QVariant& a6, const QVariant& a7, const QVariant& a8,
               const QVariant& a9, const QVariant& a10);
    void query(const QString& str, const QVariant& a1, const QVariant& a2,
               const QVariant& a3, const QVariant& a4, const QVariant& a5,
               const QVariant& a6, const QVariant& a7, const QVariant& a8,
               const QVariant& a9, const QVariant& a10, const QVariant& a11);
    QSqlRecord record() const;
private:
    QSharedDataPointer<QueryPrivate> _d;
}; // End of class Query.
} // End of namespace Sql.
} // End of namespace Aki.

#endif // AKI_SQL_QUERY_HPP
