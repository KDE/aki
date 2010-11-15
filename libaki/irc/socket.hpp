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
#include "irc/basesocket.hpp"
#include "message/adminmessage.hpp"
#include "message/awaymessage.hpp"
#include <QtCore/QList>

namespace Aki
{
namespace Irc
{
class SocketPrivate;
class LIBAKI_EXPORT Socket : public Aki::Irc::BaseSocket
{
    Q_OBJECT
public:
    explicit Socket(QObject* parent = 0);
    explicit Socket(const QString& name, QObject* parent = 0);
    virtual ~Socket();
    void connectSlotsBySignals(Aki::Irc::Socket* socket);
Q_SIGNALS:
    void onAdminMessage(const Aki::Irc::AdminMessage& adminMessage);
    void onAwayMessage(const Aki::Irc::AwayMessage& awayMessage);
    void onBanListMessage(const Aki::Irc::Message& banListMessage);
    void onErrorMessage(const Aki::Irc::Message& errorMessage);
    void onExceptListMessage(const Aki::Irc::Message& exceptListMessage);
    void onHelpMessage(const Aki::Irc::Message& helpMessage);
    void onInfoMessage(const Aki::Irc::Message& infoMessage);
    void onInviteListMessage(const Aki::Irc::Message& inviteListMessage);
    void onInviteMessage(const Aki::Irc::Message& inviteMessage);
    void onIsOnMessage(const Aki::Irc::Message& isOnMessage);
    void onKnockMessage(const Aki::Irc::Message& knockMessage);
    void onLinksMessage(const Aki::Irc::Message& linksMessage);
    void onListMessage(const Aki::Irc::Message& listMessage);
    void onLUserMessage(const Aki::Irc::Message& luserMessage);
    void onModListMessage(const Aki::Irc::Message& modListMessage);
    void onMonitorMessage(const Aki::Irc::Message& monitorMessage);
    void onMotdMessage(const Aki::Irc::Message& motdMessage);
    void onNamesMessage(const Aki::Irc::Message& namesMessage);
    void onOMotdMessage(const Aki::Irc::Message& omotdMessage);
    void onPingPongMessage(const Aki::Irc::Message& pingPongMessage);
    void onServerListMessage(const Aki::Irc::Message& serverListMessage);
    void onStatsMessage(const Aki::Irc::Message& statsMessage);
    void onSummonMessage(const Aki::Irc::Message& summonMessage);
    void onTestMessage(const Aki::Irc::Message& testMessage);
    void onTimeMessage(const Aki::Irc::Message& timeMessage);
    void onTopicChangeMessage(const Aki::Irc::Message& topicChangeMessage);
    void onTopicMessage(const Aki::Irc::Message& topicMessage);
    void onTraceMessage(const Aki::Irc::Message& traceMessage);
    void onUserHostMessage(const Aki::Irc::Message& userHostMessage);
    void onUsersMessage(const Aki::Irc::Message& usersMessage);
    void onWhoIsMessage(const Aki::Irc::Message& whoisMessage);
    void onWhoMessage(const Aki::Irc::Message& whoMessage);
    void onWhoWasMessage(const Aki::Irc::Message& whoWasMessage);
    void onWelcomeMessage(const Aki::Irc::Message& welcomeMessage);
private:
    Q_PRIVATE_SLOT(_d, void error(Aki::Irc::BaseSocket::SocketError))
    Q_PRIVATE_SLOT(_d, void rawMessageReceived(const Aki::Irc::Message& message))
    Q_PRIVATE_SLOT(_d, void sslErrors(const QList<Aki::Irc::BaseSocket::SslError>& errors))
    Q_PRIVATE_SLOT(_d, void stateChanged(Aki::Irc::BaseSocket::SocketState))
private:
    AKI_DECLARE_PRIVATE(Socket)
}; // End of class Socket.
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_SOCKET_HOO
