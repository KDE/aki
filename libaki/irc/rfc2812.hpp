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

#ifndef AKI_IRC_RFC2812_HPP
#define AKI_IRC_RFC2812_HPP

#include "aki.hpp"
#include "irc/message.hpp"

namespace Aki
{
namespace Irc
{
class LIBAKI_EXPORT Rfc2812
{
public:
    static Aki::Irc::Message admin(const QString& target = QString());
    static Aki::Irc::Message away(const QString& message = QString());
    static Aki::Irc::Message connect(const QString& targetServer, const QString& port,
                                     const QString& remoteServer = QString());
    static Aki::Irc::Message ctcpAction(const QString& target, const QString& message);
    static Aki::Irc::Message ctcpClientInfo(const QString& target);
    static Aki::Irc::Message ctcpSource(const QString& target);
    static Aki::Irc::Message ctcpTime(const QString& target);
    static Aki::Irc::Message ctcpUserInfo(const QString& target);
    static Aki::Irc::Message ctcpVersion(const QString& target);
    static Aki::Irc::Message die();
    static Aki::Irc::Message error(const QString& message);
    static Aki::Irc::Message info(const QString& target = QString());
    static Aki::Irc::Message invite(const QString& nick, const QString& channel);
    static Aki::Irc::Message ison(const QString& nick);
    static Aki::Irc::Message ison(const QStringList& nicks);
    static Aki::Irc::Message join(const QString& channel, const QString& key = QString());
    static Aki::Irc::Message join(const QStringList& channels, const QStringList& keys = QStringList());
    static Aki::Irc::Message kick(const QString& channel, const QString& nick,
                                  const QString& comment = QString());
    static Aki::Irc::Message kick(const QStringList& channels, const QString& nick,
                                  const QString& comment = QString());
    static Aki::Irc::Message kick(const QString& channel, const QStringList& nicks,
                                  const QString& comment = QString());
    static Aki::Irc::Message kick(const QStringList& channels, const QStringList& nicks,
                                  const QString& comment = QString());
    static Aki::Irc::Message links(const QString& remoteServer = QString(),
                                   const QString& serverMask = QString());
    static Aki::Irc::Message list(const QString& channel = QString(), const QString& target = QString());
    static Aki::Irc::Message list(const QStringList& channels, const QString& target = QString());
    static Aki::Irc::Message lUsers(const QString& mask = QString(), const QString& target = QString());
    static Aki::Irc::Message mode(const QString& target, const QString& mode = QString());
    static Aki::Irc::Message motd(const QString& target = QString());
    static Aki::Irc::Message names(const QString& channel = QString(), const QString& target = QString());
    static Aki::Irc::Message names(const QStringList& channels, const QString& target = QString());
    static Aki::Irc::Message nick(const QString& nick);
    static Aki::Irc::Message notice(const QString& destination, const QString& message);
    static Aki::Irc::Message oper(const QString& name, const QString& password);
    static Aki::Irc::Message part(const QString& channel, const QString& message = QString());
    static Aki::Irc::Message part(const QStringList& channels, const QString& message = QString());
    static Aki::Irc::Message pass(const QString& password);
    static Aki::Irc::Message ping(const QString& server, const QString& server2 = QString());
    static Aki::Irc::Message pong(const QString& server, const QString& server2 = QString());
    static Aki::Irc::Message privmsg(const QString& destination, const QString& message);
    static Aki::Irc::Message quit(const QString& message = QString());
    static Aki::Irc::Message raw(const QString& command);
    static Aki::Irc::Message rehash();
    static Aki::Irc::Message restart();
    static Aki::Irc::Message service(const QString& nick, const QString& distribution, const QString& info);
    static Aki::Irc::Message servList(const QString& mask = QString(), const QString& type = QString());
    static Aki::Irc::Message sQuit(const QString& server, const QString& comment);
    static Aki::Irc::Message stats(const QString& query = QString(), const QString& target = QString());
    static Aki::Irc::Message summon(const QString& user, const QString& target = QString(),
                                    const QString& channel = QString());
    static Aki::Irc::Message time(const QString& target = QString());
    static Aki::Irc::Message topic(const QString& channel, const QString& topic = QString());
    static Aki::Irc::Message trace(const QString& target = QString());
    static Aki::Irc::Message user(const QString& identity, bool isInvisible, const QString& realName);
    static Aki::Irc::Message userHost(const QString& nick);
    static Aki::Irc::Message userHost(const QStringList& nicks);
    static Aki::Irc::Message users(const QString& target = QString());
    static Aki::Irc::Message version(const QString& target = QString());
    static Aki::Irc::Message wallOps(const QString& text);
    static Aki::Irc::Message who(const QString& mask = QString(), bool ircOp = false);
    static Aki::Irc::Message whoIs(const QString &mask);
    static Aki::Irc::Message whoIs(const QStringList& masks);
    static Aki::Irc::Message whoIs(const QString& target, const QString& mask);
    static Aki::Irc::Message whoIs(const QString& target, const QStringList& masks);
    static Aki::Irc::Message whoWas(const QString& nick, const QString& count = QString(),
                                    const QString& target = QString());
    static Aki::Irc::Message whoWas(const QStringList& nicks, const QString& count = QString(),
                                    const QString& target = QString());
}; // End of class Rfc2812.

inline QString
akiString(const char* str)
{
    return QString(str);
}

template<typename A1>
inline QString
akiString(const char* str, const A1& a1)
{
    return QString(str).arg(a1);
}

template<typename A1, typename A2>
inline QString
akiString(const char* str, const A1& a1, const A2& a2)
{
    return QString(str).arg(a1).arg(a2);
}

template<typename A1, typename A2, typename A3>
inline QString
akiString(const char* str, const A1& a1, const A2& a2, const A3& a3)
{
    return QString(str).arg(a1).arg(a2).arg(a3);
}

template<typename A1, typename A2, typename A3, typename A4>
inline QString
akiString(const char* str, const A1& a1, const A2& a2, const A3& a3, const A4& a4)
{
    return QString(str).arg(a1).arg(a2).arg(a3).arg(a4);
}

template<typename A1, typename A2, typename A3, typename A4, typename A5>
inline QString
akiString(const char* str, const A1& a1, const A2& a2, const A3& a3, const A4& a4,
          const A5& a5)
{
    return QString(str).arg(a1).arg(a2).arg(a3).arg(a4).arg(a5);
}

template<typename A1, typename A2, typename A3, typename A4, typename A5,
         typename A6>
inline QString
akiString(const char* str, const A1& a1, const A2& a2, const A3& a3, const A4& a4,
          const A5& a5, const A5& a6)
{
    return QString(str).arg(a1).arg(a2).arg(a3).arg(a4).arg(a5).arg(a6);
}
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_RFC2812_HPP
