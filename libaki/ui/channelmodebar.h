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

#ifndef CHANNELMODEBAR_H
#define CHANNELMODEBAR_H

#include "libaki_export.h"
#include "ui_channelmodebar.h"
#include <QWidget>

namespace Aki
{
class ChannelModeBarPrivate;
/**
 * Channel mode bar for easy access to changing the channels modes.
 */
class LIBAKI_EXPORT ChannelModeBar : public QWidget,
                                     private Ui::ChannelModeBar
{
    Q_OBJECT
public:
    /**
     * Creates a new ChannelModeBar.
     * @param parent Parent of the object.
     */
    ChannelModeBar(QWidget *parent = 0);
    /**
     * Destroys the object.
     */
    ~ChannelModeBar();
    /**
     * Sets the channel key @p key.
     * @param key Channel key (password).
     */
    void setKey(const QString &key);
    /**
     * Gets the channel key.
     * @return Channel key or an empty QString().
     */
    QString key() const;
    /**
     * Sets the channel limit @p limit.
     * @param limit Channel limit of how many people can join.
     */
    void setLimit(int limit);
    /**
     * Gets the channel limit.
     * @return Channel limit.
     */
    int limit() const;

    void enableChannelKeyButton(bool enable);
    void disableChannelKeyButton(bool disable);
    void enableChannelLimitButton(bool enable);
    void disableChannelLimitButton(bool disable);
    void enableTopicProtectionButton(bool enable);
    void disableTopicProtectionButton(bool disable);
    void enableNoOutsideMessagesButton(bool enable);
    void disableNoOutsideMessagesButton(bool disable);
    void enableSecretButton(bool enable);
    void disableSecretButton(bool disable);
    void enableInviteOnlyButton(bool enable);
    void disableInviteOnlyButton(bool disable);
    void enablePrivateButton(bool enable);
    void disablePrivateButton(bool disable);
    void enableModeratedButton(bool enable);
    void disableModeratedButton(bool disable);
Q_SIGNALS:
    /**
     * Emitted when the channel key button has been pushed,
     * @param state true if enabled; false otherwise.
     */
    void channelKeyStateChanged(bool state);
    /**
     * Emitted when a new channel key has been set.
     * @param key New channel key.
     */
    void newChannelKey(const QString &key);
    /**
     * Emitted when the channel limit button has been pushed.
     * @param state true if enabled; false otherwise.
     */
    void channelLimitStateChanged(bool state);
    /**
     * Emitted when the channel limit has been set.
     * @param limit Channel limit.
     */
    void newChannelLimit(int limit);
    /**
     * Emitted when the 'T' button has been pushed.
     * @param state true if enabled; false otherwise.
     */
    void topicProtectionStateChanged(bool state);
    /**
     * Emitted when the 'N' button has been pushed.
     * @param state true if enabled; false otherwise.
     */
    void noOutsideMessagesStateChanged(bool state);
    /**
     * Emitted when the 'S' button has been pushed.
     * @param state true if enabled; false otherwise.
     */
    void secretStateChanged(bool state);
    /**
     * Emitted when the 'I' button has been pushed.
     * @param state true if enabled; false otherwise.
     */
    void inviteOnlyStateChanged(bool state);
    /**
     * Emitted when the 'P' button has been pushed.
     * @param state true if enabled; false otherwise.
     */
    void privateStateChanged(bool state);
    /**
     * Emitted when the 'M' button has been pushed.
     * @param state true if enabled; false otherwise.
     */
    void moderatedStateChanged(bool state);
private:
    Q_PRIVATE_SLOT(d, void channelKeyToggled(bool checked))
    Q_PRIVATE_SLOT(d, void channelLimitToggled(bool checked))
    Q_PRIVATE_SLOT(d, void topicProtectionChannelToggled(bool checked))
    Q_PRIVATE_SLOT(d, void noOutsideMessagesChannelToggled(bool checked))
    Q_PRIVATE_SLOT(d, void secretChannelToggled(bool checked))
    Q_PRIVATE_SLOT(d, void inviteOnlyChannelToggled(bool checked))
    Q_PRIVATE_SLOT(d, void privateChannelToggled(bool checked))
    Q_PRIVATE_SLOT(d, void moderatedChannelToggled(bool checked))
    Q_PRIVATE_SLOT(d, void channelKeyReturnPressed(const QString &key))
    Q_PRIVATE_SLOT(d, void channelLimitReturnPressed(const QString &limit))
private:
    friend class ChannelModeBarPrivate;
    ChannelModeBarPrivate* const d;
}; // End of class ChannelModeBar.
} // End of namespace Aki.

#endif // CHANNELMODEBAR_H
