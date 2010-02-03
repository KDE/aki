/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "chatparser.h"
#include "config/identityconfig.h"
#include "config/replaceconfig.h"
#include "logfile.h"
#include "ui/basewindow.h"
#include "ui/channelwindow.h"
#include "ui/chatview.h"
#include "ui/querywindow.h"
#include <Aki/Irc/Color>
#include <Aki/Irc/Rfc2812>
#include <Aki/Irc/Socket>
#include <Aki/Irc/User>
#include <KDebug>
#include <KLocale>
#include <QTextCodec>
using namespace Aki;

namespace Aki
{
class ChatParserPrivate
{
public:
    ChatParserPrivate(ChatParser *qq)
        : q(qq)
    {
    }

    /**
     * Removes a part of the string to the first whitespace and returns it.
     * @return String to first whitespace if a space is found; else it returns
     * the whole string. If no string in @p message it returns an null string.
     * @param message Pointer to the message.
     * @param hasWhitespace true if the string had whitespace; false otherwise
     */
    QString removeToSpace(QString *message, bool *hasWhitespace)
    {
        if (message->isEmpty() && message->isNull()) {
            *hasWhitespace = false;
            return QString();
        }

        if (message->contains(' ')) {
            QString msg = message->left(message->indexOf(' '));
            message->remove(0, msg.length() + 1);
            *hasWhitespace = true;
            return msg;
        } else {
            *hasWhitespace = false;
            return *message;
        }
    }

    QStringList checkMessageLength(const QString &type, const QString &destination,
                                               const QString &message, Aki::Irc::User *user)
    {
        const int nickLength = user->nick().length();
        const int hostMaskLength = user->hostMask().length();
        const int maxLength = 512 - 8 - (nickLength + hostMaskLength + type.length() +
                                         destination.length());

        QTextCodec *codec = window->socket()->codec();
        QTextEncoder *encoder = codec->makeEncoder();
        QByteArray process = encoder->fromUnicode(message);
        QString line = message;
        QStringList split;

        while (process.length() > maxLength - 2) {
            QByteArray tmp = process.left(maxLength - 2);
            QString unconverted = codec->makeDecoder()->toUnicode(tmp);
            split.append(unconverted);
            process.remove(0, maxLength - 2);
            line.remove(unconverted);
        }

        if (!process.isEmpty()) {
            split.append(line);
        }

        return split;
    }

    void parseAway(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            window->socket()->rfcAway(message);
        } else {
            Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
            if (channel->identity()->isAwayMessagesEnabled()) {
                channel->socket()->rfcAway(channel->identity()->awayMessage());
            } else {
                channel->socket()->rfcAway();
            }
        }
    }

