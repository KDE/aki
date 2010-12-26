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

#ifndef AKI_IRC_REPLY_HPP
#define AKI_IRC_REPLY_HPP

#include "aki.hpp"
#include "irc/replyinfo.hpp"

namespace Aki
{
namespace Irc
{
class ReplyPrivate;
/**
 * Base class for all replies.
 * Gives access to ReplyInfo so users can have access to the details of the message.
 */
class LIBAKI_EXPORT Reply
{
public:
    /**
     * Constructs an empty Reply.
     */
    Reply();
    /**
     * Sets the ReplyInfo for the message.
     *
     * @param replyInfo IRC message information.
     */
    explicit Reply(const Aki::Irc::ReplyInfo& replyInfo);
    /**
     * Copies @p other Reply to this.
     *
     * @param other IRC Reply to copy.
     */
    Reply(const Aki::Irc::Reply& other);
    /**
     * Destroys the object.
     */
    virtual ~Reply();
    /**
     * Assigns the @p other Reply to this.
     *
     * @param other IRC Reply to copy.
     */
    Aki::Irc::Reply& operator=(const Aki::Irc::Reply& other);
    /**
     * Gets the detail information of the reply.
     *
     * @return Detail information about the reply.
     */
    Aki::Irc::ReplyInfo reply() const;
private:
    QSharedDataPointer<Aki::Irc::ReplyPrivate> _d;
}; // End of class Reply.
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_REPLY_HPP
