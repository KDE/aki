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

#ifndef AKI_IRC_REPLYINFO_HPP
#define AKI_IRC_REPLYINFO_HPP

#include "aki.hpp"
#include "irc/nickinfo.hpp"
#include "irc/replycodes.hpp"
#include <QtCore/QStringList>

namespace Aki
{
namespace Irc
{
class ReplyInfoPrivate;
/**
 * ReplyInfo is a class that wraps around a message that is received from the IRC server.
 * It makes it easier to get specific information from the message.
 *
 * This class is implicitly shared. It's meant to be used on the stack.
 */
class LIBAKI_EXPORT ReplyInfo
{
public:
    /**
     * Empty ReplyInfo.
     */
    ReplyInfo();
    /**
     * Constructs a ReplyInfo with the @p sender, @p command, @p message, @p params and a @p replyCode.
     * The parameters @p params and @p replyCode are optional. If the message has a @p replyCode then
     * the @p command will be empty and vice-versa.
     *
     * @param sender Sender of the message. This can be the server or a user.
     * @param command The message command (empty if there is a replyCode set).
     * @param message Raw message
     * @param params Split form of @p message.
     * @param replyCode IRC numeric reply (default Null numeric).
     */
    explicit ReplyInfo(const Aki::Irc::NickInfo& sender, const QString& command, const QString& message,
                       const QStringList& params = QStringList(), Aki::Irc::ReplyCodes replyCode = Aki::Irc::RPL_NULL);
    /**
     * Copies @p other to this.
     *
     * @param other ReplyInfo to copy.
     */
    ReplyInfo(const Aki::Irc::ReplyInfo& other);
    /**
     * Destroys the object.
     */
    ~ReplyInfo();
    /**
     * Assigns @p other to this object.
     *
     * @param other ReplyInfo to assign.
     */
    Aki::Irc::ReplyInfo& operator=(const Aki::Irc::ReplyInfo& other);
    /**
     * Gets the IRC command. This is empty if isNumeric is true.
     *
     * @return IRC command.
     */
    QString command() const;
    /**
     * Gets the state of if the message is numeric.
     *
     * @return true if message is a numeric reply; other it is a command.
     */
    bool isNumeric() const;
    /**
     * Gets the raw representation of the message.
     *
     * @return Raw IRC message.
     */
    QString message() const;
    /**
     * Gets the numeric value of the reply.
     *
     * @return Numeric reply; Aki::Irc::RPL_NULL if this message is a command or invalid.
     */
    Aki::Irc::ReplyCodes numeric() const;
    /**
     * Gets a array representation of the message broken down.
     *
     * @return Array representation of the message.
     */
    QStringList params() const;
    /**
     * Gets the sender of the message. This can be the server or user who sent it.
     *
     * @return Sender of the message.
     */
    Aki::Irc::NickInfo sender() const;
private:
    QSharedDataPointer<Aki::Irc::ReplyInfoPrivate> _d;
}; // End of class ReplyInfo.
} // End of namespace Irc.
} // End of namespace Aki.

QDebug LIBAKI_EXPORT operator<<(QDebug dbg, const Aki::Irc::ReplyInfo& replyInfo);

#endif // AKI_IRC_REPLYINFO_HPP
