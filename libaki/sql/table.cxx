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

#include "table.hpp"
#include "private/table_p.hpp"
#include <QSqlQuery>
#include "database.hpp"
#include <qsqldriver.h>
using namespace Aki;
using namespace Sql;

Table::Table(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::Sql::TablePrivate(this));
}

Table::~Table()
{
}

bool
Table::create()
{
    return false;
}

bool
Table::remove()
{
    /*QSqlQuery query(Aki::Sql::Database::self()->database());
    query.prepare(QString("DROP TABLE %1").arg(metaObject()->className()));
    if (!query.exec()) {
        return false;
    }

    return true;*/
    return true;
}

#include "sql/table.moc"
