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
    : _q(qq)
{
}

void
SocketPrivate::commandReceived(const Aki::Irc::Message& message)
{
///TODO: Fix switch
    switch (0) {
    case RPL_NULL: {
        break;
    }
    case RPL_WELCOME: {
        break;
    }
    case RPL_YOURHOST: {
        break;
    }
    case RPL_CREATED: {
        break;
    }
    case RPL_MYINFO: {
        break;
    }
    case RPL_ISUPPORT: {
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
    case RPL_TRACELINK: {
    case RPL_TRACECONNECTING:
    case RPL_TRACEHANDSHAKE:
    case RPL_TRACEUNKNOWN:
    case RPL_TRACEOPERATOR:
    case RPL_TRACEUSER:
    case RPL_TRACESERVER:
    case RPL_TRACENEWTYPE:
    case RPL_TRACECLASS:
        emit _q->onTraceMessage(message);
        break;
    }
    case RPL_STATSLINKINFO: {
    case RPL_STATSCOMMANDS:
    case RPL_STATSCLINE:
    case RPL_STATSNLINE:
    case RPL_STATSILINE:
    case RPL_STATSKLINE:
    case RPL_STATSQLINE:
    case RPL_STATSYLINE:
    case RPL_ENDOFSTATS:
    case RPL_STATSPLINE:
        emit _q->onStatsMessage(message);
        break;
    }
    case RPL_UMODEIS: {
        break;
    }
    case RPL_STATSFLINE: {
    case RPL_STATSDLINE:
        emit _q->onStatsMessage(message);
        break;
    }
    case RPL_SERVLIST: {
    case RPL_SERVLISTEND:
        emit _q->onServerListMessage(message);
        break;
    }
    case RPL_STATSLLINE: {
    case RPL_STATSUPTIME:
    case RPL_STATSOLINE:
    case RPL_STATSHLINE:
    case RPL_STATSSLINE:
    case RPL_STATSXLINE:
    case RPL_STATSULINE:
    case RPL_STATSDEBUG:
    case RPL_STATSCONN:
        emit _q->onStatsMessage(message);
        break;
    }
    case RPL_LUSERCLIENT: {
    case RPL_LUSEROP:
    case RPL_LUSERUNKNOWN:
    case RPL_LUSERCHANNELS:
    case RPL_LUSERME:
        emit _q->onLUserMessage(message);
        break;
    }
    case RPL_ADMINME: {
    case RPL_ADMINLOC1:
    case RPL_ADMINLOC2:
    case RPL_ADMINEMAIL:
        ///TODO: Fix me
        //emit _q->onAdminMessage(Aki::Irc::AdminMessage(message));
        break;
    }
    case RPL_TRACELOG: {
    case RPL_ENDOFTRACE:
        emit _q->onTraceMessage(message);
        break;
    }
    case RPL_LOAD2HI: {
        break;
    }
    case RPL_LOCALUSERS: {
        break;
    }
    case RPL_GLOBALUSERS: {
        break;
    }
    case RPL_PRIVS: {
        break;
    }
    case RPL_WHOISCERTFP: {
        break;
    }
    case RPL_ACCEPTLIST:{
    case RPL_ENDOFACCEPT:
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
        emit _q->onUserHostMessage(message);
        break;
    }
    case RPL_ISON: {
        emit _q->onIsOnMessage(message);
        break;
    }
    case RPL_TEXT: {
        break;
    }
    case RPL_UNAWAY: {
    case RPL_NOWAWAY:
        ///TODO: Fix me
        //emit _q->onAwayMessage(Aki::Irc::AwayMessage(message));
        break;
    }
    case RPL_WHOISUSER: {
    case RPL_WHOISSERVER:
    case RPL_WHOISOPERATOR:
        emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_WHOWASUSER: {
        emit _q->onWhoWasMessage(message);
        break;
    }
    case RPL_ENDOFWHO: {
        emit _q->onWhoMessage(message);
        break;
    }
    case RPL_WHOISIDLE: {
    case RPL_ENDOFWHOIS:
    case RPL_WHOISCHANNELS:
    case RPL_WHOISSPECIAL:
        emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_LISTSTART: {
    case RPL_LIST:
    case RPL_LISTEND:
        emit _q->onListMessage(message);
        break;
    }
    case RPL_CHANNELMODEIS: {
        break;
    }
    case RPL_CHANNELMLOCK: {
        break;
    }
    case RPL_CHANNELURL: {
        break;
    }
    case RPL_CREATIONTIME: {
        break;
    }
    case RPL_WHOISLOGGEDIN: {
        emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_NOTOPIC: {
    case RPL_TOPIC:
    case RPL_TOPICWHOTIME:
        emit _q->onTopicMessage(message);
        break;
    }
    case RPL_WHOISACTUALLY: {
        emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_INVITING: {
        emit _q->onInviteMessage(message);
        break;
    }
    case RPL_SUMMONING: {
        emit _q->onSummonMessage(message);
        break;
    }
    case RPL_INVITELIST: {
    case RPL_ENDOFINVITELIST:
        emit _q->onInviteListMessage(message);
        break;
    }
    case RPL_EXCEPTLIST: {
    case RPL_ENDOFEXCEPTLIST:
        emit _q->onExceptListMessage(message);
        break;
    }
    case RPL_VERSION: {
        break;
    }
    case RPL_WHOREPLY: {
        emit _q->onWhoMessage(message);
        break;
    }
    case RPL_WHOSPCRPL: {
        break;
    }
    case RPL_NAMREPLY: {
        emit _q->onNamesMessage(message);
        break;
    }
    case RPL_WHOWASREAL: {
        emit _q->onWhoWasMessage(message);
        break;
    }
    case RPL_LINKS: {
    case RPL_ENDOFLINKS:
        emit _q->onLinksMessage(message);
        break;
    }
    case RPL_ENDOFNAMES: {
        emit _q->onNamesMessage(message);
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
        emit _q->onWhoWasMessage(message);
        break;
    }
    case RPL_WHOISCHANOP: {
        emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_INFO: {
        emit _q->onInfoMessage(message);
        break;
    }
    case RPL_MOTD: {
        //emit _q->onMotdMessage(message);
        break;
    }
    case RPL_INFOSTART: {
        break;
    }
    case RPL_ENDOFINFO: {
        emit _q->onInfoMessage(message);
        break;
    }
    case RPL_MOTDSTART: {
    case RPL_ENDOFMOTD:
        //emit _q->onMotdMessage(message);
        break;
    }
    case RPL_WHOISHOST: {
        emit _q->onWhoIsMessage(message);
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
        emit _q->onTimeMessage(message);
        break;
    }
    case RPL_USERSSTART: {
    case RPL_USERS:
    case RPL_ENDOFUSERS:
    case RPL_NOUSERS:
        emit _q->onUsersMessage(message);
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
        emit _q->onErrorMessage(message);
        break;
    }
    case RPL_WHOISSECURE: {
        emit _q->onWhoIsMessage(message);
        break;
    }
    case RPL_MODLIST: {
    case RPL_ENDOFMODLIST:
        emit _q->onModListMessage(message);
        break;
    }
    case RPL_HELPSTART: {
    case RPL_HELPTXT:
    case RPL_ENDOFHELP:
        emit _q->onHelpMessage(message);
        break;
    }
    case ERR_TARGCHANGE: {
        emit _q->onErrorMessage(message);
        break;
    }
    case RPL_ETRACEFULL: {
    case RPL_ETRACE:
        break;
    }
    case RPL_KNOCK: {
    case RPL_KNOCKDLVR:
        emit _q->onKnockMessage(message);
        break;
    }
    case ERR_TOOMANYKNOCK: {
    case ERR_CHANOPEN:
    case ERR_KNOCKONCHAN:
    case ERR_KNOCKDISABLED:
    case ERR_TARGUMODEG:
        emit _q->onErrorMessage(message);
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
        emit _q->onOMotdMessage(message);
        break;
    }
    case ERR_NOPRIVS: {
        emit _q->onErrorMessage(message);
        break;
    }
    case RPL_TESTMASK: {
    case RPL_TESTLINE:
    case RPL_NOTESTLINE:
        emit _q->onTestMessage(message);
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
        emit _q->onMonitorMessage(message);
        break;
    }
    case ERR_MONLISTFULL: {
        emit _q->onErrorMessage(message);
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
        emit _q->onErrorMessage(message);
        break;
    }
    case RPL_SASLSUCCESS: {
        break;
    }
    case ERR_SASLFAIL: {
    case ERR_SASLTOOLONG:
    case ERR_SASLABORTED:
    case ERR_SASLALREADY:
        emit _q->onErrorMessage(message);
        break;
    }
    case ERR_LAST_ERR_MSG: {
        break;
    }
    }

}

void
SocketPrivate::error(Aki::Irc::BaseSocket::SocketError error)
{
    DEBUG_FUNC_NAME;
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
SocketPrivate::messageReceived(const Aki::Irc::Message& message)
{
    ///TODO: Fix me
    /*const QString command = message.command().toUpper();
    if (command == "PING" || command == "PONG") {
        emit _q->onPingPongMessage(message);
    } else if (command == "TOPIC") {
        emit _q->onTopicChangeMessage(message);
    }*/
}

void
SocketPrivate::rawMessageReceived(const Aki::Irc::Message& message)
{
    ///TODO: Fix me
    /*if (message.isNumeric()) {
        commandReceived(message);
    } else {
        messageReceived(message);
    }*/
}

void
SocketPrivate::sslErrors(const QList<Aki::Irc::BaseSocket::SslError>& errors)
{
    DEBUG_FUNC_NAME;
    Q_UNUSED(errors)
}

void
SocketPrivate::stateChanged(Aki::Irc::BaseSocket::SocketState state)
{
    DEBUG_FUNC_NAME;
    switch (state) {
    case Aki::Irc::BaseSocket::ClosingState: {
        break;
    }
    case Aki::Irc::BaseSocket::ConnectedState: {
        if (not _q->serverPassword().isEmpty()) {
            Aki::Irc::Message message = Aki::Irc::Rfc2812::pass(_q->serverPassword());
            _q->sendMessage(message);
        }

        Aki::Irc::Message message = Aki::Irc::Rfc2812::user(_q->identName(), false, _q->realName());
        _q->sendMessage(message);
        message = Aki::Irc::Rfc2812::nick(_q->currentNick());
        _q->sendMessage(message);
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
