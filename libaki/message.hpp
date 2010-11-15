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

#ifndef AKI_MESSAGE_HPP
#define AKI_MESSAGE_HPP

#include "aki.hpp"
#include "irc/message.hpp"
#include <KDE/KDateTime>

namespace Aki
{
class MessagePrivate;
/**
 * Encapsulates each message that is passed through Aki. It represents
 * a message that is received from the IRC server or is passed to the IRC
 * server from the client.
 *
 * You are required to create a Message in order to send them.
 *
 * \code
 * Aki::Message message = Aki::RfcCommands::nick("FooNick");
 * message.createTimestamp();
 * message.setDirection(Aki::Message::Outgoing);
 * \endcode
 */
class LIBAKI_EXPORT Message
    : public Aki::Irc::Message
{
public:
    /**
     * Message Direction.
     */
    enum Direction {
        /**
         * Message is leaving.
         */
        Outgoing,
        /**
         * Message is incoming.
         */
        Incoming
    }; // End of enum Direction.

    /**
     * Create a Message object which parses the messsage.
     *
     * @param line Message that will be parsed.
     */
    explicit Message(const QString& line);
    /**
     * Copy constructor.
     *
     * @param message Message to copy.
     */
    Message(const Aki::Message& message);

    Message(const Aki::Irc::Message& message);
    /**
     * Destroys the message.
     */
    ~Message();
    /**
     * Copies the Message object.
     *
     * @param rhs Message to copy.
     * @return New message object.
     */
    Aki::Message& operator=(const Aki::Message& rhs);
    /**
     * Creates a timestamp for the message. The timestamp
     * is the current time for when this is called.
     */
    void createTimestamp();
    /**
     * Direction of this message.
     *
     * @return Message direction.
     */
    Aki::Message::Direction direction() const;
    /**
     * Determines if this message is right to left.
     *
     * @return true if this message is rtl; false otherwise.
     */
    bool isRightToLeft() const;
    /**
     * Sets the Message direction.
     *
     * @param direction Direction of this message.
     */
    void setDirection(Aki::Message::Direction direction);
    /**
     * Sets the timestamp for this message. This allows for
     * a custom timestamp.
     *
     * @param timeStamp Message timestamp.
     */
    void setTimeStamp(const KDateTime& timeStamp);
    /**
     * Gets the timestamp for this message for when this
     * message was received or created.
     *
     * @return Message timestamp.
     */
    KDateTime timeStamp() const;
private:
    AKI_DECLARE_PRIVATE(Message)
}; // End of class Message.
} // End of namespace Aki.

#endif // AKI_MESSAGE_HPP
