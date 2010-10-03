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

#ifndef AKI_SERVER_HPP
#define AKI_SERVER_HPP

#include "aki.hpp"
#include <QtCore/QMap>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>
#include <boost/tuple/tuple.hpp>

namespace Aki
{
typedef boost::tuple<QString,QString> Channel;
typedef boost::tuple<QString,quint16,QString,bool> Address;
typedef QList<Aki::Channel> ChannelList;
typedef QList<Aki::Address> AddressList;

class ServerPrivate;
class LIBAKI_EXPORT Server
{
public:
    typedef QSharedPointer<Aki::Server> Ptr;
    typedef QList<Aki::Server::Ptr> List;

    Server();
    explicit Server(const QString& name);
    ~Server();
    Aki::AddressList addressList() const;
    Aki::ChannelList channelList() const;
    QByteArray encoding() const;
    bool isAutoIdentifyEnabled() const;
    bool isAutoJoinChannelsEnabled() const;
    bool isAutoReconnectionEnabled() const;
    bool isConnectOnStartupEnabled() const;
    bool isConnectToRandomServerEnabled() const;
    bool isDefaultEncodingEnabled() const;
    bool isSslEnabled() const;
    QString name() const;
    int retryAttemptCount() const;
    int retryInterval() const;
    QString serviceName() const;
    QString servicePassword() const;
    void setAddressList(const Aki::AddressList& addresses);
    void setAutoIdentify(bool enabled);
    void setAutoJoinChannels(bool enabled);
    void setAutoReconnection(bool enabled);
    void setChannelList(const Aki::ChannelList& channels);
    void setConnectOnStartup(bool enabled);
    void setConnectToRandomServer(bool enabled);
    void setDefaultEncoding(bool enabled);
    void setEncoding(const QByteArray& encoding);
    void setName(const QString& name);
    void setRetryAttempts(int count);
    void setRetryInterval(int seconds);
    void setServiceName(const QString& name);
    void setServicePassword(const QString& password);
    void setSsl(bool enable);
private:
    AKI_DECLARE_PRIVATE(Server)
}; // End of class Server.
} // End of namespace Aki.

Q_DECLARE_METATYPE(Aki::Server*)
Q_DECLARE_METATYPE(Aki::Channel)
Q_DECLARE_METATYPE(Aki::Address)
Q_DECLARE_METATYPE(Aki::ChannelList)
Q_DECLARE_METATYPE(Aki::AddressList)

#endif // AKI_SERVER_HPP
