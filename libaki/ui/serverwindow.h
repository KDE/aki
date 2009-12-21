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

#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include "libaki_export.h"
#include "ui/basewindow.h"
#include <QScopedPointer>

namespace Aki
{
namespace Irc
{
class Socket;
} // End of namespace Irc.
class MessageLog;
class Notifications;
class IdentityConfig;
class ServerWindowPrivate;
class LIBAKI_EXPORT ServerWindow : public Aki::BaseWindow
{
    Q_OBJECT
public:
    explicit ServerWindow(Aki::IdentityConfig *identityConfig, Aki::Irc::Socket *socket,
                          Aki::Notifications *notifications, Aki::MessageLog *messageLog,
                          QWidget *parent = 0);
    ~ServerWindow();
    Aki::IdentityConfig* identity();
    void createNewView(Aki::BaseWindow *window);
    Aki::BaseWindow* currentFocusedChannel();
    Aki::BaseWindow* findChannel(const QString &name);
    bool hasInputFocus() const;
    QStringList rejoinChannelList() const;
    Aki::ChannelView* mainView();
    Aki::ChannelView* splitView();
Q_SIGNALS:
    void dropSuccessful(Aki::BaseWindow *window);
    void customCommand(const QString &command, const QString &message);
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
private:
    Q_PRIVATE_SLOT(d, void stateChanged(Aki::Irc::Socket::SocketState state))
    Q_PRIVATE_SLOT(d, void onAway(const QString &nick, const QString &message))
    Q_PRIVATE_SLOT(d, void onBanList(const QString &channel, const QString &mask,
                                     const QString &who, const QDateTime &time))
    Q_PRIVATE_SLOT(d, void onChannelCreated(const QString &channel, const QDateTime &time))
    Q_PRIVATE_SLOT(d, void onChannelModeIs(const QString &channel, const QString &modes,
                                           const QStringList &params))
    Q_PRIVATE_SLOT(d, void onChannelUrlIs(const QString &channel, const QString &url))
    Q_PRIVATE_SLOT(d, void onCreated(const QString &message))
    Q_PRIVATE_SLOT(d, void onCtcpAction(const QString &from, const QString &to,
                                        const QString &message))
    Q_PRIVATE_SLOT(d, void onCtcpRequest(const QString &from, const QString &type))
    Q_PRIVATE_SLOT(d, void onEndOfMotd(const QString &message))
    Q_PRIVATE_SLOT(d, void onEndOfNames(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onEndOfWho(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onEndOfWhoIs(const QString &nick, const QString &message))
    Q_PRIVATE_SLOT(d, void onEndOfWhoWas(const QString &nick, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorAlreadyRegistered(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorBadChanMask(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorBadChannelKey(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorBanListFull(const QString &channel, const QString &ban,
                                              const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorBannedFromChannel(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorCannotSendToChannel(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorCantKillServer(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorChannelIsFull(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorChannelPrivilegesNeeded(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorErroneusNickname(const QString &nick, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorFileError(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorInviteOnlyChannel(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorKeySet(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNeedMoreParams(const QString &command, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNickCollision(const QString &nick, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNicknameInUse(const QString &nick, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoAdminInfo(const QString &server, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoChanModes(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoLogin(const QString &user, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoMotd(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoNicknameGiven(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoOperatorHost(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoPrivileges(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoRecipient(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoSuchChannel(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoSuchNick(const QString &nick, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoSuchServer(const QString &server, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoSuchService(const QString &service, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoTextToSend(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNotOnChannel(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorNoTopLevel(const QString &mask, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorPasswordMismatch(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorRestricted(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorSummonDisabled(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorTooManyChannels(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorUModeUnknownFlag(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorUnknownCommand(const QString &command, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorUnknownMode(const QString &mode, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorUniqueOpPrivilegesNeeded(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorUserOnChannel(const QString &nick, const QString &channel,
                                                const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorUsersDisabled(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorUsersDontMatch(const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorWasNoSuchNick(const QString &nick, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorWildTopLevel(const QString &mask, const QString &message))
    Q_PRIVATE_SLOT(d, void onErrorYoureBannedCreep(const QString &message))
    Q_PRIVATE_SLOT(d, void onGlobalUsers(const QString &message))
    Q_PRIVATE_SLOT(d, void onHighConnectCount(const QString &message))
    Q_PRIVATE_SLOT(d, void onInvite(const QString &from, const QString &channel))
    Q_PRIVATE_SLOT(d, void onInviting(const QString &nick, const QString &channel))
    Q_PRIVATE_SLOT(d, void onIsOn(const QStringList &nicks))
    Q_PRIVATE_SLOT(d, void onISupport(const QString &message))
    Q_PRIVATE_SLOT(d, void onKick(const QString &channel, const QString &nick, const QString &message))
    Q_PRIVATE_SLOT(d, void onLocalUsers(const QString &message))
    Q_PRIVATE_SLOT(d, void onLUserChannels(int count, const QString &message))
    Q_PRIVATE_SLOT(d, void onLUserClient(const QString &message))
    Q_PRIVATE_SLOT(d, void onLUserMe(const QString &message))
    Q_PRIVATE_SLOT(d, void onLUserOp(int count, const QString &message))
    Q_PRIVATE_SLOT(d, void onLUserUnknown(int count, const QString &message))
    Q_PRIVATE_SLOT(d, void onMode(const QString &from, const QString &channel, const QString &modes,
                                  const QString &nick))
    Q_PRIVATE_SLOT(d, void onMode(const QString &from, const QString &channel, const QString &modes,
                                  const QStringList &params))
    Q_PRIVATE_SLOT(d, void onMotd(const QString &message))
    Q_PRIVATE_SLOT(d, void onMotdStart(const QString &message))
    Q_PRIVATE_SLOT(d, void onMyInfo(const QString &message))
    Q_PRIVATE_SLOT(d, void onNameReply(const QString &channel, const QString &names))
    Q_PRIVATE_SLOT(d, void onNick(const QString &oldNick, const QString &newNick))
    Q_PRIVATE_SLOT(d, void onNotice(const QString &from, const QString &message))
    Q_PRIVATE_SLOT(d, void onNoticeAuth(const QString &message))
    Q_PRIVATE_SLOT(d, void onNoticeCtcpVersion(const QString &nick, const QString &version))
    Q_PRIVATE_SLOT(d, void onNoTopic(const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onNowAway(const QString &message))
    Q_PRIVATE_SLOT(d, void onPong(const QString &server, const QString &server2))
    Q_PRIVATE_SLOT(d, void onPrivmsg(const QString &channel, const QString &from, const QString &to,
                                     const QString &message))
    Q_PRIVATE_SLOT(d, void onSelfJoin(const QString &channel))
    Q_PRIVATE_SLOT(d, void onSelfUMode(const QString &modes))
    Q_PRIVATE_SLOT(d, void onTime(const QString &server, const QString &time))
    Q_PRIVATE_SLOT(d, void onTopic(const QString &channel, const QString &topic))
    Q_PRIVATE_SLOT(d, void onTopicChanged(const QString &nick, const QString &channel, const QString &topic))
    Q_PRIVATE_SLOT(d, void onTopicSetBy(const QString &nick, const QString &channel, const QDateTime &time))
    Q_PRIVATE_SLOT(d, void onUMode(const QString &nick, const QString &modes))
    Q_PRIVATE_SLOT(d, void onUModeIs(const QString &modes))
    Q_PRIVATE_SLOT(d, void onUnAway(const QString &message))
    Q_PRIVATE_SLOT(d, void onUserHost(const QStringList &nicks, const QStringList &hosts))
    Q_PRIVATE_SLOT(d, void onUserJoin(const QString &hostMask, const QString &channel))
    Q_PRIVATE_SLOT(d, void onUserPart(const QString &hostMask, const QString &channel, const QString &message))
    Q_PRIVATE_SLOT(d, void onUserQuit(const QString &hostMask, const QString &message))
    Q_PRIVATE_SLOT(d, void onVersion(const QString &message))
    Q_PRIVATE_SLOT(d, void onWelcome(const QString &message))
    Q_PRIVATE_SLOT(d, void onWho(const QString &channel, const QString &identName, const QString &address,
                                 const QString &server, const QString &nick, const QString &flags,
                                 int hops, const QString &realName))
    Q_PRIVATE_SLOT(d, void onWhoIsChannels(const QString &nick, const QString &channels))
    Q_PRIVATE_SLOT(d, void onWhoIsIdentified(const QString &nick, const QString &info))
    Q_PRIVATE_SLOT(d, void onWhoIsIdle(const QString &nick, const QDateTime &idleTime, const QDateTime &signon,
                                       const QString &info))
    Q_PRIVATE_SLOT(d, void onWhoIsOperator(const QString &nick, const QString &message))
    Q_PRIVATE_SLOT(d, void onWhoIsServer(const QString &nick, const QString &server, const QString &info))
    Q_PRIVATE_SLOT(d, void onWhoIsUser(const QString &nick, const QString &userName, const QString &address,
                                       const QString &info))
    Q_PRIVATE_SLOT(d, void onWhoWasUser(const QString &nick, const QString &userName, const QString &address,
                                        const QString &info))
    Q_PRIVATE_SLOT(d, void onWriteLine(bool in, const QString &line))
    Q_PRIVATE_SLOT(d, void onYoureOper(const QString &message))
    Q_PRIVATE_SLOT(d, void onYourHost(const QString &message))
    Q_PRIVATE_SLOT(d, void on901(const QString &id, const QString &identName, const QString &address,
                                 const QString &message))
    Q_PRIVATE_SLOT(d, void whoRequested(const QString &name))
    Q_PRIVATE_SLOT(d, void queryMessage(const QString &to, const QString &message))
private:
    friend class ServerWindowPrivate;
    QScopedPointer<ServerWindowPrivate> d;
}; // End of class ServerWindow.
} // End of namespace Aki.

#endif // SERVERWINDOW_H
