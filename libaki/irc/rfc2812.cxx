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

#include "rfc2812.hpp"
using namespace Aki;
using namespace Irc;

Aki::Irc::Message
Rfc2812::admin(const QString& target)
{
    if (target.isEmpty()) {
        return raw("ADMIN");
    } else {
        return raw(akiString("ADMIN %1", target));
    }
}

Aki::Irc::Message
Rfc2812::away(const QString& message)
{
    if (message.isEmpty()) {
        return raw(QString("AWAY"));
    } else {
        return raw(akiString("AWAY :%1", message));
    }
}

Aki::Irc::Message
Rfc2812::connect(const QString& targetServer, const QString& port,
                    const QString& remoteServer)
{
    if (remoteServer.isEmpty()) {
        return raw(akiString("CONNECT %1 %2", targetServer, port));
    } else {
        return raw(akiString("CONNECT %1 %2 %3", targetServer, port, remoteServer));
    }
}

Aki::Irc::Message
Rfc2812::ctcpAction(const QString& target, const QString& message)
{
    return privmsg(target, QString("\x01" "ACTION " + message + "\x01"));
}

Aki::Irc::Message
Rfc2812::ctcpClientInfo(const QString& target)
{
    return privmsg(target, QString("\x01" "CLIENTINFO" "\x01"));
}

Aki::Irc::Message
Rfc2812::ctcpSource(const QString& target)
{
    return privmsg(target, QString("\x01" "SOURCE" "\x01"));
}

Aki::Irc::Message
Rfc2812::ctcpTime(const QString& target)
{
    return privmsg(target, QString("\x01" "TIME" "\x01"));
}

Aki::Irc::Message
Rfc2812::ctcpUserInfo(const QString& target)
{
    return privmsg(target, QString("\x01" "USERINFO" "\x01"));
}

Aki::Irc::Message
Rfc2812::ctcpVersion(const QString& target)
{
    return privmsg(target, QString("\x01" "VERSION" "\x01"));
}

Aki::Irc::Message
Rfc2812::die()
{
    return raw("DIE");
}

Aki::Irc::Message
Rfc2812::error(const QString& message)
{
    return raw(akiString("ERROR :%1", message));
}

Aki::Irc::Message
Rfc2812::info(const QString& target)
{
    if (target.isEmpty()) {
        return raw("INFO");
    } else {
        return raw(akiString("INFO %1", target));
    }
}

Aki::Irc::Message
Rfc2812::invite(const QString& nick, const QString& channel)
{
    return raw(akiString("INVITE %1 %2", nick, channel));
}

Aki::Irc::Message
Rfc2812::ison(const QString& nick)
{
    return raw(akiString("ISON %1", nick));
}

Aki::Irc::Message
Rfc2812::ison(const QStringList& nicks)
{
    return ison(nicks.join(","));
}

Aki::Irc::Message
Rfc2812::join(const QString& channel, const QString& key)
{
    if (key.isEmpty()) {
        return raw(akiString("JOIN %1", channel));
    } else {
        return raw(akiString("JOIN %1 %2", channel, key));
    }
}

Aki::Irc::Message
Rfc2812::join(const QStringList& channels, const QStringList& keys)
{
    if (keys.isEmpty()) {
        return join(channels.join(","));
    } else {
        return join(channels.join(","), keys.join(","));
    }
}

Aki::Irc::Message
Rfc2812::kick(const QString& channel, const QString& nick, const QString& comment)
{
    if (comment.isEmpty()) {
        return raw(akiString("KICK %1 %2", channel, nick));
    } else {
        return raw(akiString("KICK %1 %2 :%3", channel, nick, comment));
    }
}

Aki::Irc::Message
Rfc2812::kick(const QStringList& channels, const QString& nick, const QString& comment)
{
    return kick(channels.join(","), nick, comment);
}

Aki::Irc::Message
Rfc2812::kick(const QString& channel, const QStringList& nicks, const QString& comment)
{
    return kick(channel, nicks.join(","), comment);
}

Aki::Irc::Message
Rfc2812::kick(const QStringList& channels, const QStringList& nicks, const QString& comment)
{
    return kick(channels.join(","), nicks.join(","), comment);
}

Aki::Irc::Message
Rfc2812::links(const QString& remoteServer, const QString& serverMask)
{
    if (remoteServer.isEmpty() and serverMask.isEmpty()) {
        return raw("LINKS");
    } else if (serverMask.isEmpty()) {
        return raw(akiString("LINKS %1", remoteServer));
    } else {
        return raw(akiString("LINKS %1 %2", remoteServer, serverMask));
    }
}

