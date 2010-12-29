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

#ifndef AKI_IRC_SOCKET_HOO
#define AKI_IRC_SOCKET_HOO

#include "aki.hpp"
#include "irc/actionreply.hpp"
#include "irc/adminreply.hpp"
#include "irc/awayreply.hpp"
#include "irc/banreply.hpp"
#include "irc/basesocket.hpp"
#include "irc/ctcpreply.hpp"
#include "irc/globalusersreply.hpp"
#include "irc/joinreply.hpp"
#include "irc/kickreply.hpp"
#include "irc/localusersreply.hpp"
#include "irc/luserreply.hpp"
#include "irc/motdreply.hpp"
#include "irc/namesreply.hpp"
#include "irc/nickreply.hpp"
#include "irc/noticereply.hpp"
#include "irc/startupreply.hpp"
#include "irc/topicreply.hpp"
#include <QtCore/QList>

namespace Aki
{
namespace Irc
{
class SocketPrivate;
class LIBAKI_EXPORT Socket
    : public Aki::Irc::BaseSocket
{
    Q_OBJECT
public:
    typedef QHash<QString,QString> ServerCapabilities;
    explicit Socket(QObject* parent = 0);
    explicit Socket(const QString& name, QObject* parent = 0);
    virtual ~Socket();
    void connectSlotsBySignals(Aki::Irc::Socket* socket);
    Aki::Irc::Socket::ServerCapabilities serverCapabilities() const;
Q_SIGNALS:
    void onActionReply(const Aki::Irc::ActionReply& reply);
    void onAdminReply(const Aki::Irc::AdminReply& reply);
    void onAwayReply(const Aki::Irc::AwayReply& reply);
    void onBanListReply(const Aki::Irc::BanReply& reply);
    void onCtcpReply(const Aki::Irc::CtcpReply& reply);
    void onErrorReply(const Aki::Irc::Message& errorMessage);
    void onExceptListReply(const Aki::Irc::Message& exceptListMessage);
    void onGlobalUsersReply(const Aki::Irc::GlobalUsersReply& reply);
    void onHelpReply(const Aki::Irc::Message& helpMessage);
    void onInfoReply(const Aki::Irc::Message& infoMessage);
    void onInviteListReply(const Aki::Irc::Message& inviteListMessage);
    void onInviteReply(const Aki::Irc::Message& inviteMessage);
    void onIsOnReply(const Aki::Irc::Message& isOnMessage);
    void onJoinReply(const Aki::Irc::JoinReply& reply);
    void onKickReply(const Aki::Irc::KickReply& reply);
    void onKnockReply(const Aki::Irc::Message& knockMessage);
    void onLinksReply(const Aki::Irc::Message& linksMessage);
    void onListReply(const Aki::Irc::Message& listMessage);
    void onLocalUsersReply(const Aki::Irc::LocalUsersReply& reply);
    void onLUserReply(const Aki::Irc::LUserReply& reply);
    void onModListReply(const Aki::Irc::Message& modListMessage);
    void onMonitorReply(const Aki::Irc::Message& monitorMessage);
    void onMotdReply(const Aki::Irc::MotdReply& reply);
    void onNamesReply(const Aki::Irc::NamesReply& namesMessage);
    void onNickReply(const Aki::Irc::NickReply& reply);
    void onNoticeReply(const Aki::Irc::NoticeReply& reply);
    void onOMotdReply(const Aki::Irc::Message& omotdMessage);
    void onPingPongReply(const Aki::Irc::Message& pingPongMessage);
    void onServerListReply(const Aki::Irc::Message& serverListMessage);
    void onStartupReply(const Aki::Irc::StartupReply& reply);
    void onStatsReply(const Aki::Irc::Message& statsMessage);
    void onSummonReply(const Aki::Irc::Message& summonMessage);
    void onTestReply(const Aki::Irc::Message& testMessage);
    void onTimeReply(const Aki::Irc::Message& timeMessage);
    void onTopicChangeReply(const Aki::Irc::Message& topicChangeMessage);
    void onTopicReply(const Aki::Irc::TopicReply& reply);
    void onTraceReply(const Aki::Irc::Message& traceMessage);
    void onUserHostReply(const Aki::Irc::Message& userHostMessage);
    void onUsersReply(const Aki::Irc::Message& usersMessage);
    void onWhoIsReply(const Aki::Irc::Message& whoisMessage);
    void onWhoReply(const Aki::Irc::Message& whoMessage);
    void onWhoWasReply(const Aki::Irc::Message& whoWasMessage);
private:
    Q_PRIVATE_SLOT(_d, void error(Aki::Irc::BaseSocket::SocketError))
    Q_PRIVATE_SLOT(_d, void rawMessageReceived(const QString& message))
    Q_PRIVATE_SLOT(_d, void sslErrors(const QList<Aki::Irc::BaseSocket::SslError>& errors))
    Q_PRIVATE_SLOT(_d, void stateChanged(Aki::Irc::BaseSocket::SocketState))
private:
    AKI_DECLARE_PRIVATE(Socket)
}; // End of class Socket.
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_SOCKET_HOO
