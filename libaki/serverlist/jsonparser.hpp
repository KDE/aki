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

#ifndef AKI_JSONPARSER_HPP
#define AKI_JSONPARSER_HPP

#include "aki.hpp"
#include <QtCore/QObject>

namespace Aki
{
namespace Sql
{
class Database;
class Identity;
} // End of namespace Sql.
class JsonParserPrivate;
class LIBAKI_EXPORT JsonParser
    : public QObject
{
    Q_OBJECT
public:
    JsonParser(QObject* parent = 0);
    ~JsonParser();
    bool read(const QString& file);
    void setDatabase(Aki::Sql::Database* database);
    void setIdentity(Aki::Sql::Identity* identity);
    bool write(const QString& file);
private:
    AKI_DECLARE_PRIVATE(JsonParser)
}; // End of class JsonParser.
} // End of namespace Aki.

#endif // AKI_JSONPARSER_HPP
