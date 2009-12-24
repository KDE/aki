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

#include "serverwindow.h"
#include "chatparser.h"
#include "config/identityconfig.h"
#include "dialogs/messagelog.h"
#include "notifications.h"
#include "settings.h"
#include "ui/channelview.h"
#include "ui/channelwindow.h"
#include "ui/chatview.h"
#include "ui/querywindow.h"
#include "ui/statuswindow.h"
#include <Aki/Irc/Color>
#include <Aki/Irc/Ctcp>
#include <Aki/Irc/Rfc2812>
#include <Aki/Irc/Socket>
#include <Aki/Irc/User>
#include <KLocale>
#include <KMessageBox>
#include <QHostAddress>
#include <QHostInfo>
#include <QDragEnterEvent>
#include <QVBoxLayout>
using namespace Aki;

namespace Aki
{
class ServerWindowPrivate
{
public:
    ServerWindowPrivate(ServerWindow *qq)
        : q(qq),
        identityConfig(0),
        messageLog(0)
    {
    }

    Aki::BaseWindow* currentFocusedChannel()
    {
        Aki::BaseWindow *base = 0;
        foreach (Aki::BaseWindow *window, splitView->windows()) {
            if (window->isCurrent() && window->hasInputFocus()) {
                base = window;
            }
        }
        foreach (Aki::BaseWindow *window, mainView->windows()) {
            if (!base) {
                if (window->isCurrent()) {
                    base = window;
                }
            }
        }

        return base;
    }

    Aki::BaseWindow* findChannel(const QString &name)
    {
        Aki::BaseWindow *channel = 0;
        if (splitView->isHidden()) {
            channel = mainView->findChannel(name);
        } else {
            channel = splitView->findChannel(name);
            if (!channel) {
                channel = mainView->findChannel(name);
            }
        }

        return channel;
    }

    void stateChanged(Aki::Irc::Socket::SocketState state)
    {
        switch (state) {
        case Aki::Irc::Socket::UnconnectedState: {
            Aki::BaseWindow *base = currentFocusedChannel();
            if (base && base->view()) {
                base->view()->addMessage(i18n("Disconnecting"));
            }

            foreach (Aki::BaseWindow *window, mainView->windows()) {
                switch (window->windowType()) {
                case Aki::BaseWindow::ChannelWindow: {
                    Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
                    foreach (Aki::Irc::User *user, channel->users()) {
                        channel->removeUser(user);
                    }
                    break;
                }
                default: {
                    break;
                }
                }
            }
            break;
        }
        case Aki::Irc::Socket::ConnectingState: {
            Aki::BaseWindow *base = findChannel(q->socket()->name());
            if (base && base->view()) {
                base->view()->addMessage(i18n("Looking up server %1 on port %2", q->socket()->currentAddress(),
                                         QString::number(q->socket()->currentPort())));

                QHostInfo host = QHostInfo::fromName(q->socket()->currentAddress());
                QHostAddress address = host.addresses()[0];
                base->view()->addMessage(i18n("Connecting to %1(%2) on port %3", q->socket()->currentAddress(),
                                         address.toString(), QString::number(q->socket()->currentPort())));
            }
            break;
        }
        case Aki::Irc::Socket::ConnectedState: {
            Aki::StatusWindow *base = qobject_cast<Aki::StatusWindow*>(findChannel(q->socket()->name()));
            if (base && base->view()) {
                base->view()->addMessage(i18n("Connected to %1:%2", q->socket()->currentAddress(),
                                         QString::number(q->socket()->currentPort())));
                base->view()->addMessage(i18n("Using identity profile: %1", base->identity()->name()));
            }
            break;
        }
        default: {
            break;
        }
        }
    }

    void onAway(const QString &nick, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window->view()) {
            window->view()->addAway(nick, message);
        }
    }

    void onBanList(const QString &channel, const QString &mask, const QString &who,
                   const QDateTime &time)
    {
        Q_UNUSED(channel);
        Q_UNUSED(mask);
        Q_UNUSED(who);
        Q_UNUSED(time);
    }

    void onChannelCreated(const QString &channel, const QDateTime &time)
    {
        Aki::BaseWindow *window = findChannel(channel.toLower());
        if (window && window->view()) {
            if (!Aki::Settings::hideChannelCreation()) {
                KDateTime dt(time);
                dt = dt.toClockTime();
                window->view()->addChannelCreated(dt.toString("%B %d, %Y %H:%M:%S"));
            }
        }
    }

