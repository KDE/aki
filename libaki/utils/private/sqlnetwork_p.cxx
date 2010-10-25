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

#include "sqlnetwork_p.hpp"
#include "aki.hpp"
#include "debughelper.hpp"
using namespace Aki;

SqlNetworkPrivate::SqlNetworkPrivate(SqlNetwork* qq)
    : QObject(),
    server(0),
    id(-1),
    serverId(-1),
    _q(qq)
{
}

void
SqlNetworkPrivate::checkError(const QSqlError& error)
{
    DEBUG_FUNC_NAME;
    if (error.isValid()) {
        DEBUG_TEXT2("Driver: %1", error.driverText());
        DEBUG_TEXT2("Database: %1", error.databaseText());
    }
}

QVariant
SqlNetworkPrivate::checkString(const QString& str)
{
    if (str.isEmpty() || str.isNull()) {
        return QVariant(QVariant::String);
    }
    return str;
}
