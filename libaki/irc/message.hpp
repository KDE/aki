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

#ifndef AKI_IRC_MESSAGE_HPP
#define AKI_IRC_MESSAGE_HPP

#include "aki.hpp"
#include "nickinfo.hpp"
#include "replycodes.hpp"
#include <KDE/KDateTime>
#include <QtCore/QObject>
#include <QtCore/QStringList>

class QTextCodec;
namespace Aki
{
namespace Irc
{
class MessagePrivate;
/**
 * Represents a message that is sent from the IRC Client to the IRC Server and
 * also received from the IRC Server to the IRC client.
 *
 * Creates a message to send.
 * @code
 * Aki::Irc::Message message;
 * message.setTimestamp(KDateTime::currentDateTime());
 * message.setDirection(Aki::Irc::Message::Outgoing).
 * message.setPlainText("Hello");
 * @endcode
 *
 * Creates a message that you're receiving.
 * @code
 * Aki::Irc::Message message;
 * message.setDirection(Aki::Irc::Message::Incoming);
 * message.setTimeStamp(KDateTime::currentDateTime());
 * message.setPlainText(ircMessage);
 * message.setSender(sender);
 * message.setTarget(target);
 * @endcode
 *
 * Creates a message that is being sent other than to you.
 * @code
 * Aki::Irc::Message message;
 * message.setTimestamp(KDateTime::currentDateTime());
 * message.setDirection(Aki::Irc::Message::Incoming);
 * message.setPlainText(ircMessage);
 * message.setSender(sender);
 * 
 * @endcode
 */
class LIBAKI_EXPORT Message
{
    Q_ENUMS(Direction)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection)
    Q_PROPERTY(bool isRightToLeft READ isRightToLeft)
    Q_PROPERTY(QString plainText READ plainText WRITE setPlainText READ clear)
    Q_PROPERTY(QString html READ html WRITE setHtml RESET clear)
    Q_PROPERTY(KDateTime timeStamp READ timeStamp WRITE setTimeStamp)
    Q_PROPERTY(State state READ read WRITE setState)
    Q_PROPERTY(Aki::Irc::NickInfo sender READ sender WRITE setSender)
    Q_PROPERTY(Aki::Irc::NickInfo target READ target WRITE setTarget)
public:
    /**
     * Direction of the message.
     */
    enum Direction {
        /**
         * Message is going to the server.
         */
        Incoming,
        /**
         * Message is coming to the client.
         */
        Outgoing
    }; // End of enum Direction.
    /**
     * Sets the state of the message.
     */
    enum State {
        /**
         * Message is in a unknown state.
         */
        Unknown,
        /**
         * Message is in a sending state.
         */
        Sending,
        /**
         * Message was successfully sent
         */
        Sent,
        /**
         * Message was an error.
         */
        Error
    }; // End of enum State.
    enum Type {
        /**
         * Message was a CTCP action /me
         */
        Action,
        /**
         * Message was a CTCP.
         */
        Ctcp,
        /**
         * Message was a DCC Chat message.
         */
        DccChat,
        /**
         * Message was a DCC File.
         */
        DccFile,
        /**
         * Message was a highlight.
         */
        Highlight,
        /**
         * Normal message.
         */
        Normal
    };
    /**
     * Creates an empty message.
     */
    Message();
    /**
     * Creates a message from another message.
     *
     * @param other Message to copy.
     */
    Message(const Aki::Irc::Message& other);
    /**
     * Destroys the message.
     */
    ~Message();
    Aki::Irc::Message& operator=(const Aki::Irc::Message& rhs);
    /**
     * Clears the message.
     */
    void clear();
    /**
     * Gets the direction of the message.
     *
     * @return Message direction.
     * @sa Aki::Irc::Message::setDirection()
     */
    Aki::Irc::Message::Direction direction() const;
    /**
     * Gets the message in HTML form.
     *
     * @return HTML style message.
     * @sa Aki::Irc::Message::setHtml()
     */
    QString html() const;
    /**
     * Determines is the message is right to left direction.
     *
     * @return true if the message is RTL; false if LTR.
     */
    bool isRightToLeft() const;
    /**
     * Gets the message in plain text without HTML.
     *
     * @return Plain text message.
     * @sa Aki::Irc::Message::setPlainText()
     */
    QString plainText() const;
    /**
     * Gets the sender of the message.
     *
     * @return Person who sent the message.
     * @sa Aki::Irc::Message::setSender()
     */
    const Aki::Irc::NickInfo& sender() const;
    /**
     * Sets the message @p direction.
     *
     * @param direction Message direction.
     * @sa Aki::Irc::Message::direction()
     */
    void setDirection(Aki::Irc::Message::Direction direction);
    /**
     * Sets the message with HTML text.
     * @note If this message already has text. Calling thie method will replace the text
     * with the @p message.
     *
     * @param message HTML based message.
     * @sa Aki::Irc::Message::html()
     */
    void setHtml(const QString& message);
    /**
     * Sets the @p message with plain text.
     * @note If this message already has text. Calling this method will replace the text
     * with the @p message.
     *
     * @param message Plain text message.
     * @sa Aki::Irc::Message::plainText()
     */
    void setPlainText(const QString& message);
    /**
     * Sets the @p sender of the message.
     *
     * @param sender Person who is sending the message.
     * @sa Aki::Irc::Message::sender()
     */
    void setSender(const Aki::Irc::NickInfo& sender);
    /**
     * Sets the message @p state.
     *
     * @param state Message state.
     * @sa Aki::Irc::Message::state()
     */
    void setState(Aki::Irc::Message::State state);
    /**
     * Sets the @p target of the message. This is only required when you are
     * sending it via private message.
     *
     * @param target Person receiving the message.
     * @sa Aki::Irc::Message::target()
     */
    void setTarget(const Aki::Irc::NickInfo& target);
    /**
     * Sets the @p timeStamp that the message was sent or received. The @p timeStamp
     * will be converted to the system's locale.
     *
     * @param timeStamp When the message was created or received.
     * @sa Aki::Irc::Message::timeStamp()
     */
    void setTimeStamp(const KDateTime& timeStamp);
    /**
     * Sets the @p type of message this is.
     *
     * @param type Message Type.
     * @sa Aki::Irc::Message::type()
     */
    void setType(Aki::Irc::Message::Type type);
    /**
     * Gets the message state.
     *
     * @return Message state.
     * @sa Aki::Irc::Message::setState()
     */
    Aki::Irc::Message::State state() const;
    /**
     * Gets the target of this message.
     *
     * @return Target that is receiving this message.
     * @sa Aki::Irc::Message::setTarget()
     */
    const Aki::Irc::NickInfo& target() const;
    /**
     * Gets the timestamp of when this message was created.
     *
     * @return Timestamp when this message was created.
     * @sa Aki::Irc::Message::setTimeStamp()
     */
    const KDateTime& timeStamp() const;
    /**
     * Gets the specific type of message this is.
     *
     * @return Message Type.
     * @sa Aki::Irc::Message::setType()
     */
    Aki::Irc::Message::Type type() const;
private:
    AKI_DECLARE_PRIVATE(Message)
}; // End of class Message.
} // End of namespace Irc.
} // End of namespace Aki.

Q_DECLARE_METATYPE(Aki::Irc::Message)

QDebug LIBAKI_EXPORT operator<<(QDebug dbg, const Aki::Irc::Message& message);

#endif // AKI_IRC_MESSAGE_HPP