    void parseCtcp(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            if (msg.contains(' ')) {
                QString nick = msg.left(msg.indexOf(' '));
                msg.remove(0, nick.length() + 1);

                if (!msg.isEmpty()) {
                    window->socket()->rfcPrivmsg(nick, QString("\x01%1\x01").arg(msg.toUpper()));
                } else {
                    if (window->view()) {
                        window->view()->addHelp(i18n("/ctcp <nick> <message>, sends message to nick, common ones"
                                                     " are USERINFO and VERSION"));
                    }
                }
            } else {
                if (window->view()) {
                    window->view()->addHelp(i18n("/ctcp <nick> <message>, sends message to nick, common ones"
                                                 " are USERINFO and VERSION"));
                }
            }
        } else {
            if (window->view()) {
                window->view()->addHelp(i18n("/ctcp <nick> <message>, sends message to nick, common ones"
                                             " are USERINFO and VERSION"));
            }
        }
    }

    void parseGhost(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            QString nick = msg.left(msg.indexOf(' '));
            msg.remove(0, nick.length() + 1);

            if (!msg.isEmpty()) {
                QString password = msg;
                window->socket()->rfcPrivmsg("nickserv", "GHOST " + nick + ' ' + password);
            } else {
                if (window->view()) {
                    window->view()->addHelp(i18n("/ghost <nick> <password>, kill a ghost nick."));
                }
            }
        } else {
            if (window->view()) {
                window->view()->addHelp(i18n("/ghost <nick> <password>, kill a ghost nick."));
            }
        }
    }

    void parseInvite(const QString &message)
    {
        QString msg = message;

        if (window->windowType() == Aki::BaseWindow::StatusWindow ||
            window->windowType() == Aki::BaseWindow::QueryWindow) {
            if (window->view()) {
                window->view()->addHelp(i18n("Invite requires you to be on a channel"));
                return;
            }
        }

        if (!msg.isEmpty()) {
            if (msg.contains(' ')) {
                window->socket()->rfcInvite(msg, window->name());
            } else {
            }
        } else {
            if (window->view()) {
                window->view()->addHelp(i18n("/invite <nick> [<channel>], invites nick to a channel, by default"
                                             " the current one. Requires channel operator status"));
            }
        }
    }

    void parseIsOn(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            window->socket()->rfcIson(msg);
        } else {
            if (window->view()) {
                window->view()->addHelp(i18n("/ison <nick> *(' ' <nick>). Checks online status of nick"));
            }
        }
    }

    void parseJoin(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            if (msg.contains(' ')) {
                QString channel = msg.left(msg.indexOf(' '));
                msg.remove(0, channel.length() + 1);

                QString keys = msg;
                window->socket()->rfcJoin(channel, keys);
            } else {
                window->socket()->rfcJoin(msg);
            }
        } else {
            if (window->view()) {
                window->view()->addHelp(i18n("/join <channel> *(',' <channel>) [<key> *(',' <key>)], "
                                             "joins the specified channel. By default requires no channel key."));
            }
        }
    }

    void parseKick(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            if (msg.contains(' ')) {
                QString nick = msg.left(msg.indexOf(' '));
                msg.remove(0, nick.length() + 1);

                QString message = msg;
                window->socket()->rfcKick(window->name(), nick, message);
            } else {
                Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
                channel->socket()->rfcKick(channel->name(), msg, channel->identity()->kickMessage());
            }
        } else {
            if (window->view()) {
                window->view()->addHelp(i18n("/kick <nick> [<message>], kicks a user from the channel."
                                             " Optional kick message. Requires channel operator status"));
            }
        }
    }

    void parseList(const QString &message)
    {
        QString msg = message;
    }

    void parseLUsers(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            window->socket()->rfcLUsers(msg);
        } else {
            window->socket()->rfcLUsers();
        }
    }

    void parseMe(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            window->socket()->rfcCtcpAction(window->name(), msg);
            window->view()->addCtcpAction(window->socket()->currentNick(),
                                          Aki::Irc::Color::toHtml(msg));
        } else {
            if (window->view()) {
                window->view()->addHelp(i18n("/me <message>, talks in 3rd person view to the channel in an RPG"
                                             " style"));
            }
        }
    }

    void parseMessage(const QString &message)
    {
        QString msg = message;

        if (msg.isEmpty()) {
            return;
        }

        Aki::ReplaceConfig *replace = new Aki::ReplaceConfig(q);
        QList<Aki::ReplaceConfig::ReplaceItem> items = replace->wordList();

        foreach (const Aki::ReplaceConfig::ReplaceItem &item, items) {
            if (item.replacementMethod.toLower() == "both" ||
                item.replacementMethod.toLower() == "outcoming") {
                if (item.regex) {
                    msg.replace(QRegExp(item.text), item.replacementText);
                } else {
                    msg.replace(item.text, item.replacementText, Qt::CaseSensitive);
                }
            }
        }

        if (window->windowType() == Aki::BaseWindow::ChannelWindow) {
            Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
            foreach (Aki::Irc::User *user, channel->users()) {
                if (user->nick() == channel->socket()->currentNick()) {
                    QString nickColour = QString("<font color='%1'>%2</font>")
                                            .arg(user->color().name(), user->nick());
                    QStringList messages = checkMessageLength("PRIVMSG", channel->name(), msg, user);
                    QStringListIterator iter(messages);

                    while (iter.hasNext()) {
                        QString tmp = iter.next();
                        channel->view()->addPrivmsg(nickColour, Aki::Irc::Color::toHtml(tmp));
                        channel->socket()->rfcPrivmsg(channel->name(), tmp);
                    }
                }
            }
        } else if (window->windowType() == Aki::BaseWindow::QueryWindow) {
            Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);

            QString nickColour = QString("<font color='%1'>%2</font>")
                                    .arg(query->selfUser()->color().name(),
                                         query->selfUser()->nick());
            QStringList messages = checkMessageLength("PRIVMSG", query->name(), msg, query->otherUser());

            QStringListIterator iter(messages);
            while (iter.hasNext()) {
                QString tmp = iter.next();
                query->view()->addPrivmsg(nickColour, Aki::Irc::Color::toHtml(tmp));
                query->socket()->rfcPrivmsg(query->name(), tmp);
            }
        }
    }

    void parseMode(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            if (msg.contains(' ')) {
                QString channel = msg.left(msg.indexOf(' '));
                msg.remove(0, channel.length() + 1);

                if (Aki::Irc::Rfc2812::isValidChannel(channel)) {
                    if (!msg.isEmpty()) {
                        window->socket()->rfcMode(channel, msg);
                    } else {
                        window->socket()->rfcMode(channel);
                    }
                } else {
                    window->socket()->rfcMode(window->name(), channel + msg);
                }
            } else {
                window->socket()->rfcMode(window->name(), msg);
            }
        } else {
            window->socket()->rfcMode(window->name());
        }
    }

    void parseNick(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            window->socket()->rfcNick(msg);
        } else {
            if (window->view()) {
                window->view()->addHelp(i18n("/nick <nick>, changes your current nick"));
            }
        }
    }

    void parsePrivmsg(const QString &message)
    {
        QString msg = message;

        if (msg.isEmpty()) {
            window->socket()->rfcPrivmsg("", msg);
            return;
        }

        Aki::ReplaceConfig *replace = new Aki::ReplaceConfig(q);
        QList<Aki::ReplaceConfig::ReplaceItem> items = replace->wordList();

        foreach (const Aki::ReplaceConfig::ReplaceItem &item, items) {
            if (item.replacementMethod.toLower() == "both" ||
                item.replacementMethod.toLower() == "outcoming") {
                if (item.regex) {
                    msg.replace(QRegExp(item.text), item.replacementText);
                } else {
                    msg.replace(item.text, item.replacementText, Qt::CaseSensitive);
                }
            }
        }

        QString dest = msg.left(msg.indexOf(' '));
        msg.remove(0, dest.length() + 1);

        if (!msg.isEmpty()) {
            if (dest.toLower() == window->name()) {
                if (window->windowType() == Aki::BaseWindow::ChannelWindow) {
                    Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
                    foreach (Aki::Irc::User *user, channel->users()) {
                        if (user->nick() == channel->socket()->currentNick()) {
                            QString nickColour = QString("<font color='%1'>%2</font>")
                                                    .arg(user->color().name(), user->nick());
                            QStringList messages = checkMessageLength("PRIVMSG", dest, msg, user);

                            QStringListIterator iter(messages);
                            while (iter.hasNext()) {
                                QString tmp = iter.next();
                                channel->socket()->rfcPrivmsg(dest, tmp);
                                channel->view()->addPrivmsg(nickColour, Aki::Irc::Color::toHtml(tmp));
                            }
                        }
                    }
                } else if (window->windowType() == Aki::BaseWindow::QueryWindow) {
                    Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);

                    QString nickColour = QString("<font color='%1'>%2</font>")
                                            .arg(query->selfUser()->color().name(),
                                                 query->selfUser()->nick());
                    QStringList messages = checkMessageLength("PRIVMSG", dest, msg, query->otherUser());

                    QStringListIterator iter(messages);
                    while (iter.hasNext()) {
                        QString tmp = iter.next();
                        query->socket()->rfcPrivmsg(dest, tmp);
                        query->view()->addPrivmsg(nickColour, Aki::Irc::Color::toHtml(tmp));
                    }
                }
            } else if (Aki::Irc::Rfc2812::isValidNickname(dest)) {
                emit q->queryMessage(dest, msg);
            } else if (Aki::Irc::Rfc2812::isValidChannel(dest)) {
#ifdef Q_CC_GNU
#warning Implement message to channel when not on channel
#endif
            }
        }
    }

    void parseServer(const QString &message)
    {
        QString msg = message;
        if (message.isEmpty() && message.isNull()) {
            return;
        }

        bool ssl = false;
        bool hasWhitespace = false;
        bool ok = false;
        quint16 port = 6667;
        QString host;
        QString password;

        QString tmp = removeToSpace(&msg, &hasWhitespace);
        if (tmp == "-ssl") {
            ssl = true;
        } else {
            host = tmp;
        }

        if (!hasWhitespace) {
            emit q->newServerRequest(window->socket()->currentNick(), host, port, ssl, password);
            return;
        }

        tmp = removeToSpace(&msg, &hasWhitespace);
        if (ssl) {
            host = tmp;
        } else {
            tmp.toUShort(&ok);
            if (ok) {
                port = tmp.toUShort();
            } else {
                password = tmp;
            }
        }

        if (!hasWhitespace) {
            emit q->newServerRequest(window->socket()->currentNick(), host, port, ssl, password);
            return;
        }

        tmp = removeToSpace(&msg, &hasWhitespace);
        if (ssl) {
            tmp.toUShort(&ok);
            if (ok) {
                port = tmp.toUShort();
            } else {
                password = tmp;
            }
        }

        if (!hasWhitespace) {
            emit q->newServerRequest(window->socket()->currentNick(), host, port, ssl, password);
            return;
        }

        tmp = removeToSpace(&msg, &hasWhitespace);
        if (ssl) {
            password = tmp;
        }
    }

    void parseTopic(const QString &message)
    {
        QString msg = message;
        if (window->windowType() != Aki::BaseWindow::ChannelWindow) {
            if (window->view()) {
                window->view()->addHelp(i18n("TOPIC requires you to be on a channel. Channel operator status maybe"
                                             " required"));
            }
            return;
        }

        if (!msg.isEmpty()) {
            window->socket()->rfcTopic(window->name(), msg);
        } else {
            window->socket()->rfcTopic(window->name());
        }
    }

    void parseUMode(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            window->socket()->rfcMode(window->socket()->currentNick(), msg);
        } else {
            window->socket()->rfcMode(window->socket()->currentNick());
        }
    }

    void parseUserHost(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            window->socket()->rfcUserHost(message);
        } else {
            if (window->view()) {
                window->view()->addHelp(i18n("/ison <nick> *(' ' <nick>). Checks userhost of nick"));
            }
        }
    }

    void parseWho(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            emit q->whoRequest(msg);
        } else {
            emit q->whoRequest(window->name());
        }
    }

    void parseWhoIs(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            if (msg.contains(' ')) {
                QString target = msg.left(msg.indexOf(' '));
                msg.remove(0, target.length() + 1);

                window->socket()->rfcWhoIs(target, msg);
            } else {
                window->socket()->rfcWhoIs(msg);
            }
        } else {
            window->socket()->rfcWhoIs(QString());
        }
    }

    void parseWhoWas(const QString &message)
    {
        QString msg = message;

        if (!msg.isEmpty()) {
            if (msg.contains(' ')) {
                QString nick = msg.left(msg.indexOf(' '));
                msg.remove(0, nick.length() + 1);

                if (!msg.isEmpty()) {
                    window->socket()->rfcWhoWas(nick, msg);
                } else {
                    window->socket()->rfcWhoWas(nick);
                }
            } else {
                window->socket()->rfcWhoWas(msg);
            }
        } else {
            window->socket()->rfcWhoWas(QString());
        }
    }

    ChatParser *q;
    Aki::BaseWindow *window;
}; // End of class ChatParserPrivate.
} // End of namespace Aki.