    void onChannelModeIs(const QString &channel, const QString &modes,
                         const QStringList &params)
    {
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(channel.toLower()));
        if (window && window->view()) {
            QStringList modeStrs;
            int indexParams = 0;

            foreach (QChar ch, modes) {
                if (ch == QChar('t')) {
                    modeStrs << i18n("Topic protection");
                    window->modeBar()->enableTopicProtectionButton(true);
                } else if (ch == QChar('n')) {
                    modeStrs << i18n("No outside messages");
                    window->modeBar()->enableNoOutsideMessagesButton(true);
                } else if (ch == QChar('s')) {
                    modeStrs << i18n("Secret");
                    window->modeBar()->enableSecretButton(true);
                } else if (ch == QChar('i')) {
                    modeStrs << i18n("Invite only");
                    window->modeBar()->enableInviteOnlyButton(true);
                } else if (ch == QChar('p')) {
                    modeStrs << i18n("Private");
                    window->modeBar()->enablePrivateButton(true);
                } else if (ch == QChar('m')) {
                    modeStrs << i18n("Moderated");
                    window->modeBar()->enableModeratedButton(true);
                } else if (ch == QChar('b')) {
                } else if (ch == QChar('k')) {
                    QString param = params[indexParams++];
                    modeStrs << i18n("Password protected: %1", param);
                    window->modeBar()->enableChannelKeyButton(true);
                    window->modeBar()->setKey(param);
                } else if (ch == QChar('l')) {
                    QString param = params[indexParams++];
                    modeStrs << i18n("Limited to %1 users", param);
                    window->modeBar()->enableChannelLimitButton(true);
                    window->modeBar()->setLimit(param.toInt());
                }
            }

            if (!Aki::Settings::hideChannelModes()) {
                window->view()->addChannelModes(modeStrs.join(", "));
            }
        }
    }

    void onChannelUrlIs(const QString &channel, const QString &url)
    {
        Aki::BaseWindow *window = findChannel(channel.toLower());
        if (window && window->view()) {
            window->view()->addChannelUrl(url);
        }
    }

    void onCreated(const QString &message)
    {
        Aki::BaseWindow *window = findChannel(q->socket()->name());
        if (window && window->view()) {
            window->view()->addWelcome(message);
        }
    }

    void onCtcpAction(const QString &from, const QString &to, const QString &message)
    {
        Aki::BaseWindow *window = findChannel(to);
        if (window && window->view()) {
            window->view()->addCtcpAction(from, message);
        }
    }

    void onCtcpRequest(const QString &from, const QString &type)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addCtcpRequest(from, type);
        }
    }

    void onEndOfMotd(const QString &message)
    {
        Aki::BaseWindow *window = findChannel(q->socket()->name());
        if (window && window->view()) {
            window->view()->addMotd(message);
        }
    }

    void onEndOfNames(const QString &channel, const QString &message)
    {
        Q_UNUSED(channel);
        Q_UNUSED(message);
    }

    void onEndOfWho(const QString &channel, const QString &message)
    {
        Aki::ChannelWindow *chan = qobject_cast<Aki::ChannelWindow*>(findChannel(channel.toLower()));

        if (!whoRequest.isEmpty()) {
            const int index = whoRequest.indexOf(channel);

            if (index > -1) {
                Aki::BaseWindow *window = currentFocusedChannel();

                if (window && window->view()) {
                    window->view()->addWho(channel, message);
                } else {
                    chan->resetWho();
                }
                whoRequest.removeAt(index);
            }
        }

        if (chan) {
            chan->resetWho();
        }
    }

    void onEndOfWhoIs(const QString &nick, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addWhoIs(nick, message);
        }
    }

    void onEndOfWhoWas(const QString &nick, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addWhoWas(nick, message);
        }
    }

    void onErrorAlreadyRegistered(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorBadChanMask(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorBadChannelKey(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorBanListFull(const QString &channel, const QString &ban,
                            const QString &message)
    {
        Aki::BaseWindow *window = findChannel(channel.toLower());
        if (window && window->view()) {
            window->view()->addError(QString("%1 %2 %3").arg(channel, ban, message));
        }
    }

    void onErrorBannedFromChannel(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorCannotSendToChannel(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = findChannel(channel.toLower());
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorCantKillServer(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorChannelIsFull(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorChannelPrivilegesNeeded(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorErroneusNickname(const QString &nick, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(nick, message);
        }
    }

    void onErrorFileError(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorInviteOnlyChannel(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorKeySet(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorNeedMoreParams(const QString &command, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(command, message);
        }
    }

    void onErrorNickCollision(const QString &nick, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(nick, message);
        }
    }

    void onErrorNicknameInUse(const QString &nick, const QString &message)
    {
        Aki::BaseWindow *base = currentFocusedChannel();
        if (base && base->view()) {
            base->view()->addError(nick, message);
        }

        QString nextNick = q->socket()->nextAvailableNick();
        q->socket()->setCurrentNick(nextNick);
        q->socket()->rfcNick(nextNick);

        foreach (Aki::BaseWindow *window, splitView->windows()) {
            if (window) {
                switch (window->windowType()) {
                case Aki::BaseWindow::ChannelWindow: {
                    Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
                    channel->setNewNick(nextNick);
                    break;
                }
                case Aki::BaseWindow::StatusWindow: {
                    Aki::StatusWindow *status = qobject_cast<Aki::StatusWindow*>(window);
                    status->setNewNick(nextNick);
                    break;
                }
                case Aki::BaseWindow::QueryWindow: {
                    Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);
                    query->selfUser()->setNick(nextNick);
                    break;
                }
                default: {
                    break;
                }
                }
            }
        }

        foreach (Aki::BaseWindow *window, mainView->windows()) {
            if (window) {
                switch (window->windowType()) {
                case Aki::BaseWindow::ChannelWindow: {
                    Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
                    channel->setNewNick(nextNick);
                    break;
                }
                case Aki::BaseWindow::StatusWindow: {
                    Aki::StatusWindow *status = qobject_cast<Aki::StatusWindow*>(window);
                    status->setNewNick(nextNick);
                    break;
                }
                case Aki::BaseWindow::QueryWindow: {
                    Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);
                    query->selfUser()->setNick(nextNick);
                    break;
                }
                default: {
                    break;
                }
                }
            }
        }
    }

    void onErrorNoAdminInfo(const QString &server, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(server, message);
        }
    }

    void onErrorNoChanModes(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = findChannel(channel.toLower());
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorNoLogin(const QString &user, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(user, message);
        }
    }

    void onErrorNoMotd(const QString &message)
    {
        Aki::BaseWindow *window = findChannel(q->socket()->name());
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorNoNicknameGiven(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorNoOperatorHost(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorNoPrivileges(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorNoRecipient(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorNoSuchChannel(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorNoSuchNick(const QString &nick, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(nick, message);
        }
    }

    void onErrorNoSuchServer(const QString &server, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(server, message);
        }
    }

    void onErrorNoSuchService(const QString &service, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(service, message);
        }
    }

    void onErrorNoTextToSend(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorNotOnChannel(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorNoTopLevel(const QString &mask, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(mask, message);
        }
    }

    void onErrorPasswordMismatch(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorRestricted(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorSummonDisabled(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorTooManyChannels(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(channel, message);
        }
    }

    void onErrorUModeUnknownFlag(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorUniqueOpPrivilegesNeeded(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorUnknownCommand(const QString &command, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(command, message);
        }
    }

    void onErrorUnknownMode(const QString &mode, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(mode, message);
        }
    }

    void onErrorUserOnChannel(const QString &nick, const QString &channel,
                              const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(QString("%1 %2 %3").arg(nick, channel, message));
        }
    }

    void onErrorUsersDisabled(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorUsersDontMatch(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onErrorWasNoSuchNick(const QString &nick, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(nick, message);
        }
    }

    void onErrorWildTopLevel(const QString &mask, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(mask, message);
        }
    }

    void onErrorYoureBannedCreep(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addError(message);
        }
    }

    void onGlobalUsers(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addLUser(message);
        }
    }

    void onHighConnectCount(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addLUser(message);
        }
    }

    void onInvite(const QString &from, const QString &channel)
    {
        int response =
        KMessageBox::questionYesNo(0, i18n("%1 has invited you to join channel: %2.\n"
                                              "Would you like to accept the invitation?", from, channel),
                                              i18n("Received a channel invitation"));
        if (response == KMessageBox::Yes) {
            q->socket()->rfcJoin(channel);
        }
    }

    void onInviting(const QString &nick, const QString &channel)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addInvite(nick, channel);
        }
    }

    void onIsOn(const QStringList &nicks)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addIsOn(nicks);
        }
    }

    void onISupport(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addSupport(message);
        }
    }

    void onKick(const QString &channel, const QString &nick, const QString &message)
    {
        Q_UNUSED(channel);
        Q_UNUSED(nick);
        Q_UNUSED(message);
    }

    void onLocalUsers(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addLUser(message);
        }
    }

    void onLUserChannels(int count, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addLUser(QString::number(count) + QChar(' ') + message);
        }
    }

    void onLUserClient(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addLUser(message);
        }
    }

    void onLUserMe(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addLUser(message);
        }
    }

    void onLUserOp(int count, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addLUser(QString::number(count) + QChar(' ') + message);
        }
    }

    void onLUserUnknown(int count, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addLUser(QString::number(count) + QChar(' ') + message);
        }
    }

    void onMode(const QString &from, const QString &channel, const QString &modes,
                const QString &nick)
    {
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(channel.toLower()));
        if (window && window->view()) {
            enum {
                None,
                Give,
                Take
            };
            int state = None;

            bool fromYou = (from == window->socket()->currentNick());
            bool toYou = (nick == window->socket()->currentNick());

            if (!window->isCurrent()) {
                if (toYou) {
                    window->setTabColor(Aki::BaseWindow::Highlight);
                } else {
                    window->setTabColor(Aki::BaseWindow::NewData);
                }
            }

            foreach (QChar ch, modes) {
                if (ch == QChar('+')) {
                    state = Give;
                } else if (ch == QChar('-')) {
                    state = Take;
                } else if (ch == QChar('o')) {
                    if (state == Give) {
                        window->view()->addMode(from, nick, ch, toYou, fromYou);
                        window->addMode(nick, ch);
                    } else if (state == Take) {
                        window->view()->addRemoveMode(from, nick, ch, toYou, fromYou);
                        window->removeMode(nick, ch);
                    }
                } else if (ch == QChar('h')) {
                    if (state == Give) {
                        window->view()->addMode(from, nick, ch, toYou, fromYou);
                        window->addMode(nick, ch);
                    } else if (state == Give) {
                        window->view()->addRemoveMode(from, nick, ch, toYou, fromYou);
                        window->removeMode(nick, ch);
                    }
                } else if (ch == QChar('v')) {
                    if (state == Give) {
                        window->view()->addMode(from, nick, ch, toYou, fromYou);
                        window->addMode(nick, ch);
                    } else if (state == Take) {
                        window->view()->addRemoveMode(from, nick, ch, toYou, fromYou);
                        window->removeMode(nick, ch);
                    }
                } else if (ch == QChar('b')) {
                    if (state == Give) {
                        window->view()->addBan(from, nick);
                    } else if (state == Take) {
                        window->view()->addRemoveBan(from, nick);
                    }
                } else if (ch == 'i') {
                    if (state == Give) {
                        window->view()->addChannelMode(from, ch, QString(), fromYou);
                    } else if (state == Take) {
                        window->view()->addRemoveMode(from, ch, fromYou);
                    }
                } else if (ch == QChar('m')) {
                    if (state == Give) {
                        window->view()->addChannelMode(from, ch, QString(), fromYou);
                    } else if (state == Take) {
                        window->view()->addRemoveMode(from, ch, fromYou);
                    }
                } else if (ch == QChar('p')) {
                    if (state == Give) {
                        window->view()->addChannelMode(from, ch, QString(), fromYou);
                    } else if (state == Take) {
                        window->view()->addRemoveMode(from, ch, fromYou);
                    }
                } else if (ch == QChar('s')) {
                    if (state == Give) {
                        window->view()->addChannelMode(from, ch, QString(), fromYou);
                    } else if (state == Take) {
                        window->view()->addRemoveMode(from, ch, fromYou);
                    }
                } else if (ch == QChar('t')) {
                    if (state == Give) {
                        window->view()->addChannelMode(from, ch, QString(), fromYou);
                    } else if (state == Take) {
                        window->view()->addRemoveMode(from, ch, fromYou);
                    }
                } else if (ch == QChar('n')) {
                    if (state == Give) {
                        window->view()->addChannelMode(from, ch, QString(), fromYou);
                    } else if (state == Take) {
                        window->view()->addRemoveMode(from, ch, fromYou);
                    }
                } else if (ch == QChar('l')) {
                    if (state == Give) {
                        window->view()->addChannelMode(from, ch, QString(), fromYou);
                    } else if (state == Take) {
                        window->view()->addRemoveMode(from, ch, fromYou);
                    }
                } else if (ch == QChar('k')) {
                    if (state == Give) {
                        window->view()->addChannelMode(from, ch, QString(), fromYou);
                    } else if (state == Take) {
                        window->view()->addRemoveMode(from, ch, fromYou);
                    }
                }
            }
        }
    }

    void onMode(const QString &from, const QString &channel, const QString &modes,
                const QStringList &params)
    {
        Q_UNUSED(from);
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(channel.toLower()));
        if (window && window->view()) {
            QStringList modeStrs;
            int indexParams = 0;

            foreach (QChar ch, modes) {
                if (ch == QChar('t')) {
                    modeStrs << i18n("Topic protection");
                    window->modeBar()->enableTopicProtectionButton(true);
                } else if (ch == QChar('n')) {
                    modeStrs << i18n("No outside messages");
                    window->modeBar()->enableNoOutsideMessagesButton(true);
                } else if (ch == QChar('s')) {
                    modeStrs << i18n("Secret");
                    window->modeBar()->enableSecretButton(true);
                } else if (ch == QChar('i')) {
                    modeStrs << i18n("Invite only");
                    window->modeBar()->enableInviteOnlyButton(true);
                } else if (ch == QChar('p')) {
                    modeStrs << i18n("Private");
                    window->modeBar()->enablePrivateButton(true);
                } else if (ch == QChar('m')) {
                    modeStrs << i18n("Moderated");
                    window->modeBar()->enableModeratedButton(true);
                } else if (ch == QChar('b')) {
                } else if (ch == QChar('k')) {
                    QString param = params[indexParams++];
                    modeStrs << i18n("Password protected: %1", param);
                    window->modeBar()->enableChannelKeyButton(true);
                    window->modeBar()->setKey(param);
                } else if (ch == QChar('l')) {
                    QString param = params[indexParams++];
                    modeStrs << i18n("Limited to %1 users", param);
                    window->modeBar()->enableChannelLimitButton(true);
                    window->modeBar()->setLimit(param.toInt());
                }
            }

            if (!Aki::Settings::hideChannelModes()) {
                window->view()->addChannelModes(modeStrs.join(", "));
            }
        }
    }

    void onMotd(const QString &message)
    {
        Aki::BaseWindow *window = findChannel(q->socket()->name());
        if (window && window->view()) {
            window->view()->addMotd(message);
        }
    }

    void onMotdStart(const QString &message)
    {
        Aki::BaseWindow *window = findChannel(q->socket()->name());
        if (window && window->view()) {
            window->view()->addMotd(message);
        }
    }

    void onMyInfo(const QString &message)
    {
        Aki::BaseWindow *window = findChannel(q->socket()->name());
        if (window && window->view()) {
            window->view()->addMotd(message);
        }
    }

    void onNameReply(const QString &channel, const QString &names)
    {
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(channel.toLower()));
        if (window && window->view()) {
            QStringList tmp = names.split(QChar(' '), QString::SkipEmptyParts);
            QStringListIterator iter(tmp);

            while (iter.hasNext()) {
                QString nick = iter.next();

                Aki::Irc::User *user = new Aki::Irc::User(QString(), window);
                user->setColor(randomColor());

                if (nick.left(1) == QChar('@')) {
                    user->setNick(nick.remove(QChar('@')));
                    if (user->nick() == window->socket()->currentNick()) {
                        window->modeBar()->setEnabled(true);
                    }
                    window->addUser(user);
                } else if (nick.left(1) == QChar('+')) {
                    user->setNick(nick.remove(QChar('+')));
                    window->addUser(user);
                } else if (nick.left(1) == QChar('%')) {
                    user->setNick(nick.remove(QChar('%')));
                    window->addUser(user);
                } else {
                    user->setNick(nick);
                    window->addUser(user);
                }
            }
        }
    }

    void onNick(const QString &oldNick, const QString &newNick)
    {
        foreach (Aki::BaseWindow *window, splitView->windows()) {
            switch (window->windowType()) {
            case Aki::BaseWindow::StatusWindow: {
                Aki::StatusWindow *status = qobject_cast<Aki::StatusWindow*>(window);
                status->addNick(oldNick, newNick);
                break;
            }
            case Aki::BaseWindow::QueryWindow: {
                Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);
                query->addNick(oldNick, newNick);
                break;
            }
            case Aki::BaseWindow::ChannelWindow: {
                Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
                channel->addNick(oldNick, newNick);
                break;
            }
            default: {
                break;
            }
            }
        }

        foreach (Aki::BaseWindow *window, mainView->windows()) {
            switch (window->windowType()) {
            case Aki::BaseWindow::StatusWindow: {
                Aki::StatusWindow *status = qobject_cast<Aki::StatusWindow*>(window);
                status->addNick(oldNick, newNick);
                break;
            }
            case Aki::BaseWindow::QueryWindow: {
                Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);
                query->addNick(oldNick, newNick);
                break;
            }
            case Aki::BaseWindow::ChannelWindow: {
                Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
                channel->addNick(oldNick, newNick);
                break;
            }
            default: {
                break;
            }
            }
        }
    }

    void onNotice(const QString &from, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addNotice(from, Aki::Irc::Color::toHtml(message));

            if (Aki::Irc::Rfc2812::isValidNickname(from)) {
                if (from == "NickServ" && message.contains("This nickname is registered", Qt::CaseInsensitive)) {
                    if (q->socket()->serviceName().toLower().trimmed() == "nickserv" &&
                        !q->socket()->servicePassword().isEmpty() && q->socket()->isAutoIdentifyEnabled()) {
                        q->socket()->rfcPrivmsg("NickServ", "identify " + q->socket()->servicePassword().toLatin1());
                    }
                    if (!q->socket()->channelList().isEmpty()) {
                        q->socket()->rfcJoin(q->socket()->channelList());
                    }
                }
            }

            if (!window->isCurrent()) {
                window->setTabColor(Aki::BaseWindow::NewData);
            }
        }
    }

    void onNoticeAuth(const QString &message)
    {
        Aki::BaseWindow *window = findChannel(q->socket()->name());
        if (window && window->view()) {
            window->view()->addNoticeAuth(Aki::Irc::Color::toHtml(message));
        }
    }

    void onNoticeCtcpVersion(const QString &nick, const QString &version)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addCtcp(nick, Aki::Irc::Color::toHtml(version));
        }
    }

    void onNoTopic(const QString &channel, const QString &message)
    {
        Aki::BaseWindow *window = findChannel(channel.toLower());
        if (window && window->view()) {
            window->view()->addTopic(Aki::Irc::Color::toHtml(message));
        }
    }

    void onNowAway(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addAway(message);
        }
    }

    void onPong(const QString &server, const QString &server2)
    {
        Q_UNUSED(server)
        Q_UNUSED(server2)
    }

    void onPrivmsg(const QString &channel, const QString &from, const QString &to,
                   const QString &message)
    {
        if (!channel.isEmpty()) {
            Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(channel.toLower()));
            if (window && window->view()) {
                if (q->identity()->isMarkLastPositionEnabled()) {
                    if (currentFocusedChannel() != window) {
                        window->view()->insertMarker();
                    }
                }
                window->addMessage(from, message);
            
            }
        } else if (!to.isEmpty()) {
            Aki::QueryWindow *window = qobject_cast<Aki::QueryWindow*>(findChannel(from.toLower()));
            if (window && window->view()) {
                window->addMessage(from, message);
            } else {
                Aki::Irc::User *self = new Aki::Irc::User("", q);
                self->setNick(to);
                self->setColor(randomColor());

                Aki::Irc::User *other = new Aki::Irc::User("", q);
                other->setNick(from);
                other->setColor(randomColor());

                mainView->addQuery(self, other, message);
            }
        }
    }

    void onSelfJoin(const QString &channel)
    {
        mainView->addChannel(channel);
        autoRequest.insert("WHO", channel.toLower());

        if (!rejoinChannels.contains(channel.toLower())) {
            rejoinChannels << channel.toLower();
        }
    }

    void onSelfUMode(const QString &modes)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addSelfUModeSet(modes);
        }
    }

    void onTime(const QString &server, const QString &time)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addTime(server, time);
        }
    }

    void onTopic(const QString &channel, const QString &topic)
    {
        Aki::BaseWindow *window = findChannel(channel.toLower());
        if (window && window->view()) {
            switch (window->windowType()) {
            case Aki::BaseWindow::ChannelWindow: {
                Aki::ChannelWindow *chan = qobject_cast<Aki::ChannelWindow*>(window);
                chan->setTopic(topic);
                if (!Aki::Settings::hideTopic()) {
                    chan->view()->addTopic(Aki::Irc::Color::toHtml(topic));
                }
                break;
            }
            default: {
                break;
            }
            }
        }
    }

    void onTopicChanged(const QString &nick, const QString &channel, const QString &topic)
    {
        Aki::BaseWindow *window = findChannel(channel.toLower());
        if (window && window->view()) {
            switch (window->windowType()) {
            case Aki::BaseWindow::ChannelWindow: {
                Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
                channel->setTopic(topic);
                channel->view()->addTopicChanged(nick, Aki::Irc::Color::toHtml(topic));
                if (!channel->isCurrent()) {
                    channel->setTabColor(Aki::BaseWindow::NewData);
                }
                break;
            }
            default: {
                break;
            }
            }
        }
    }

    void onTopicSetBy(const QString &nick, const QString &channel, const QDateTime &time)
    {
        Aki::BaseWindow *window = findChannel(channel.toLower());
        if (window && window->view()) {
            KDateTime dt(time);
            dt = dt.toClockTime();
            window->view()->addTopicSetBy(nick, dt.toString("%B %d, %Y %H:%M:%S"));
        }
    }

    void onUMode(const QString &nick, const QString &modes)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addUMode(nick, modes);
        }
    }

    void onUModeIs(const QString &modes)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addUMode(modes);
        }
    }

    void onUnAway(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addAway(message);
        }
    }

    void onUserHost(const QStringList &nicks, const QStringList &hosts)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addUserHost(nicks, hosts);
        }
    }

    void onUserJoin(const QString &hostMask, const QString &channel)
    {
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(channel.toLower()));
        if (window && window->view()) {
            const QString nick = Aki::Irc::Rfc2812::nickFromMask(hostMask);
            const QString user = Aki::Irc::Rfc2812::userFromMask(hostMask);
            const QString host = Aki::Irc::Rfc2812::hostFromMask(hostMask);

            Aki::Irc::User *ircUser = new Aki::Irc::User(hostMask, q);
            ircUser->setColor(randomColor());
            window->addUser(ircUser);

            if (!Aki::Settings::conferenceMode()) {
                window->view()->addUserJoin(channel, nick, hostMask);
            }

            if (!window->isCurrent()) {
                window->setTabColor(Aki::BaseWindow::NewData);
            }
        }
    }

    void onUserPart(const QString &hostMask, const QString &channel, const QString &message)
    {
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(channel.toLower()));
        if (window && window->view()) {
            const QString nick = Aki::Irc::Rfc2812::nickFromMask(hostMask);
            const QString user = Aki::Irc::Rfc2812::userFromMask(hostMask);
            const QString host = Aki::Irc::Rfc2812::hostFromMask(hostMask);

            foreach (Aki::Irc::User *user, window->users()) {
                if (user->nick() == nick) {
                    window->removeUser(user);

                    if (!Aki::Settings::conferenceMode()) {
                        window->view()->addUserPart(channel, nick, hostMask, Aki::Irc::Color::toHtml(message));
                    }

                    if (!window->isCurrent()) {
                        window->setTabColor(Aki::BaseWindow::NewData);
                    }
                    return;
                } else if (user->nick() == q->socket()->currentNick()) {
                    rejoinChannels.removeAll(channel.toLower());
                }
            }
        }
    }

    void onUserQuit(const QString &hostMask, const QString &message)
    {
        const QString nick = Aki::Irc::Rfc2812::nickFromMask(hostMask);
        const QString user = Aki::Irc::Rfc2812::userFromMask(hostMask);
        const QString host = Aki::Irc::Rfc2812::hostFromMask(hostMask);

        foreach (Aki::BaseWindow *base, splitView->windows()) {
            if (base && base->windowType() == Aki::BaseWindow::ChannelWindow) {
                Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(base);
                foreach (Aki::Irc::User *user, channel->users()) {
                    if (user->nick() == nick) {
                        channel->removeUser(user);

                        if (!Aki::Settings::conferenceMode()) {
                            channel->view()->addUserQuit(nick, hostMask, message);
                        }

                        if (!channel->isCurrent()) {
                            channel->setTabColor(Aki::BaseWindow::NewData);
                        }
                        break;
                    }
                }
            }
        }
        foreach (Aki::BaseWindow *base, mainView->windows()) {
            if (base && base->windowType() == Aki::BaseWindow::ChannelWindow) {
                Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(base);
                foreach (Aki::Irc::User *user, channel->users()) {
                    if (user->nick() == nick) {
                        channel->removeUser(user);

                        if (!Aki::Settings::conferenceMode()) {
                            channel->view()->addUserQuit(nick, hostMask, message);
                        }

                        if (!channel->isCurrent()) {
                            channel->setTabColor(Aki::BaseWindow::NewData);
                        }
                        break;
                    }
                }
            }
        }
    }

    void onVersion(const QString &message)
    {
        Aki::BaseWindow *window = findChannel(q->socket()->name());
        if (window && window->view()) {
            window->view()->addVersion(message);
        }
    }

    void onWelcome(const QString &message)
    {
        Aki::BaseWindow *window = findChannel(q->socket()->name());
        if (window && window->view()) {
            window->view()->addWelcome(message);
        }
    }

    void onWho(const QString &channel, const QString &identName, const QString &address,
               const QString &server, const QString &nick, const QString &flags,
               int hops, const QString &realName)
    {
        if (channel == QChar('*')) {
            foreach (Aki::BaseWindow *window, splitView->windows()) {
                if (window && window->windowType() == Aki::BaseWindow::QueryWindow) {
                    Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);
                    query->addWho(channel, identName, address, server, nick, flags, hops, realName);
                }
            }

            foreach (Aki::BaseWindow *window, mainView->windows()) {
                if (window && window->windowType() == Aki::BaseWindow::QueryWindow) {
                    Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);
                    query->addWho(channel, identName, address, server, nick, flags, hops, realName);
                }
            }
            return;
        }

        if (!whoRequest.isEmpty() && channel.toLower() == whoRequest.front().toLower()) {
            Aki::BaseWindow *current = currentFocusedChannel();
            current->view()->addWho(channel, identName, address, server, nick, flags, hops, realName);
        }

        Aki::BaseWindow *window = findChannel(channel.toLower());
        if (window && window->view()) {
            if (autoRequest.contains("WHO", channel.toLower())) {
                if (window->windowType() == Aki::BaseWindow::ChannelWindow) {
                    Aki::ChannelWindow *chan = qobject_cast<Aki::ChannelWindow*>(window);
                    chan->setIsWhoRunning(true);
                    chan->addWho(channel, identName, address, server, nick, flags, hops, realName);
                }
            }
        }

        foreach (Aki::BaseWindow *window, splitView->windows()) {
            if (window && window->windowType() == Aki::BaseWindow::QueryWindow) {
                Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);
                if (nick == query->otherUser()->nick() || nick == query->selfUser()->nick()) {
                    query->addWho(channel, identName, address, server, nick, flags, hops, realName);
                }
            }
        }

        foreach (Aki::BaseWindow *window, mainView->windows()) {
            if (window && window->windowType() == Aki::BaseWindow::QueryWindow) {
                Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);
                if (nick == query->otherUser()->nick() || nick == query->selfUser()->nick()) {
                    query->addWho(channel, identName, address, server, nick, flags, hops, realName);
                }
            }
        }
    }

    void onWhoIsChannels(const QString &nick, const QString &channels)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addWhoIsChannels(nick, channels);
        }
    }

    void onWhoIsIdentified(const QString &nick, const QString &info)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addWhoIsIdentified(nick, info);
        }
    }

    void onWhoIsIdle(const QString &nick, const QDateTime &idleTime,
                     const QDateTime &signon, const QString &info)
    {
        Q_UNUSED(info);
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            KDateTime dt(signon);
            dt = dt.toClockTime();
            window->view()->addWhoIsIdle(nick, idleTime.toString("%H:%M:%S"),
                                         dt.toString("%A %B %d %H:%M:%S"));
        }
    }

    void onWhoIsOperator(const QString &nick, const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addWhoIs(nick, message);
        }
    }

    void onWhoIsServer(const QString &nick, const QString &server, const QString &info)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addWhoIsServer(nick, server, info);
        }
    }

    void onWhoIsUser(const QString &nick, const QString &userName, const QString &address,
                     const QString &info)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addWhoIsUser(nick, userName, address, info);
        }
    }

    void onWhoWasUser(const QString &nick, const QString &userName, const QString &address,
                      const QString &info)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addWhoWasUser(nick, userName, address, info);
        }
    }

    void onWriteLine(bool in, const QString &line)
    {
        messageLog->addMessage(in, line);
    }

    void onYoureOper(const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->addMessage(message);
        }
    }

    void onYourHost(const QString &message)
    {
        Aki::BaseWindow *window = findChannel(q->socket()->name());
        if (window && window->view()) {
            window->view()->addWelcome(Aki::Irc::Color::toHtml(message));
        }
    }

    void on901(const QString &id, const QString &identName, const QString &address,
               const QString &message)
    {
        Aki::BaseWindow *window = currentFocusedChannel();
        if (window && window->view()) {
            window->view()->add901(id, identName, address, message);
        }
    }

    QColor randomColor() const
    {
        QColor color;

        switch (qrand() % 7) {
        case 0: {
            color = Aki::Settings::userColor0();
            break;
        }
        case 1: {
            color = Aki::Settings::userColor1();
            break;
        }
        case 2: {
            color = Aki::Settings::userColor2();
            break;
        }
        case 3: {
            color = Aki::Settings::userColor3();
            break;
        }
        case 4: {
            color = Aki::Settings::userColor4();
            break;
        }
        case 5: {
            color = Aki::Settings::userColor5();
            break;
        }
        case 6: {
            color = Aki::Settings::userColor6();
            break;
        }
        default:{
            break;
        }
    }

    return color;
    }

    void whoRequested(const QString &channel)
    {
        whoRequest.append(channel);
        q->socket()->rfcWho(channel);

        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(channel.toLower()));
        if (window) {
            window->resetWho();
        }
    }

    void queryMessage(const QString &to, const QString &message)
    {
        Aki::QueryWindow *window = qobject_cast<Aki::QueryWindow*>(findChannel(to.toLower()));

        if (window && window->view()) {
            window->addMessage(q->socket()->currentNick(), message);
            if (to != q->socket()->currentNick()) {
                window->socket()->rfcPrivmsg(window->otherUser()->nick().toLatin1(), message.toUtf8());
            }
        } else {
            Aki::Irc::User *self = new Aki::Irc::User("", q);
            self->setNick(q->socket()->currentNick());
            self->setColor(randomColor());

            Aki::Irc::User *other = new Aki::Irc::User("", q);
            other->setNick(to);
            other->setColor(randomColor());

            mainView->addQuery(self, other, message, to != q->socket()->currentNick() ? true : false);
        }
    }

    ServerWindow *q;
    Aki::IdentityConfig *identityConfig;
    Aki::ChannelView *mainView;
    Aki::ChannelView *splitView;
    Aki::ChatParser *parser;
    Aki::MessageLog *messageLog;
    QMultiMap<QString,QString> autoRequest;
    QStringList whoRequest;
    QStringList rejoinChannels;
    QSplitter *splitter;
}; // End of class ServerWindowPrivate.
} // End of namespace Aki.