Aki::Irc::Message
Rfc2812::list(const QString& channel, const QString& target)
{
    if (channel.isEmpty() and target.isEmpty()) {
        return raw("LIST");
    } else if (target.isEmpty()) {
        return raw(akiString("LIST %1", channel));
    } else {
        return raw(akiString("LIST %1 %2", channel, target));
    }
}

Aki::Irc::Message
Rfc2812::list(const QStringList& channels, const QString& target)
{
    return list(channels.join(","), target);
}

Aki::Irc::Message
Rfc2812::lUsers(const QString& mask, const QString& target)
{
    if (mask.isEmpty() and target.isEmpty()) {
        return raw("LUSERS");
    } else if (target.isEmpty()) {
        return raw(akiString("LUSERS %1", mask));
    } else {
        return raw(akiString("LUSERS %1 %2", mask, target));
    }
}

Aki::Irc::Message
Rfc2812::mode(const QString& target, const QString& mode)
{
    if (target.isEmpty()) {
        return raw(akiString("MODE %1", target));
    } else {
        return raw(akiString("MODE %1 %2", target, mode));
    }
}

Aki::Irc::Message
Rfc2812::motd(const QString& target)
{
    if (target.isEmpty()) {
        return raw("MOTD");
    } else {
        return raw(akiString("MOTD %1", target));
    }
}

Aki::Irc::Message
Rfc2812::names(const QString& channel, const QString& target)
{
    if (channel.isEmpty() and target.isEmpty()) {
        return raw("NAMES");
    } else if (target.isEmpty()) {
        return raw(akiString("NAMES %1", channel));
    } else {
        return raw(akiString("NAMES %1 %2", channel, target));
    }
}

Aki::Irc::Message
Rfc2812::names(const QStringList& channels, const QString& target)
{
    return names(channels.join(","), target);
}

Aki::Irc::Message
Rfc2812::nick(const QString& nick)
{
    return raw(akiString("NICK %1", nick));
}

Aki::Irc::Message
Rfc2812::notice(const QString& destination, const QString& message)
{
    return raw(akiString("NOTICE %1 :%2", destination, message));
}

Aki::Irc::Message
Rfc2812::oper(const QString& name, const QString& password)
{
    return raw(akiString("OPER %1 %2", name, password));
}

Aki::Irc::Message
Rfc2812::part(const QString& channel, const QString& message)
{
    if (message.isEmpty()) {
        return raw(akiString("PART %1", channel));
    } else {
        return raw(akiString("PART %1 %2", channel, message));
    }
}

Aki::Irc::Message
Rfc2812::part(const QStringList& channels, const QString& message)
{
    return part(channels.join(","), message);
}

Aki::Irc::Message
Rfc2812::pass(const QString& password)
{
    return raw(akiString("PASS %1", password));
}

Aki::Irc::Message
Rfc2812::ping(const QString& server, const QString& server2)
{
    if (server2.isEmpty()) {
        return raw(akiString("PING %1", server));
    } else {
        return raw(akiString("PING %1 %2", server, server2));
    }
}

Aki::Irc::Message
Rfc2812::pong(const QString& server, const QString& server2)
{
    if (server2.isEmpty()) {
        return raw(akiString("PONG %1", server));
    } else {
        return raw(akiString("PONG %1 %2", server, server2));
    }
}

Aki::Irc::Message
Rfc2812::privmsg(const QString& destination, const QString& message)
{
    return raw(akiString("PRIVMSG %1 :%2", destination, message));
}

Aki::Irc::Message
Rfc2812::quit(const QString& message)
{
    if (message.isEmpty()) {
        return raw("QUIT");
    } else {
        return raw(akiString("QUIT :%1", message));
    }
}

Aki::Irc::Message
Rfc2812::raw(const QString& command)
{
    Aki::Irc::Message message;
    message.setPlainText(command);
    return message;
}

Aki::Irc::Message
Rfc2812::rehash()
{
    return raw("REHASH");
}

Aki::Irc::Message
Rfc2812::restart()
{
    return raw("RESTART");
}

Aki::Irc::Message
Rfc2812::service(const QString& nick, const QString& distribution, const QString& info)
{
    return raw(akiString("SERVICE %1 * %2 0 0 :%3", nick, distribution, info));
}

Aki::Irc::Message
Rfc2812::servList(const QString& mask, const QString& type)
{
    if (mask.isEmpty() and type.isEmpty()) {
        return raw("SERVLIST");
    } else if (type.isEmpty()) {
        return raw(akiString("SERVLIST %1", mask));
    } else {
        return raw(akiString("SERVLIST %1 %2", mask, type));
    }
}

