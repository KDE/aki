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

#ifndef SOCKETTEST_HPP
#define SOCKETTEST_HPP

#include "irc/socket.hpp"
#include <QtNetwork/QSslCipher>

class SocketTest
    : QObject
{
    Q_OBJECT
public:
    SocketTest(QObject* parent = 0);
    ~SocketTest();
    void connectToHost();
private:
    void dumpCipher(const QSslCipher& cipher);
    void dumpCertificate(const QSslCertificate& cert);
private Q_SLOTS:
    void slotOnActionReply(const Aki::Irc::ActionReply& reply);
    void slotOnAwayReply(const Aki::Irc::AwayReply& reply);
    void slotOnChannelCreationReply(const Aki::Irc::ChannelCreationTimeReply& reply);
    void slotOnChannelMessageReply(const Aki::Irc::ChannelMessageReply& reply);
    void slotOnChannelModeReply(const Aki::Irc::ChannelModeReply& reply);
    void slotOnJoinReply(const Aki::Irc::JoinReply& reply);
    void slotOnKickReply(const Aki::Irc::KickReply& reply);
    void slotOnMotdReply(const Aki::Irc::MotdReply& reply);
    void slotOnNickReply(const Aki::Irc::NickReply& reply);
    void slotOnNoticeReply(const Aki::Irc::NoticeReply& reply);
    void slotOnPrivateMessageReply(const Aki::Irc::PrivateMessageReply& reply);
    void slotOnQuitReply(const Aki::Irc::QuitReply& reply);
    void slotSslErrors(const QList<QSslError>& errors);
private:
    Aki::Irc::Socket* _socket;
};

#endif // SOCKETTEST_HPP