ChatParser::ChatParser(QObject *parent)
    : QObject(parent)
{
    d.reset(new Aki::ChatParserPrivate(this));
}

ChatParser::~ChatParser()
{
}

void
ChatParser::parse(const QString &data)
{
    QString message = data;
    QString command;

    if (message.startsWith(QChar('/'))) {
        message.remove(0, 1);

        if (message.isEmpty()) {
            return;
        }

        // Check and see if this is a single command. Because most
        // IRC commands are single while others are not.
        // This makes a easier check so nothing bad will go wrong
        // We should always expect the worst and prepare for it.
        if (message.contains(' ')) {
            command = message.left(message.indexOf(' ')).toLower();
            message.remove(0, command.length() + 1);
        } else {
            command = message.toLower();
            message.clear();
        }

        // Trim useless whitespace
        message = message.trimmed();

        if (command == "away") {
            d->parseAway(message);
        } else if (command == "ban") {
            if (d->window->windowType() != Aki::BaseWindow::ChannelWindow) {
                if (d->window->view()) {
                    d->window->view()->addHelp(i18n("Ban requires you to be on a channel"));
                    return;
                }
            }
            d->window->socket()->rfcMode(d->window->name(), "+b");
        } else if (command == "chanserv") {
            d->window->socket()->rfcPrivmsg("chanserv", message);
        } else if (command == "clear") {
            if (d->window->view()) {
                d->window->view()->clear();
            }
        } else if (command == "ctcp") {
            d->parseCtcp(message);
        } else if (command == "cs") {
            d->window->socket()->rfcPrivmsg("chanserv", message);
        } else if (command == "ghost") {
            d->parseGhost(message);
        } else if (command == "invite") {
            d->parseInvite(message);
        } else if (command == "ison") {
            d->parseIsOn(message);
        } else if (command == "j") {
            d->parseJoin(message);
        } else if (command == "join") {
            d->parseJoin(message);
        } else if (command == "kick") {
            d->parseKick(message);
        } else if (command == "list") {
            d->parseList(message);
        } else if (command == "lusers") {
            d->parseLUsers(message);
        } else if (command == "me") {
            d->parseMe(message);
        } else if (command == "mode") {
            d->parseMode(message);
        } else if (command == "msg") {
            d->parsePrivmsg(message);
        } else if (command == "nick") {
            d->parseNick(message);
        } else if (command == "nickserv") {
            d->window->socket()->rfcPrivmsg("nickserv", message);
        } else if (command == "ns") {
            d->window->socket()->rfcPrivmsg("nickserv", message);
        } else if (command == "privmsg") {
            d->parsePrivmsg(message);
        } else if (command == "server") {
            d->parseServer(message);
        } else if (command == "time") {
            d->window->socket()->rfcTime();
        } else if (command == "topic") {
            d->parseTopic(message);
        } else if (command == "umode") {
            d->parseUMode(message);
        } else if (command == "unaway") {
            d->window->socket()->rfcAway();
        } else if (command == "userhost") {
            d->parseUserHost(message);
        } else if (command == "version") {
            d->window->socket()->rfcVersion();
        } else if (command == "who") {
            d->parseWho(message);
        } else if (command == "whois") {
            d->parseWhoIs(message);
        } else if (command == "whowas") {
            d->parseWhoWas(message);
        } else {
            emit customCommand(command, message);
        }
        message.clear();
    } else {
        d->parseMessage(message);
    }
}

void
ChatParser::setWindow(Aki::BaseWindow *window)
{
    d->window = window;
}

Aki::BaseWindow*
ChatParser::window()
{
    return d->window;
}

#include "chatparser.moc"