Aki::Irc::Message
Rfc2812::sQuit(const QString& server, const QString& comment)
{
    return raw(akiString("SQUIT %1 :%2", server, comment));
}

Aki::Irc::Message
Rfc2812::stats(const QString& query, const QString& target)
{
    if (query.isEmpty() and target.isEmpty()) {
        return raw("STATS");
    } else if (target.isEmpty()) {
        return raw(akiString("STATS %1", query));
    } else {
        return raw(akiString("STATS %1 :%2", query, target));
    }
}

Aki::Irc::Message
Rfc2812::summon(const QString& user, const QString& target, const QString& channel)
{
    if (target.isEmpty() and channel.isEmpty()) {
        return raw(akiString("SUMMON %1", user));
    } else if (channel.isEmpty()) {
        return raw(akiString("SUMMON %1", user, target));
    } else {
        return raw(akiString("SUMMON %1 %2 %3", user, target, channel));
    }
}

Aki::Irc::Message
Rfc2812::time(const QString& target)
{
    if (target.isEmpty()) {
        return raw("TIME");
    } else {
        return raw(akiString("TIME %1", target));
    }
}

Aki::Irc::Message
Rfc2812::topic(const QString& channel, const QString& topic)
{
    if (topic.isEmpty()) {
        return raw(akiString("TOPIC %1", channel));
    } else if (topic.isEmpty() and not topic.isNull()) {
        return raw(akiString("TOPIC %1 : ", channel));
    } else {
        return raw(akiString("TOPIC %1 :%2", channel, topic));
    }
}

Aki::Irc::Message
Rfc2812::trace(const QString& target)
{
    if (target.isEmpty()) {
        return raw("TRACE");
    } else {
        return raw(akiString("TRACE %1", target));
    }
}

Aki::Irc::Message
Rfc2812::user(const QString& identity, bool isInvisible, const QString& realName)
{
    return raw(akiString("USER %1 %2 * :%3", identity, isInvisible ? "8" : "0", realName));
}

Aki::Irc::Message
Rfc2812::userHost(const QString& nick)
{
    return raw(akiString("USERHOST %1", nick));
}

Aki::Irc::Message
Rfc2812::userHost(const QStringList& nicks)
{
    return userHost(nicks.join(","));
}

Aki::Irc::Message
Rfc2812::users(const QString& target)
{
    if (target.isEmpty()) {
        return raw("USERS");
    } else {
        return raw(akiString("USERS %1", target));
    }
}

Aki::Irc::Message
Rfc2812::version(const QString& target)
{
    if (target.isEmpty()) {
        return raw("VERSION");
    } else {
        return raw(akiString("VERSION %1", target));
    }
}

Aki::Irc::Message
Rfc2812::wallOps(const QString& text)
{
    return raw(akiString("WALLOPS %1", text));
}

Aki::Irc::Message
Rfc2812::who(const QString& mask, bool ircOp)
{
    if (mask.isEmpty() and not ircOp) {
        return raw("WHO");
    } else if (not ircOp) {
        return raw(akiString("WHO %1", mask));
    } else {
        return raw(akiString("WHO %1 o", mask));
    }
}

Aki::Irc::Message
Rfc2812::whoIs(const QString& mask)
{
    return raw(akiString("WHOIS %1", mask));
}

Aki::Irc::Message
Rfc2812::whoIs(const QStringList& masks)
{
    return whoIs(masks.join(","));
}

Aki::Irc::Message
Rfc2812::whoIs(const QString& target, const QString& mask)
{
    return raw(akiString("WHOIS %1 %2", target, mask));
}

Aki::Irc::Message
Rfc2812::whoIs(const QString& target, const QStringList& masks)
{
    return whoIs(target, masks.join(","));
}

Aki::Irc::Message
Rfc2812::whoWas(const QString& nick, const QString& count, const QString& target)
{
    if (count.isEmpty() and target.isEmpty()) {
        return raw(akiString("WHOWAS %1", nick));
    } else if (target.isEmpty()) {
        return raw(akiString("WHOWAS %1 %2", nick, count));
    } else {
        return raw(akiString("WHOWAS %1 %2 %3", nick, count, target));
    }
}

Aki::Irc::Message
Rfc2812::whoWas(const QStringList& nicks, const QString& count, const QString& target)
{
    return whoWas(nicks.join(","), count, target);
}
