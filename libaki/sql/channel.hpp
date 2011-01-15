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

#ifndef AKI_SQL_CHANNEL_HPP
#define AKI_SQL_CHANNEL_HPP

#include "aki.hpp"
#include "sql/database.hpp"
#include "sql/table.hpp"

namespace Aki
{
namespace Sql
{
class ChannelPrivate;
class LIBAKI_EXPORT Channel
    : public Aki::Sql::Table
{
    Q_OBJECT
    Q_CLASSINFO("id","PRIMARY_KEY,AUTO_INCREMENT")
    Q_CLASSINFO("name","LENGTH=0,NULL=false")
    Q_CLASSINFO("password","LENGTH=0,NULL=true")
    Q_CLASSINFO("channelServer","FOREIGN_KEY=Server;id,NULL=false")
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(int channelServer READ channelServer WRITE setChannelServer)
public:
    Q_INVOKABLE explicit Channel(QObject* parent = 0);
    ~Channel();
    int channelServer() const;
    int id() const;
    QString name() const;
    QString password() const;
    void setChannelServer(int id);
    void setId(int id);
    void setName(const QString& name);
    void setPassword(const QString& password);
private:
    AKI_DECLARE_PRIVATE(Channel)
}; // End of class Channel.
} // End of namespace Sql.
} // End of namespace Aki.

//AKI_REGISTER_SQL_TABLE(Aki::Sql::Channel, Channel)

#endif // AKI_SQL_CHANNEL_HPP
