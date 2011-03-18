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

#ifndef AKI_KONVERSATIONCONFIGURATION_HPP
#define AKI_KONVERSATIONCONFIGURATION_HPP

#include "aki.hpp"
#include "config/configfile.hpp"

namespace Aki
{
namespace Sql
{
class Database;
class Identity;
} // End of namespace Sql.
class KonversationConfigurationPrivate;
class KonversationConfiguration
    : public Aki::ConfigFile
{
    Q_OBJECT
public:
    explicit KonversationConfiguration(QObject* parent = 0);
    ~KonversationConfiguration();
    bool read();
    void setDatabase(Aki::Sql::Database* database);
    void setIdentity(Aki::Sql::Identity* identity);
private:
    AKI_DECLARE_PRIVATE(Aki::KonversationConfiguration)
}; // End of class KonversationConfiguration.
} // End of namespace Aki.

#endif // AKI_KONVERSATIONCONFIGURATION_HPP
