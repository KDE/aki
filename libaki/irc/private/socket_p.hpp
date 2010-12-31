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

#ifndef AKI_IRC_SOCKET_P_HPP
#define AKI_IRC_SOCKET_P_HPP

#include "irc/replycodes.hpp"
#include "irc/socket.hpp"

namespace Aki
{
namespace Irc
{
class Message;
class SocketPrivate
{
public:
    explicit SocketPrivate(Aki::Irc::Socket* qq);
    void error(Aki::Irc::BaseSocket::SocketError error);
    void rawMessageReceived(const QString& message);
    void sslErrors(const QList<Aki::Irc::BaseSocket::SslError>& errors);
    void stateChanged(Aki::Irc::BaseSocket::SocketState state);
public:
    Aki::Irc::Socket::ServerCapabilities serverCapabilities;
    bool isMotdEnabled;
    bool isSaslEnabled;
private:
    QString removeStringToFirstWhitespace(QString* line);
    QString removeStringToFirstWhitespace(QString* line, int start, int stop);
private:
    class Message
    {
    public:
        Aki::Irc::NickInfo sender;
        QString command;
        QString message;
        QStringList params;
        Aki::Irc::ReplyCodes replyCode;
    }; // End of class Message.
    void commandReceived(const Aki::Irc::ReplyInfo& message);
    void messageReceived(const Aki::Irc::ReplyInfo& message);
private:
    Aki::Irc::Socket* _q;
}; // End of class SocketPrivate.
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_SOCKET_P_HPP
