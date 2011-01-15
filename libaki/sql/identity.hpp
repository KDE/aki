/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
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

#ifndef AKI_SQL_IDENTITY_HPP
#define AKI_SQL_IDENTITY_HPP

#include "aki.hpp"
#include "sql/database.hpp"
#include "sql/table.hpp"

namespace Aki
{
namespace Sql
{
class IdentityPrivate;
class LIBAKI_EXPORT Identity
    : public Aki::Sql::Table
{
    Q_OBJECT
    Q_CLASSINFO("id","PRIMARY_KEY,AUTO_INCREMENT")
    Q_CLASSINFO("awayMessage","LENGTH=250,NULL=true")
    Q_CLASSINFO("awayNickname","LENGTH=30,NULL=true")
    Q_CLASSINFO("name","LENGTH=30,NULL=false,UNIQUE")
    Q_CLASSINFO("kickMessage","LENGTH=250,NULL=true")
    Q_CLASSINFO("enableMessages","NULL=false")
    Q_CLASSINFO("markLastPosition","NULL=false")
    Q_CLASSINFO("partMessage","LENGTH=250,NULL=true")
    Q_CLASSINFO("quitMessage","LENGTH=250,NULL=true")
    Q_CLASSINFO("realName","LENGTH=40,NULL=false")
    Q_CLASSINFO("returnMessage","LENGTH=250,NULL=true")
    /**
     * This property holds the @p id of the row when it was commited.<br>
     * The @p id is unique. If the @p id is -1, it means it hasn't been committed yet.
     *
     * The default value is -1.
     */
    Q_PROPERTY(int id READ id WRITE setId)
    /**
     * This property holds the @p awayMessage for when the user marks their self away.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString awayMessage READ awayMessage WRITE setAwayMessage)
    /**
     * This property holds the @p awayNickname for when the user marks their self away.
     *
     * The default value is their desktop name with |away.
     */
    Q_PROPERTY(QString awayNickname READ awayNickname WRITE setAwayNickname)
    /**
     * This property holds the @p name of the identity.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString name READ name WRITE setName)
    /**
     * This property holds the @p kickMessage for when the user kicks someone from the channel.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString kickMessage READ kickMessage WRITE setKickMessage)
    /**
     * This property holds the state of whether to enable the away/kick/part messages.
     *
     * The default value is true.
     */
    Q_PROPERTY(bool enableMessages READ isMessagesEnabled WRITE setMessagesEnabled)
    /**
     * This property holds the state of whether to whether remember where the user left
     * off in the conversation of a channel due to minimised window, away, loses focus of the window,
     * or is in another channel. This will have no affect on the current channel unless the user has lost focus.
     *
     * The default value is false.
     */
    Q_PROPERTY(bool markLastPosition READ isMarkLastPositionEnabled WRITE setMarkLastPositionEnabled)
    /**
     * This property holds the @p partMessage for when the user parts aka quits the channel but not the server.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString partMessage READ partMessage WRITE setPartMessage)
    /**
     * This property holds the @p quitMessage for when the user quits the server.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString quitMessage READ quitMessage WRITE setQuitMessage)
    /**
     * This property holds the user's @p realName.
     *
     * The default value is the user's login real name where it is supported. If the user's login name
     * on the system does not support this, then it will return an empty QString.
     */
    Q_PROPERTY(QString realName READ realName WRITE setRealName)
    /**
     * This property holds the user's @p returnMessage for when the user returns from an IDLE state.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString returnMessage READ returnMessage WRITE setReturnMessage)
public:
    Q_INVOKABLE explicit Identity(QObject* parent = 0);
    ~Identity();
    /**
     * Gets the user's away message.
     *
     * @return User's away message.
     */
    QString awayMessage() const;
    /**
     * Gets the user's away nickname.
     *
     * @return User's away nickname.
     */
    QString awayNickname() const;
    /**
     * Gets the id of this data.
     *
     * @return -1 if the data isn't committed yet; otherwise the row of this
     * data when it was committed.
     */
    int id() const;
    /**
     * Gets the state of whether to remember where the user left off from in the channel.
     *
     * @return true if to remember last position in the chat; false otherwise.
     */
    bool isMarkLastPositionEnabled() const;
    /**
     * Gets the state of whether to user the users messages or not.
     *
     * @return true if to use the user's away messages; false otherwise.
     */
    bool isMessagesEnabled() const;
    /**
     * Gets the user's kick message.
     *
     * @return User's kick message.
     */
    QString kickMessage() const;
    /**
     * Gets the name of the identity.
     *
     * @return Identity name.
     */
    QString name() const;
    /**
     * Gets the user's part message.
     *
     * @return User's part message.
     */
    QString partMessage() const;
    /**
     * Gets the user's quit message.
     *
     * @return User's quit message.
     */
    QString quitMessage() const;
    /**
     * Gets the user's real name.
     *
     * @return User's real name.
     */
    QString realName() const;
    /**
     * Gets the return message of when the user returns back from IDLE state.
     *
     * @return Return message.
     */
    QString returnMessage() const;
    /**
     * Sets the away @p message for when the user marks their self as away.
     *
     * @param message Away message.
     */
    void setAwayMessage(const QString& message);
    /**
     * Sets the away @p nickname for when the user marks their self as away.
     *
     * @param nickname Away nickname.
     */
    void setAwayNickname(const QString& nickname);
    void setId(int id);
    /**
     * Sets the @p name of the identity.
     *
     * @param name Identity name.
     */
    void setName(const QString& name);
    /**
     * Sets the kick @p message for when the user kicks someone.
     *
     * @param message Kick message.
     */
    void setKickMessage(const QString& message);
    /**
     * Sets the state of whether to use custom messages like part/quit/kick.
     *
     * @param enable true to enable these messages; false otherwise.
     */
    void setMessagesEnabled(bool enable);
    /**
     * Sets the state of the whether to remember where the user last left off of
     * when the user minimizes the client or is in another channel. Will not happen
     * when you are in the current channel.
     *
     * @param enable true to remember last position; false otherwise.
     */
    void setMarkLastPositionEnabled(bool enable);
    /**
     * Sets the part @p message when the user leaves a channel on the server.
     *
     * @param message Part message.
     */
    void setPartMessage(const QString& message);
    /**
     * Sets the quit @p message when the user quits the server.
     *
     * @param message Users quit message.
     */
    void setQuitMessage(const QString& message);
    /**
     * Sets the real @p name of the user.
     *
     * @param name Users real name.
     */
    void setRealName(const QString& name);
    /**
     * Sets the return @p message for when the user comes back from an IDLE state.
     *
     * @param message Return message.
     */
    void setReturnMessage(const QString& message);
private:
    AKI_DECLARE_PRIVATE(Identity)
}; // End of class Identity.
} // End of namespace Sql.
} // End of namespace Aki.

#endif // AKI_SQL_IDENTITY_HPP