ServerWindow::ServerWindow(Aki::IdentityConfig *identityConfig, Aki::Irc::Socket *socket,
                           Aki::Notifications *notifications, Aki::MessageLog *messageLog,
                           QWidget *parent)
    : Aki::BaseWindow(socket->name(), Aki::BaseWindow::ServerWindow, parent)
{
    d.reset(new Aki::ServerWindowPrivate(this));

    Aki::Irc::Ctcp *ctcp = new Aki::Irc::Ctcp(this);
    ctcp->setVersionString(Aki::Settings::ctcpVersionMessage());
    ctcp->blockClientInfo(Aki::Settings::ignoreClientInfo());
    ctcp->blockDcc(Aki::Settings::ignoreDcc());
    ctcp->blockPing(Aki::Settings::ignorePing());
    ctcp->blockSource(Aki::Settings::ignoreSource());
    ctcp->blockTime(Aki::Settings::ignoreTime());
    ctcp->blockUserInfo(Aki::Settings::ignoreUserInfo());
    ctcp->blockVersion(Aki::Settings::ignoreVersion());
    socket->setCtcp(ctcp);

    setNotifications(notifications);
    setSocket(socket);
    d->identityConfig = identityConfig;
    d->parser = new Aki::ChatParser(this);
    connect(d->parser, SIGNAL(customCommand(QString,QString)),
            SIGNAL(customCommand(QString,QString)));

    d->messageLog = messageLog;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    d->mainView = new Aki::ChannelView(identityConfig, socket, d->parser, notifications,
                                       this);
    d->splitView = new Aki::ChannelView(identityConfig, this);
    d->splitView->hide();

    d->splitter = new QSplitter(Qt::Vertical, this);
    d->splitter->addWidget(d->splitView);
    d->splitter->addWidget(d->mainView);

    mainLayout->addWidget(d->splitter);

    setLayout(mainLayout);

    connect(d->mainView, SIGNAL(dropSuccessful(Aki::BaseWindow*)),
            d->splitView, SLOT(checkChannelDrop(Aki::BaseWindow*)));
    connect(d->splitView, SIGNAL(dropSuccessful(Aki::BaseWindow*)),
            d->mainView, SLOT(checkChannelDrop(Aki::BaseWindow*)));
    connect(this, SIGNAL(dropSuccessful(Aki::BaseWindow*)),
            d->mainView, SLOT(checkChannelDrop(Aki::BaseWindow*)));
    connect(socket, SIGNAL(stateChanged(Aki::Irc::Socket::SocketState)),
            SLOT(stateChanged(Aki::Irc::Socket::SocketState)));
    connect(socket, SIGNAL(onAway(QString,QString)),
            SLOT(onAway(QString,QString)));
    connect(socket, SIGNAL(onBanList(QString,QString,QString,QDateTime)),
            SLOT(onBanList(QString,QString,QString,QDateTime)));
    connect(socket, SIGNAL(onChannelCreated(QString,QDateTime)),
            SLOT(onChannelCreated(QString,QDateTime)));
    connect(socket, SIGNAL(onChannelModeIs(QString,QString,QStringList)),
            SLOT(onChannelModeIs(QString,QString,QStringList)));
    connect(socket, SIGNAL(onChannelUrlIs(QString,QString)),
            SLOT(onChannelUrlIs(QString,QString)));
    connect(socket, SIGNAL(onCreated(QString)),
            SLOT(onCreated(QString)));
    connect(socket, SIGNAL(onCtcpAction(QString,QString,QString)),
            SLOT(onCtcpAction(QString,QString,QString)));
    connect(socket, SIGNAL(onCtcpRequest(QString,QString)),
            SLOT(onCtcpRequest(QString,QString)));
    connect(socket, SIGNAL(onEndOfMotd(QString)),
            SLOT(onEndOfMotd(QString)));
    connect(socket, SIGNAL(onEndOfNames(QString,QString)),
            SLOT(onEndOfNames(QString,QString)));
    connect(socket, SIGNAL(onEndOfWho(QString,QString)),
            SLOT(onEndOfWho(QString,QString)));
    connect(socket, SIGNAL(onEndOfWhoIs(QString,QString)),
            SLOT(onEndOfWhoIs(QString,QString)));
    connect(socket, SIGNAL(onEndOfWhoWas(QString,QString)),
            SLOT(onEndOfWhoWas(QString,QString)));
    connect(socket, SIGNAL(onErrorAlreadyRegistered(QString)),
            SLOT(onErrorAlreadyRegistered(QString)));
    connect(socket, SIGNAL(onErrorBadChanMask(QString,QString)),
            SLOT(onErrorBadChanMask(QString,QString)));
    connect(socket, SIGNAL(onErrorBadChannelKey(QString,QString)),
            SLOT(onErrorBadChannelKey(QString,QString)));
    connect(socket, SIGNAL(onErrorBanListFull(QString,QString,QString)),
            SLOT(onErrorBanListFull(QString,QString,QString)));
    connect(socket, SIGNAL(onErrorBannedFromChannel(QString,QString)),
            SLOT(onErrorBannedFromChannel(QString,QString)));
    connect(socket, SIGNAL(onErrorCannotSendToChannel(QString,QString)),
            SLOT(onErrorCannotSendToChannel(QString,QString)));
    connect(socket, SIGNAL(onErrorCantKillServer(QString)),
            SLOT(onErrorCantKillServer(QString)));
    connect(socket, SIGNAL(onErrorChannelIsFull(QString,QString)),
            SLOT(onErrorChannelIsFull(QString,QString)));
    connect(socket, SIGNAL(onErrorChannelPrivilegesNeeded(QString,QString)),
            SLOT(onErrorChannelPrivilegesNeeded(QString,QString)));
    connect(socket, SIGNAL(onErrorErroneusNickname(QString,QString)),
            SLOT(onErrorErroneusNickname(QString,QString)));
    connect(socket, SIGNAL(onErrorFileError(QString)),
            SLOT(onErrorFileError(QString)));
    connect(socket, SIGNAL(onErrorInviteOnlyChannel(QString,QString)),
            SLOT(onErrorInviteOnlyChannel(QString,QString)));
    connect(socket, SIGNAL(onErrorKeySet(QString,QString)),
            SLOT(onErrorKeySet(QString,QString)));
    connect(socket, SIGNAL(onErrorNeedMoreParams(QString,QString)),
            SLOT(onErrorNeedMoreParams(QString,QString)));
    connect(socket, SIGNAL(onErrorNickCollision(QString,QString)),
            SLOT(onErrorNickCollision(QString,QString)));
    connect(socket, SIGNAL(onErrorNicknameInUse(QString,QString)),
            SLOT(onErrorNicknameInUse(QString,QString)));
    connect(socket, SIGNAL(onErrorNoAdminInfo(QString,QString)),
            SLOT(onErrorNoAdminInfo(QString,QString)));
    connect(socket, SIGNAL(onErrorNoChanModes(QString,QString)),
            SLOT(onErrorNoChanModes(QString,QString)));
    connect(socket, SIGNAL(onErrorNoLogin(QString,QString)),
            SLOT(onErrorNoLogin(QString,QString)));
    connect(socket, SIGNAL(onErrorNoMotd(QString)),
            SLOT(onErrorNoMotd(QString)));
    connect(socket, SIGNAL(onErrorNoNicknameGiven(QString)),
            SLOT(onErrorNoNicknameGiven(QString)));
    connect(socket, SIGNAL(onErrorNoOperatorHost(QString)),
            SLOT(onErrorNoOperatorHost(QString)));
    connect(socket, SIGNAL(onErrorNoPrivileges(QString)),
            SLOT(onErrorNoPrivileges(QString)));
    connect(socket, SIGNAL(onErrorNoRecipient(QString)),
            SLOT(onErrorNoRecipient(QString)));
    connect(socket, SIGNAL(onErrorNoSuchChannel(QString,QString)),
            SLOT(onErrorNoSuchChannel(QString,QString)));
    connect(socket, SIGNAL(onErrorNoSuchNick(QString,QString)),
            SLOT(onErrorNoSuchNick(QString,QString)));
    connect(socket, SIGNAL(onErrorNoSuchServer(QString,QString)),
            SLOT(onErrorNoSuchServer(QString,QString)));
    connect(socket, SIGNAL(onErrorNoSuchService(QString,QString)),
            SLOT(onErrorNoSuchService(QString,QString)));
    connect(socket, SIGNAL(onErrorNoTextToSend(QString)),
            SLOT(onErrorNoTextToSend(QString)));
    connect(socket, SIGNAL(onErrorNotOnChannel(QString,QString)),
            SLOT(onErrorNotOnChannel(QString,QString)));
    connect(socket, SIGNAL(onErrorNoTopLevel(QString,QString)),
            SLOT(onErrorNoTopLevel(QString,QString)));
    connect(socket, SIGNAL(onErrorPasswordMismatch(QString)),
            SLOT(onErrorPasswordMismatch(QString)));
    connect(socket, SIGNAL(onErrorRestricted(QString)),
            SLOT(onErrorRestricted(QString)));
    connect(socket, SIGNAL(onErrorSummonDisabled(QString)),
            SLOT(onErrorSummonDisabled(QString)));
    connect(socket, SIGNAL(onErrorTooManyChannels(QString,QString)),
            SLOT(onErrorTooManyChannels(QString,QString)));
    connect(socket, SIGNAL(onErrorUModeUnknownFlag(QString)),
            SLOT(onErrorUModeUnknownFlag(QString)));
    connect(socket, SIGNAL(onErrorUniqueOpPrivilegesNeeded(QString)),
            SLOT(onErrorUniqueOpPrivilegesNeeded(QString)));
    connect(socket, SIGNAL(onErrorUnknownCommand(QString,QString)),
            SLOT(onErrorUnknownCommand(QString,QString)));
    connect(socket, SIGNAL(onErrorUnknownMode(QString,QString)),
            SLOT(onErrorUnknownMode(QString,QString)));
    connect(socket, SIGNAL(onErrorUserOnChannel(QString,QString,QString)),
            SLOT(onErrorUserOnChannel(QString,QString,QString)));
    connect(socket, SIGNAL(onErrorUsersDisabled(QString)),
            SLOT(onErrorUsersDisabled(QString)));
    connect(socket, SIGNAL(onErrorUsersDontMatch(QString)),
            SLOT(onErrorUsersDontMatch(QString)));
    connect(socket, SIGNAL(onErrorWasNoSuchNick(QString,QString)),
            SLOT(onErrorWasNoSuchNick(QString,QString)));
    connect(socket, SIGNAL(onErrorWildTopLevel(QString,QString)),
            SLOT(onErrorWildTopLevel(QString,QString)));
    connect(socket, SIGNAL(onErrorYoureBannedCreep(QString)),
            SLOT(onErrorYoureBannedCreep(QString)));
    connect(socket, SIGNAL(onGlobalUsers(QString)),
            SLOT(onGlobalUsers(QString)));
    connect(socket, SIGNAL(onHighConnectCount(QString)),
            SLOT(onHighConnectCount(QString)));
    connect(socket, SIGNAL(onInvite(QString,QString)),
            SLOT(onInvite(QString,QString)));
    connect(socket, SIGNAL(onInviting(QString,QString)),
            SLOT(onInviting(QString,QString)));
    connect(socket, SIGNAL(onISupport(QString)),
            SLOT(onISupport(QString)));
    connect(socket, SIGNAL(onIsOn(QStringList)),
            SLOT(onIsOn(QStringList)));
    connect(socket, SIGNAL(onKick(QString,QString,QString)),
            SLOT(onKick(QString,QString,QString)));
    connect(socket, SIGNAL(onLocalUsers(QString)),
            SLOT(onLocalUsers(QString)));
    connect(socket, SIGNAL(onLUserChannels(int,QString)),
            SLOT(onLUserChannels(int,QString)));
    connect(socket, SIGNAL(onLUserClient(QString)),
            SLOT(onLUserClient(QString)));
    connect(socket, SIGNAL(onLUserMe(QString)),
            SLOT(onLUserMe(QString)));
    connect(socket, SIGNAL(onLUserOp(int,QString)),
            SLOT(onLUserOp(int,QString)));
    connect(socket, SIGNAL(onLUserUnknown(int,QString)),
            SLOT(onLUserUnknown(int,QString)));
    connect(socket, SIGNAL(onMode(QString,QString,QString,QString)),
            SLOT(onMode(QString,QString,QString,QString)));
    connect(socket, SIGNAL(onMode(QString,QString,QString,QStringList)),
            SLOT(onMode(QString,QString,QString,QStringList)));
    connect(socket, SIGNAL(onMotd(QString)),
            SLOT(onMotd(QString)));
    connect(socket, SIGNAL(onMotdStart(QString)),
            SLOT(onMotdStart(QString)));
    connect(socket, SIGNAL(onMyInfo(QString)),
            SLOT(onMyInfo(QString)));
    connect(socket, SIGNAL(onNameReply(QString,QString)),
            SLOT(onNameReply(QString,QString)));
    connect(socket, SIGNAL(onNick(QString,QString)),
            SLOT(onNick(QString,QString)));
    connect(socket, SIGNAL(onNotice(QString,QString)),
            SLOT(onNotice(QString,QString)));
    connect(socket, SIGNAL(onNoticeAuth(QString)),
            SLOT(onNoticeAuth(QString)));
    connect(socket, SIGNAL(onNoticeCtcpVersion(QString,QString)),
            SLOT(onNoticeCtcpVersion(QString,QString)));
    connect(socket, SIGNAL(onNoTopic(QString,QString)),
            SLOT(onNoTopic(QString,QString)));
    connect(socket, SIGNAL(onNowAway(QString)),
            SLOT(onNowAway(QString)));
    connect(socket, SIGNAL(onPong(QString,QString)),
            SLOT(onPong(QString,QString)));
    connect(socket, SIGNAL(onPrivmsg(QString,QString,QString,QString)),
            SLOT(onPrivmsg(QString,QString,QString,QString)));
    connect(socket, SIGNAL(onSelfJoin(QString)),
            SLOT(onSelfJoin(QString)));
    connect(socket, SIGNAL(onSelfUMode(QString)),
            SLOT(onSelfUMode(QString)));
    connect(socket, SIGNAL(onTime(QString,QString)),
            SLOT(onTime(QString,QString)));
    connect(socket, SIGNAL(onTopic(QString,QString)),
            SLOT(onTopic(QString,QString)));
    connect(socket, SIGNAL(onTopicChanged(QString,QString,QDateTime)),
            SLOT(onTopicChanged(QString,QString,QDateTime)));
    connect(socket, SIGNAL(onTopicSetBy(QString,QString,QString)),
            SLOT(onTopicSetBy(QString,QString,QString)));
    connect(socket, SIGNAL(onUMode(QString,QString)),
            SLOT(onUMode(QString,QString)));
    connect(socket, SIGNAL(onUModeIs(QString)),
            SLOT(onUModeIs(QString)));
    connect(socket, SIGNAL(onUnAway(QString)),
            SLOT(onUnAway(QString)));
    connect(socket, SIGNAL(onUserHost(QStringList,QStringList)),
            SLOT(onUserHost(QStringList,QStringList)));
    connect(socket, SIGNAL(onUserJoin(QString,QString)),
            SLOT(onUserJoin(QString,QString)));
    connect(socket, SIGNAL(onUserPart(QString,QString,QString)),
            SLOT(onUserPart(QString,QString,QString)));
    connect(socket, SIGNAL(onUserQuit(QString,QString)),
            SLOT(onUserQuit(QString,QString)));
    connect(socket, SIGNAL(onVersion(QString)),
            SLOT(onVersion(QString)));
    connect(socket, SIGNAL(onWelcome(QString)),
            SLOT(onWelcome(QString)));
    connect(socket, SIGNAL(onWho(QString,QString,QString,QString,QString,QString,int,QString)),
            SLOT(onWho(QString,QString,QString,QString,QString,QString,int,QString)));
    connect(socket, SIGNAL(onWhoIsChannels(QString,QString)),
            SLOT(onWhoIsChannels(QString,QString)));
    connect(socket, SIGNAL(onWhoIsIdentified(QString,QString)),
            SLOT(onWhoIsIdentified(QString,QString)));
    connect(socket, SIGNAL(onWhoIsIdle(QString,QString,QString,QString)),
            SLOT(onWhoIsIdle(QString,QString,QString,QString)));
    connect(socket, SIGNAL(onWhoIsOperator(QString,QString)),
            SLOT(onWhoIsOperator(QString,QString)));
    connect(socket, SIGNAL(onWhoIsServer(QString,QString,QString)),
            SLOT(onWhoIsServer(QString,QString,QString)));
    connect(socket, SIGNAL(onWhoIsUser(QString,QString,QString,QString)),
            SLOT(onWhoIsUser(QString,QString,QString,QString)));
    connect(socket, SIGNAL(onWhoWasUser(QString,QString,QString,QString)),
            SLOT(onWhoWasUser(QString,QString,QString,QString)));
    connect(socket, SIGNAL(onWriteLine(bool,QString)),
            SLOT(onWriteLine(bool,QString)));
    connect(socket, SIGNAL(onYoureOper(QString)),
            SLOT(onYoureOper(QString)));
    connect(socket, SIGNAL(onYourHost(QString)),
            SLOT(onYourHost(QString)));
    connect(socket, SIGNAL(on901(QString,QString,QString,QString)),
            SLOT(on901(QString,QString,QString,QString)));
    connect(d->parser, SIGNAL(whoRequest(QString)),
            SLOT(whoRequested(QString)));
    connect(d->parser, SIGNAL(queryMessage(QString,QString)),
            SLOT(queryMessage(QString,QString)));
}

