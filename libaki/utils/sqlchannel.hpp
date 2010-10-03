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

#ifndef AKI_SQLCHANNEL_HPP
#define AKI_SQLCHANNEL_HPP

#include "aki.hpp"
#include <QtCore/QObject>

namespace Aki
{
class SqlNetwork;
class SqlChannelPrivate;
class LIBAKI_EXPORT SqlChannel : public QObject
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlChannel*> List;
    virtual ~SqlChannel();
    bool operator==(const Aki::SqlChannel& rhs) const;
    bool operator!=(const Aki::SqlChannel& rhs) const;
    static Aki::SqlChannel* newChannel(const QString& name, const Aki::SqlNetwork* server);
    static Aki::SqlChannel::List channelListForServer(const Aki::SqlNetwork* server);
    static Aki::SqlChannel* findChannel(const QString& name, const Aki::SqlNetwork* server);
    void setChannel(const QString& channel);
    QString channel() const;
    void setPassword(const QString& password);
    QString password() const;
    int id() const;
    void setChannelServerId(int id);
    int channelServerId() const;
    bool save();
    bool remove();
protected:
    explicit SqlChannel(QObject* parent = 0);
    void setId(int id);
private:
    AKI_DECLARE_PRIVATE(SqlChannel)
}; // End of class SqlChannel.
} // End of namespace Aki.

#endif // AKI_SQLCHANNEL_HPP
