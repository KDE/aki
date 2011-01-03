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

#include "socket_p.hpp"
#include "debughelper.hpp"
#include "irc/rfc2812.hpp"
#include <QtCore/QTimer>
using namespace Aki;
using namespace Irc;

SocketPrivate::SocketPrivate(Aki::Irc::Socket* qq)
    : isMotdEnabled(true),
    isSaslEnabled(false),
    _q(qq)
{
}

void
SocketPrivate::commandReceived(const Aki::Irc::ReplyInfo& message)
{
    switch (message.numeric()) {
    case RPL_NULL: {
        break;
    }
    case RPL_WELCOME:
    case RPL_YOURHOST:
    case RPL_CREATED:
    case RPL_MYINFO: {
        emit _q->onStartupReply(Aki::Irc::StartupReply(message));
        break;
    }
    case RPL_ISUPPORT: {
        emit _q->onStartupReply(Aki::Irc::StartupReply(message));
        // We skip the first one because that is the user's nickname.
        // We skip the last one because that is a message to the user which has no use in the capabilities.
        // I'm not sure if all the network's have that so I have to do something different if there is.
        // But I highly doubt it will be different but then again you can't trust input :(
        for (int i = 1, count = (message.params().count() - 1); i < count; ++i) {
            const QString cap = message.params().at(i);
            // Capabilities are done like <cap>=<option> or just <cap>
            if (cap.contains('=')) {
                const QStringList split = cap.split('=');
                serverCapabilities.insert(split.at(0), split.at(1));
            } else {
                serverCapabilities.insert(cap, QString());
            }
        }
        break;
    }
    case RPL_SNOMASK: {
        break;
    }
    case RPL_REDIR: {
        break;
    }
    case RPL_MAP: {
        break;
    }
    case RPL_MAPMORE: {
        break;
    }
    case RPL_MAPEND: {
        break;
    }
    case RPL_SAVENICK: {
        break;
    }
    case RPL_TRACELINK:
    case RPL_TRACECONNECTING:
    case RPL_TRACEHANDSHAKE:
    case RPL_TRACEUNKNOWN:
    case RPL_TRACEOPERATOR:
    case RPL_TRACEUSER:
    case RPL_TRACESERVER:
    case RPL_TRACENEWTYPE:
    case RPL_TRACECLASS: {
        //emit _q->onTraceMessage(message);
        break;
    }
    case RPL_STATSLINKINFO:
    case RPL_STATSCOMMANDS:
    case RPL_STATSCLINE:
    case RPL_STATSNLINE:
    case RPL_STATSILINE:
    case RPL_STATSKLINE:
    case RPL_STATSQLINE:
    case RPL_STATSYLINE:
    case RPL_ENDOFSTATS:
    case RPL_STATSPLINE: {
        //emit _q->onStatsMessage(message);
        break;
    }
    case RPL_UMODEIS: {
        break;
    }
    case RPL_STATSFLINE:
    case RPL_STATSDLINE: {
        //emit _q->onStatsMessage(message);
        break;
    }
    case RPL_SERVLIST:
    case RPL_SERVLISTEND: {
        //emit _q->onServerListMessage(message);
        break;
    }
    case RPL_STATSLLINE:
    case RPL_STATSUPTIME:
    case RPL_STATSOLINE:
    case RPL_STATSHLINE:
    case RPL_STATSSLINE:
    case RPL_STATSXLINE:
    case RPL_STATSULINE:
    case RPL_STATSDEBUG:
    case RPL_STATSCONN: {
        //emit _q->onStatsMessage(message);
        break;
    }
    case RPL_LUSERCLIENT:
    case RPL_LUSEROP:
    case RPL_LUSERUNKNOWN:
    case RPL_LUSERCHANNELS:
    case RPL_LUSERME: {
        emit _q->onLUserReply(Aki::Irc::LUserReply(message));
        break;
    }
    case RPL_ADMINME:
    case RPL_ADMINLOC1:
    case RPL_ADMINLOC2:
    case RPL_ADMINEMAIL: {
        emit _q->onAdminReply(Aki::Irc::AdminReply(message));
        break;
    }
    case RPL_TRACELOG:
    case RPL_ENDOFTRACE: {
        //emit _q->onTraceMessage(message);
        break;
    }
    case RPL_LOAD2HI: {
        break;
    }
    case RPL_LOCALUSERS: {
        emit _q->onLocalUsersReply(Aki::Irc::LocalUsersReply(message));
        break;
    }
    case RPL_GLOBALUSERS: {
        emit _q->onGlobalUsersReply(Aki::Irc::GlobalUsersReply(message));
        break;
    }
    case RPL_PRIVS: {
        break;
    }
    case RPL_WHOISCERTFP: {
        break;
    }
    case RPL_ACCEPTLIST:
    case RPL_ENDOFACCEPT: {
        break;
    }
    case RPL_NONE: {
         // NOT USED
        break;
    }
    case RPL_AWAY: {
        ///TODO: Fix me
        //emit _q->onAwayMessage(Aki::Irc::AwayMessage(message));
        break;
    }
    case RPL_USERHOST: {
        //emit _q->onUserHostMessage(message);
        break;
    }
    case RPL_ISON: {
        //emit _q->onIsOnMessage(message);
        break;
    }
    case RPL_TEXT: {
        break;
    }
    case RPL_UNAWAY:
    case RPL_NOWAWAY: {
        ///TODO: Fix me
        //emit _q->onAwayMessage(Aki::Irc::AwayMessage(message));
        break;
    }
    case RPL_WHOISUSER: 
    case RPL_WHOISSERVER:
    case RPL_WHOISOPERATOR: {
        //emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_WHOWASUSER: {
        //emit _q->onWhoWasMessage(message);
        break;
    }
    case RPL_ENDOFWHO: {
        emit _q->onWhoReply(Aki::Irc::WhoReply(message));
        break;
    }
    case RPL_WHOISIDLE: 
    case RPL_ENDOFWHOIS:
    case RPL_WHOISCHANNELS:
    case RPL_WHOISSPECIAL: {
        //emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_LISTSTART: {
    case RPL_LIST:
    case RPL_LISTEND:
        //emit _q->onListMessage(message);
        break;
    }
    case RPL_CHANNELMODEIS: {
        break;
    }
    case RPL_CHANNELMLOCK: {
        break;
    }
    case RPL_CHANNELURL: {
        emit _q->onChannelUrlReply(Aki::Irc::ChannelUrlReply(message));
        break;
    }
    case RPL_CREATIONTIME: {
        break;
    }
    case RPL_WHOISLOGGEDIN: {
        //emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_NOTOPIC:
    case RPL_TOPIC: {
        emit _q->onTopicReply(Aki::Irc::TopicReply(message));
        break;
    }
    case RPL_TOPICWHOTIME: {
        emit _q->onTopicSetByReply(Aki::Irc::TopicSetByReply(message));
        break;
    }
    case RPL_WHOISACTUALLY: {
        //emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_INVITING: {
        emit _q->onInviteSent(Aki::Irc::InviteSentReply(message));
        break;
    }
    case RPL_SUMMONING: {
        //emit _q->onSummonMessage(message);
        break;
    }
    case RPL_INVITELIST: {
    case RPL_ENDOFINVITELIST:
        //emit _q->onInviteListMessage(message);
        break;
    }
    case RPL_EXCEPTLIST: {
    case RPL_ENDOFEXCEPTLIST:
        //emit _q->onExceptListMessage(message);
        break;
    }
    case RPL_VERSION: {
        break;
    }
    case RPL_WHOREPLY: {
        emit _q->onWhoReply(Aki::Irc::WhoReply(message));
        break;
    }
    case RPL_WHOSPCRPL: {
        break;
    }
    case RPL_NAMREPLY: {
        emit _q->onNamesReply(Aki::Irc::NamesReply(message));
        break;
    }
    case RPL_WHOWASREAL: {
        //emit _q->onWhoWasMessage(message);
        break;
    }
    case RPL_LINKS: {
    case RPL_ENDOFLINKS:
        //emit _q->onLinksMessage(message);
        break;
    }
    case RPL_ENDOFNAMES: {
        emit _q->onNamesReply(Aki::Irc::NamesReply(message));
        break;
    }
    case RPL_KILLDONE: {
        break;
    }
    case RPL_CLOSING: {
        break;
    }
    case RPL_CLOSEEND: {
        break;
    }
    case RPL_BANLIST: {
    case RPL_ENDOFBANLIST:
        //emit _q->onBanListMessage(message);
        break;
    }
    case RPL_ENDOFWHOWAS: {
        //emit _q->onWhoWasMessage(message);
        break;
    }
    case RPL_WHOISCHANOP: {
        //emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_INFO: {
        //emit _q->onInfoMessage(message);
        break;
    }
    case RPL_MOTD: {
        if (_q->isMotdEnabled()) {
            emit _q->onMotdReply(Aki::Irc::MotdReply(message));
        }
        break;
    }
    case RPL_INFOSTART: {
        break;
    }
    case RPL_ENDOFINFO: {
        //emit _q->onInfoMessage(message);
        break;
    }
    case RPL_MOTDSTART: {
    case RPL_ENDOFMOTD:
        if (_q->isMotdEnabled()) {
            emit _q->onMotdReply(Aki::Irc::MotdReply(message));
        }
        break;
    }
    case RPL_WHOISHOST: {
        //emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_YOUREOPER: {
        break;
    }
    case RPL_REHASHING: {
        break;
    }
    case RPL_MYPORTIS: {
        break;
    }
    case RPL_NOTOPERANYMORE: {
        break;
    }
    case RPL_RSACHALLENGE: {
        break;
    }
    case RPL_TIME: {
        //emit _q->onTimeMessage(message);
        break;
    }
    case RPL_USERSSTART: {
    case RPL_USERS:
    case RPL_ENDOFUSERS:
    case RPL_NOUSERS:
        //emit _q->onUsersMessage(message);
        break;
    }
    case RPL_HOSTHIDDEN: {
        break;
    }
    case ERR_NOSUCHNICK: {
    case ERR_NOSUCHSERVER:
    case ERR_NOSUCHCHANNEL:
    case ERR_CANNOTSENDTOCHAN:
    case ERR_TOOMANYCHANNELS:
    case ERR_WASNOSUCHNICK:
    case ERR_TOOMANYTARGETS:
    case ERR_NOORIGIN:
    case ERR_INVALIDCAPCMD:
    case ERR_NORECIPIENT:
    case ERR_NOTEXTTOSEND:
    case ERR_NOTOPLEVEL:
    case ERR_WILDTOPLEVEL:
    case ERR_TOOMANYMATCHES:
    case ERR_UNKNOWNCOMMAND:
    case ERR_NOMOTD:
    case ERR_NOADMININFO:
    case ERR_FILEERROR:
    case ERR_NONICKNAMEGIVEN:
    case ERR_ERRONEUSNICKNAME:
    case ERR_NICKNAMEINUSE:
    case ERR_BANNICKCHANGE:
    case ERR_NICKCOLLISION:
    case ERR_UNAVAILRESOURCE:
    case ERR_NICKTOOFAST:
    case ERR_SERVICESDOWN:
    case ERR_USERNOTINCHANNEL:
    case ERR_NOTONCHANNEL:
    case ERR_USERONCHANNEL:
    case ERR_NOLOGIN:
    case ERR_SUMMONDISABLED:
    case ERR_USERSDISABLED:
    case ERR_NOTREGISTERED:
    case ERR_ACCEPTFULL:
    case ERR_ACCEPTEXIST:
    case ERR_ACCEPTNOT:
    case ERR_NEEDMOREPARAMS:
    case ERR_ALREADYREGISTRED:
    case ERR_NOPERMFORHOST:
    case ERR_PASSWDMISMATCH:
    case ERR_YOUREBANNEDCREEP:
    case ERR_YOUWILLBEBANNED:
    case ERR_KEYSET:
    case ERR_LINKCHANNEL:
    case ERR_CHANNELISFULL:
    case ERR_UNKNOWNMODE:
    case ERR_INVITEONLYCHAN:
    case ERR_BANNEDFROMCHAN:
    case ERR_BADCHANNELKEY:
    case ERR_BADCHANMASK:
    case ERR_NEEDREGGEDNICK:
    case ERR_BANLISTFULL:
    case ERR_BADCHANNAME:
    case ERR_THROTTLE:
    case ERR_NOPRIVILEGES:
    case ERR_CHANOPRIVSNEEDED:
    case ERR_CANTKILLSERVER:
    case ERR_ISCHANSERVICE:
    case ERR_BANNEDNICK:
    case ERR_NONONREG:
    case ERR_VOICENEEDED:
    case ERR_NOOPERHOST:
    case ERR_OWNMODE:
    case ERR_UMODEUNKNOWNFLAG:
    case ERR_USERSDONTMATCH:
    case ERR_GHOSTEDCLIENT:
    case ERR_USERNOTONSERV:
    case ERR_WRONGPONG:
    case ERR_DISABLED:
    case ERR_HELPNOTFOUND:
        //emit _q->onErrorMessage(message);
        break;
    }
    case RPL_WHOISSECURE: {
        //emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_MODLIST: {
    case RPL_ENDOFMODLIST:
        //emit _q->onModListMessage(message);
        break;
    }
    case RPL_HELPSTART: {
    case RPL_HELPTXT:
    case RPL_ENDOFHELP:
        //emit _q->onHelpMessage(message);
        break;
    }
    case ERR_TARGCHANGE: {
        //emit _q->onErrorMessage(message);
        break;
    }
    case RPL_ETRACEFULL: {
    case RPL_ETRACE:
        break;
    }
    case RPL_KNOCK: {
    case RPL_KNOCKDLVR:
        //emit _q->onKnockMessage(message);
        break;
    }
    case ERR_TOOMANYKNOCK: {
    case ERR_CHANOPEN:
    case ERR_KNOCKONCHAN:
    case ERR_KNOCKDISABLED:
    case ERR_TARGUMODEG:
        //emit _q->onErrorMessage(message);
        break;
    }
    case RPL_TARGNOTIFY: {
        break;
    }
    case RPL_UMODEGMSG: {
        break;
    }
    case RPL_OMOTDSTART: {
    case RPL_OMOTD:
    case RPL_ENDOFOMOTD:
        //emit _q->onOMotdMessage(message);
        break;
    }
    case ERR_NOPRIVS: {
        //emit _q->onErrorMessage(message);
        break;
    }
    case RPL_TESTMASK: {
    case RPL_TESTLINE:
    case RPL_NOTESTLINE:
        //emit _q->onTestMessage(message);
        break;
    }
    case RPL_TESTMASKGECOS: {
        break;
    }

    case RPL_QUIETLIST: {
    case RPL_ENDOFQUIETLIST:
        break;
    }
    case RPL_MONONLINE: {
    case RPL_MONOFFLINE:
    case RPL_MONLIST:
    case RPL_ENDOFMONLIST:
        //emit _q->onMonitorMessage(message);
        break;
    }
    case ERR_MONLISTFULL: {
        //emit _q->onErrorMessage(message);
        break;
    }
    case RPL_RSACHALLENGE2: {
        break;
    }
    case RPL_ENDOFRSACHALLENGE2: {
        break;
    }
    case ERR_MLOCKRESTRICTED: {
        break;
    }
    case RPL_SCANMATCHED: {
        break;
    }
    case RPL_SCANUMODES: {
        break;
    }
    case RPL_LOGGEDIN: {
        break;
    }
    case RPL_LOGGEDOUT: {
        break;
    }
    case ERR_NICKLOCKED: {
        //emit _q->onErrorMessage(message);
        break;
    }
    case RPL_SASLSUCCESS: {
        _q->sendMessage(Aki::Irc::Rfc2812::raw("CAP END"));
        break;
    }
    case ERR_SASLFAIL: {
    case ERR_SASLTOOLONG:
    case ERR_SASLABORTED:
    case ERR_SASLALREADY:
        //emit _q->onErrorMessage(message);
        break;
    }
    case ERR_LAST_ERR_MSG: {
        break;
    }
    default: {
        emit _q->onUnknownReply(message);
        return;
        break;
    }
    }

}

void
SocketPrivate::error(Aki::Irc::BaseSocket::SocketError error)
{
    switch (error) {
    case Aki::Irc::BaseSocket::ConnectionRefusedError: {
        break;
    }
    case Aki::Irc::BaseSocket::HostNotFoundError: {
        break;
    }
    case Aki::Irc::BaseSocket::NetworkError: {
        break;
    }
    case Aki::Irc::BaseSocket::RemoteHostClosedError: {
        break;
    }
    case Aki::Irc::BaseSocket::SocketAccessError: {
        break;
    }
    case Aki::Irc::BaseSocket::SocketResourceError: {
        break;
    }
    case Aki::Irc::BaseSocket::SocketTimeoutError: {
        break;
    }
    case Aki::Irc::BaseSocket::UnknownError: {
        break;
    }
    case Aki::Irc::BaseSocket::UnsupportedSocketOperationError: {
        break;
    }
    default: {
        break;
    }
    }
}

void
SocketPrivate::messageReceived(const Aki::Irc::ReplyInfo& message)
{
    qDebug() << message;
    const QString command = message.command().toUpper();
    if (command == "NOTICE") {
        if (message.message().startsWith('\1') &&
            message.message().endsWith('\1')) {
            
        } else {
            emit _q->onNoticeReply(Aki::Irc::NoticeReply(message));
        }
    } else if (command == "PRIVMSG") {
        // We need to check if this is a ctcp reply privmsg.
        if (message.params().at(1).startsWith('\1') &&
            message.params().at(1).endsWith('\1')) {
            // Get the message which will be for example <ACTION> <message>
            QString tmpMessage = message.params().at(1);

            // Remove the \1 from the beginning.
            tmpMessage.remove(0, 1);
            // Remove the last \1 at the end.
            tmpMessage.remove(tmpMessage.count() - 1, 1);

            // Remove the message from the start
            const QString cmd = removeStringToFirstWhitespace(&tmpMessage);
            if (cmd.toUpper() == "ACTION") {
                emit _q->onActionReply(Aki::Irc::ActionReply(Aki::Irc::CtcpReply(message)));
            } else {
                emit _q->onCtcpReply(Aki::Irc::CtcpReply(message));
            }
        } else if (message.params().at(1) == _q->currentNick()) {
            // We gotten a privmsg meant for the user only.
            emit _q->onPrivateMessageReply(Aki::Irc::PrivateMessageReply(message));
        } else {
            // We gotten a privmsg meant for the channel.
            emit _q->onChannelMessageReply(Aki::Irc::ChannelMessageReply(Aki::Irc::PrivateMessageReply(message)));
        }
    } else if (command == "KICK") {
        emit _q->onKickReply(Aki::Irc::KickReply(message));
    } else if (command == "NICK") {
        emit _q->onNickReply(Aki::Irc::NickReply(message));
    } else if (command == "TOPIC") {
        emit _q->onTopicChangeReply(Aki::Irc::TopicChangeReply(message));
    } else if (command == "INVITE") {
        emit _q->onInviteReply(Aki::Irc::InviteReply(message));
    } else if (command == "PART") {
        
    } else if (command == "QUIT") {
        emit _q->onQuitReply(Aki::Irc::QuitReply(message));
    } else if (command == "JOIN") {
        emit _q->onJoinReply(Aki::Irc::JoinReply(message));
    } else if (command == "CAP") {
        const QString subCommand = message.params().at(1);
        const QStringList split = message.params().value(2).split(' ');
        if (subCommand == "LS") {
            QStringList avail;
            foreach (const QString& support, split) {
                if (support == "multi-prefix") {
                    avail.append("multi-prefix");
                } else if (support == "sasl") {
                    if (_q->isSaslEnabled()) {
                        avail.append("sasl");
                    }
                } else if (support == "identify-msg") {
                    avail.append("identify-msg");
                }
            }

            if (!avail.isEmpty()) {
                _q->sendMessage(Aki::Irc::Rfc2812::raw(QString("CAP REQ :%1").arg(avail.join(" "))));
            } else {
                _q->sendMessage(Aki::Irc::Rfc2812::raw("CAP END"));
            }
        } else if (subCommand == "ACK") {
            foreach (const QString& support, split) {
                if (support == "sasl") {
                    if (_q->isSaslEnabled()) {
                        _q->sendMessage(Aki::Irc::Rfc2812::raw("AUTHENTICATE PLAIN"));
                    }
                }
            }
        } else if (subCommand == "NAK") {
            _q->sendMessage(Aki::Irc::Rfc2812::raw("CAP END"));
        } else if (subCommand == "LIST") {
            
        }
    } else if (command == "AUTHENTICATE") {
        if (_q->isSaslEnabled()) {
            const QString nick = _q->currentNick().toLower();
            const QString password = _q->servicePassword();
            const QByteArray base64 = (nick + '\0' + nick + '\0' + password).toAscii().toBase64();

            _q->sendMessage(Aki::Irc::Rfc2812::raw(QString("AUTHENTICATE %1").arg(QString(base64))));
        }
    } else if (command == "PING") {
        _q->sendMessage(Aki::Irc::Rfc2812::pong(message.params().at(0)));
    }
}

void
SocketPrivate::rawMessageReceived(const QString& message)
{
    QString line = message;
    qDebug() << line;

    Aki::Irc::SocketPrivate::Message msg;
    msg.message = message;

    if (line.startsWith(':')) {
        msg.sender.setHostmask(removeStringToFirstWhitespace(&line, 1, 1));
    }

    msg.command = removeStringToFirstWhitespace(&line);
    msg.replyCode = static_cast<Aki::Irc::ReplyCodes>(msg.command.toInt());

    while (!line.isEmpty()) {
        if (line.startsWith(':')) {
            line.remove(0, 1);
            msg.params << line;
            line.clear();
        } else {
            msg.params << removeStringToFirstWhitespace(&line);
        }
    }

    if (!msg.params.isEmpty()) {
    }

    const Aki::Irc::ReplyInfo info(msg.sender, msg.command, msg.message, msg.params, msg.replyCode);
    if (info.isNumeric()) {
        commandReceived(info);
    } else {
        messageReceived(info);
    }
}

QString
SocketPrivate::removeStringToFirstWhitespace(QString* line)
{
    const QString tmp = line->left(line->indexOf(' '));
    line->remove(0, tmp.length() + 1);
    return tmp;
}

QString
SocketPrivate::removeStringToFirstWhitespace(QString* line, int start, int stop)
{
    const QString tmp = line->mid(start, line->indexOf(' ') - stop);
    line->remove(0, tmp.length() + 1 + stop);
    return tmp;
}

void
SocketPrivate::sslErrors(const QList<Aki::Irc::BaseSocket::SslError>& errors)
{
    Q_UNUSED(errors)
}

void
SocketPrivate::stateChanged(Aki::Irc::BaseSocket::SocketState state)
{
    switch (state) {
    case Aki::Irc::BaseSocket::ClosingState: {
        break;
    }
    case Aki::Irc::BaseSocket::ConnectedState: {
        if (!_q->serverPassword().isEmpty()) {
            _q->sendMessage(Aki::Irc::Rfc2812::pass(_q->serverPassword()));
        }

        _q->sendMessage(Aki::Irc::Rfc2812::raw("CAP LS"));
        _q->sendMessage(Aki::Irc::Rfc2812::user(_q->identName(), false, _q->realName()));
        _q->sendMessage(Aki::Irc::Rfc2812::nick(_q->currentNick()));
        break;
    }
    case Aki::Irc::BaseSocket::ConnectingState: {
        break;
    }
    case Aki::Irc::BaseSocket::HostLookupState: {
        break;
    }
    case Aki::Irc::BaseSocket::UnconnectState: {
        if (_q->isAutoReconnectEnabled()) {
            QTimer::singleShot(_q->retryInterval() * 1000, _q, SLOT(connectToHost()));
        }
        break;
    }
    default: {
        break;
    }
    }
}