ServerWindow::~ServerWindow()
{
}

Aki::IdentityConfig*
ServerWindow::identity()
{
    return d->identityConfig;
}

void
ServerWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *data = event->mimeData();
    const QStringList formats = data->formats();

    if (formats.contains("application/aki-tab")) {
        if (d->splitView->isHidden() || d->mainView->isHidden()) {
            event->acceptProposedAction();
        } else {
            event->ignore();
        }
    } else {
        event->ignore();
    }
}

void
ServerWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *data = event->mimeData();
    const QStringList formats = data->formats();

    if (formats.contains("application/aki-tab")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();

        QByteArray itemData = event->mimeData()->data("application/aki-tab");
        QDataStream stream(&itemData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_5);

        quint64 data;
        stream >> data;

        Aki::BaseWindow *window = reinterpret_cast<Aki::BaseWindow*>(data);

        if (d->splitView->isHidden()) {
            emit dropSuccessful(window);
            d->splitView->addChannel(window);
            d->splitView->show();
        } else if (d->mainView->isHidden()) {
            emit dropSuccessful(window);
            d->mainView->addChannel(window);
            d->mainView->show();
        }
    } else {
        event->ignore();
    }
}

Aki::BaseWindow*
ServerWindow::currentFocusedChannel()
{
    return d->currentFocusedChannel();
}

Aki::BaseWindow*
ServerWindow::findChannel(const QString &name)
{
    return d->findChannel(name);
}

void
ServerWindow::createNewView(Aki::BaseWindow *window)
{
    if (d->splitView->isHidden()) {
        window->setCurrent(true);
        d->splitView->addChannel(window);
        d->splitView->show();
    } else if(d->mainView->isHidden()) {
        window->setCurrent(true);
        d->mainView->addChannel(window);
        d->mainView->show();
    }
}

bool
ServerWindow::hasInputFocus() const
{
    return false;
}

QStringList
ServerWindow::rejoinChannelList() const
{
    return d->rejoinChannels;
}

Aki::ChannelView*
ServerWindow::mainView()
{
    return d->mainView;
}

Aki::ChannelView*
ServerWindow::splitView()
{
    return d->splitView;
}

#include "serverwindow.moc"
