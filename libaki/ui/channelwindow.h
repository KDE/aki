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

#ifndef AKI_CHANNELWINDOW_H
#define AKI_CHANNELWINDOW_H

#include "libaki_export.h"
#include "ui/basewindow.h"
#include "ui_channelwindow.h"
#include <QScopedPointer>

namespace Aki
{
namespace Irc
{
class Socket;
class User;
} // End of namespace Irc.
class IdentityConfig;
class ChatParser;
class ChannelWindowPrivate;
/**
 * Displays and represents a single IRC Channel for a specific server.
 */
class LIBAKI_EXPORT ChannelWindow : public Aki::BaseWindow,
                                    public Ui::ChannelWindow
{
    Q_OBJECT
public:
    /**
     * Creates a new channel.
     * @param name Name of the channel.
     * @param identityConfig Identity configuration of the identity which you use to join the channel with.
     * @param socket Socket that is connected to this channel.
     * @param parent Parent of the object.
     */
    ChannelWindow(const QString &name, Aki::IdentityConfig *identityConfig, Aki::Irc::Socket *socket, QWidget *parent = 0);
    /**
     * Destroys the object.
     */
    ~ChannelWindow();
    /**
     * Gets the identity configuration for the identity of which this
     * channel is currently connected for.
     */
    Aki::IdentityConfig* identity();
    /**
     * Sets the nick list for the nick selector.
     * @param nickList Nickname list.
     */
    void setNickList(const QStringList &nickList);
    /**
     * Gets the mode bar for the channel.
     * @return Channel mode bar.
     */
    Aki::ChannelModeBar* modeBar();
    /**
     * Adds a new user @p user the channel.
     * @param user IRC user.
     */
    void addUser(Aki::Irc::User *user);
    /**
     * Removes a user @p user from the channel.
     * @param user IRC user.
     */
    void removeUser(Aki::Irc::User *user);
    /**
     * Sets the channel's topic @p topic.
     * @param topic Channel topic.
     */
    void setTopic(const QString &topic);
    /**
     * Gets the list of IRC users in the channel.
     * @return IRC user list.
     */
    QList<Aki::Irc::User*> users() const;
    /**
     * Gets the list of IRC users in the channel.
     * @return IRC user list.
     */
    QList<Aki::Irc::User*> users();
    /**
     * Adds a new mode @p mode to the user @p name.
     * @param name User's nickname.
     * @param mode Mode to be added.
     */
    void addMode(const QString &name, const QString &mode);
    /**
     * Removes a mode @p mode from the user @p name.
     * @param name User's nickname.
     * @param mode Mode to remove removed.
     */
    void removeMode(const QString &name, const QString &mode);
    /**
     * Adds a message @p message from a user @p from.
     * @param from User who sent the message.
     * @param message User's message to the channel.
     */
    void addMessage(const QString &from, const QString &message);
    /**
     * @copydoc BaseWindow::setCurrent()
     */
    virtual void setCurrent(bool current);

    void setIsWhoRunning(bool running);

    void addWho(const QString &channel, const QString &identName, const QString &address,
                const QString &server, const QString &nick, const QString &flags,
                int hops, const QString &realName);
    void resetWho();

    void addNick(const QString &oldNick, const QString &newNick);

    virtual bool hasInputFocus() const;

    void setNewNick(const QString &nick);
Q_SIGNALS:
    void textSubmitted(Aki::BaseWindow *window, const QString &text);
protected:
    virtual void showEvent(QShowEvent *event);
private:
    Q_PRIVATE_SLOT(d, void nickSelectorActivated(const QString &nick));
    Q_PRIVATE_SLOT(d, void banDomainTriggered())
    Q_PRIVATE_SLOT(d, void banHostTriggered())
    Q_PRIVATE_SLOT(d, void banUserDomainTriggered())
    Q_PRIVATE_SLOT(d, void banUserHostTriggered())
    Q_PRIVATE_SLOT(d, void customContextMenuRequested(const QPoint &pos))
    Q_PRIVATE_SLOT(d, void giveHalfOpTriggered())
    Q_PRIVATE_SLOT(d, void giveOpTriggered())
    Q_PRIVATE_SLOT(d, void giveVoiceTriggered())
    Q_PRIVATE_SLOT(d, void kickBanDomainTriggered())
    Q_PRIVATE_SLOT(d, void kickBanHostTriggered())
    Q_PRIVATE_SLOT(d, void kickBanUserDomainTriggered())
    Q_PRIVATE_SLOT(d, void kickBanUserHostTriggered())
    Q_PRIVATE_SLOT(d, void kickUserTriggered())
    Q_PRIVATE_SLOT(d, void showCompletion())
    Q_PRIVATE_SLOT(d, void takeHalfOpTriggered())
    Q_PRIVATE_SLOT(d, void takeOpTriggered())
    Q_PRIVATE_SLOT(d, void takeVoiceTriggered())
    Q_PRIVATE_SLOT(d, void textSubmitted())
    Q_PRIVATE_SLOT(d, void channelKeyStateChanged(bool state))
    Q_PRIVATE_SLOT(d, void newChannelKey(const QString &key))
    Q_PRIVATE_SLOT(d, void channelLimitStateChanged(bool state))
    Q_PRIVATE_SLOT(d, void newChannelLimit(int limit))
    Q_PRIVATE_SLOT(d, void topicProtectionStateChanged(bool state))
    Q_PRIVATE_SLOT(d, void noOutsideMessagesStateChanged(bool state))
    Q_PRIVATE_SLOT(d, void secretStateChanged(bool state))
    Q_PRIVATE_SLOT(d, void inviteOnlyStateChanged(bool state))
    Q_PRIVATE_SLOT(d, void privateStateChanged(bool state))
    Q_PRIVATE_SLOT(d, void moderatedStateChanged(bool state))
    Q_PRIVATE_SLOT(d, void whoTimerTimeout())
    Q_PRIVATE_SLOT(d, void chatTopicReturnPressed(const QString &topic))
    Q_PRIVATE_SLOT(d, void splitterMoved(int pos, int index))
    Q_PRIVATE_SLOT(d, void openQueryTriggered());
private:
    friend class ChannelWindowPrivate;
    QScopedPointer<ChannelWindowPrivate> d;
}; // End of class ChannelWindow.
} // End of namespace Aki::BaseWindow.

#endif // AKI_CHANNELWINDOW_H
