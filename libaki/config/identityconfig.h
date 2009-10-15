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

#ifndef IDENTITYCONFIG_H
#define IDENTITYCONFIG_H

#include "libaki_export.h"
#include "config/configfile.h"

namespace Aki
{
/**
 * Configuration class for the IRC Identity. Able to read and write to the
 * akiidentityrc file that stores the identity settings.
 */
class LIBAKI_EXPORT IdentityConfig : public Aki::ConfigFile
{
    Q_OBJECT
public:
    /**
     * Creates an identity object.
     */
    IdentityConfig(QObject *parent = 0);
    /**
     * Destroys the object.
     */
    ~IdentityConfig();
    /**
     * Gets the away message of the identity.
     * @return Away message.
     */
    QString awayMessage();
    /**
     * Gets the away nickname of the identity.
     * @return Away nickname.
     */
    QString awayNickname();
    /**
     * Gets the kick message of the identity.
     * @return Kick message.
     */
    QString kickMessage();
    /**
     * Gets the state of if the away messages should be used.
     * @return true if enabled; false otherwise.
     */
    bool isAwayMessagesEnabled();
    /**
     * Gets the state of if the mark last position should be used.
     * @return true if enabled; false otherwise.
     */
    bool isMarkLastPositionEnabled();
    /**
     * Gets the list of nicknames available for this identity.
     * @return Nickname list.
     */
    QStringList nicknameList();
    /**
     * Gets the part message of the identity.
     * @return Part message.
     */
    QString partMessage();
    /**
     * Gets the quit message of the identity.
     * @return Quit message.
     */
    QString quitMessage();
    /**
     * Gets the real name of the identity.
     * @return Real name.
     */
    QString realName();
    /**
     * Gets the return message of the identity.
     * @return Return message.
     */
    QString returnMessage();
    /**
     * Sets the away message @p message of the identity.
     * @param message Away message.
     */
    void setAwayMessage(const QString &message);
    /**
     * Sets the away nickname @p nick of the identity.
     * @param nick Nickname.
     */
    void setAwayNickname(const QString &nick);
    /**
     * Sets the kick message @p message of the identity.
     * @param message Kick message.
     */
    void setKickMessage(const QString &message);
    /**
     * Sets the state @p enable of if the identity will mark the last position
     * where the user changed tab and received a message from the channel.
     * @param enable true if marking last position; false otherwise.
     */
    void setMarkLastPosition(bool enable);
    /**
     * Sets the list of nicknames @p nicknames that will be used in
     * the identity.
     * @param nicknames Nickname list.
     */
    void setNicknameList(const QStringList &nicknames);
    /**
     * Sets the part message @p message of the identity.
     * @param message Part message.
     */
    void setPartMessage(const QString &message);
    /**
     * Sets the quit message @p message of the identity.
     * @param message Quit message.
     */
    void setQuitMessage(const QString &message);
    /**
     * Sets the real name @p name of the identity.
     * @param name Real name.
     */
    void setRealName(const QString &name);
    /**
     * Sets the return message @p message of the identity.
     * @param message Return message.
     */
    void setReturnMessage(const QString &message);
    /**
     * Sets the state @p enable of if the identity will use
     * the away messages or not.
     * @param enable True if the away messages are going to be used;
     * false otherwise.
     */
    void setUseAwayMessages(bool enable);
    /**
     *
     */
    QString name() const;
    /**
     *
     */
    void setName(const QString &name);
private:
    QString m_name;
}; // End of class IdentityConfig.
} // End of namespace Aki.

#endif // IDENTITYCONFIG_H
