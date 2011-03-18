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

#ifndef AKI_KONVERSATIONCONFIGURATION_P_H
#define AKI_KONVERSATIONCONFIGURATION_P_H

#include "aki.hpp"
#include "konversationconfiguration.hpp"
#include "sql/database.hpp"
#include "sql/identity.hpp"

namespace Aki
{
class KonversationConfigurationPrivate
{
public:
    explicit KonversationConfigurationPrivate(Aki::KonversationConfiguration* qq);
    int channelCount() const;
    int identityCount() const;
    QStringList isAutoJoinChannels() const;
    bool isAutoReconnect() const;
    bool isSslEnabled() const;
    QString name() const;
    QString password() const;
    int port() const;
    QString server() const;
    QStringList serverList() const;
    int serverCount() const;
    int serverGroupCount() const;
public:
    Aki::Sql::Database* database;
private:
    AKI_DECLARE_PUBLIC(Aki::KonversationConfiguration)
}; // End of class KonversationConfigurationPrivate.
} // End of namespace Aki.

#endif // AKI_KONVERSATIONCONFIGURATION_P_H
