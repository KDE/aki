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

#ifndef AKI_SQL_TABLELIST_HPP
#define AKI_SQL_TABLELIST_HPP

#include "sql/table.hpp"
#include <QtCore/QList>
#include <QtCore/QMetaObject>

namespace Aki
{
namespace Sql
{
/**
 * Stores the tables that are registered with AKI_REGISTER_SQLTABLE.
 * @internal internal use only.
 */
class TableList
    : public QList<Aki::Sql::Table*>
{
public:
    /**
     * Empty TableList.
     */
    inline TableList() {}
    /**
     * Add @p table to the TableList.
     *
     * @param table Sql table to add.
     */
    inline TableList(Aki::Sql::Table* table)
    {
        append(table);
    }
    /**
     * Copy @p other TableList to this TableList.
     *
     * @param other Other TableList to copy.
     */
    inline TableList(const Aki::Sql::TableList& other)
        : QList<Aki::Sql::Table*>(other)
    {
    }
    /**
     * Copy @p other QList&lt;Aki::Sql::TableInterface&gt; to this TableList.
     *
     * @param other Other QList<Aki::Sql::Table>.
     */
    inline explicit TableList(const QList<Aki::Sql::Table*>& other)
        : QList<Aki::Sql::Table*>(other)
    {
    }
    /**
     * Searches the TableList for a class name matching the @p str. The search @p cs is case sensitive by default.
     * It will be case insensitive if @p cs is Qt::CaseInsentivity.
     *
     * @param str String to match
     * @param cs Case sensitivity
     * 
     * @return true if the TableList class name contains the @p str; false otherwise.
     */
    inline QBool contains(const QString& str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const
    {
        foreach (const Aki::Sql::Table* ti, *this) {
            if (QString(ti->metaObject()->className()).contains(str, cs)) {
                return QBool(true);
            }
        }
        return QBool(false);
    }
    /**
     * Filters the TableList for a class name matching the @p str. The search @p cs is case sensitive by default.
     * It will be case insensitive if @p cs is Qt::CaseInsensitivty.
     *
     * @param str String to match
     * @param cs Case sensitivity
     * @return A list of Tables matching the @p str. Empty if no matches were found.
     */
    inline Aki::Sql::TableList filter(const QString& str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const
    {
        Aki::Sql::TableList results;
        foreach (Aki::Sql::Table* ti, *this) {
            if (QString(ti->metaObject()->className()).contains(str, cs)) {
                results.append(ti);
            }
        }
        return results;
    }
    /**
     * Appends a new Table to the TableList.
     *
     * @param interface New Table to insert.
     * @return TableList containing the new @p interface.
     */
    inline Aki::Sql::TableList& operator<<(Aki::Sql::Table* table)
    {
        append(table);
        return *this;
    }
    /**
     * Appends another TableList to this TableList.
     *
     * @param other Other TableList to append to this one.
     * @return TableList containing the @p other TableList.
     */
    inline Aki::Sql::TableList& operator<<(const Aki::Sql::TableList& other)
    {
        append(other);
        return *this;
    }
    using QList<Aki::Sql::Table*>::indexOf;
    using QList<Aki::Sql::Table*>::lastIndexOf;
    inline int indexOf(const QString& str, int from = 0)
    {
        for (int i = from, s = size(); i < s; ++i) {
            if (QString(at(i)->metaObject()->className()) == str) {
                return i;
            }
        }

        return -1;
    }

    inline int lastIndexOf(const QString& str, int from = -1)
    {
        int last = count();
        if (from != -1) {
            last = from;
        }
        for (int i = last; i > 0; --i) {
            if (QString(at(i)->metaObject()->className()) == str) {
                return i;
            }
        }

        return -1;
    }
}; // End of class TableList.
} // End of namespace Sql.
} // End of namespace Aki.

#endif // AKI_SQL_TABLELIST_HPP
