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

#ifndef SOCKETTEST_HPP
#define SOCKETTEST_HPP

#include "irc/socket.hpp"

class SocketTest
    : QObject
{
    Q_OBJECT
public:
    SocketTest(QObject* parent = 0);
    ~SocketTest();
    void connectToHost();
private Q_SLOTS:
    void slotOnMotdMessage(const Aki::Irc::MotdReply& reply);
    void slotOnStartupReply(const Aki::Irc::StartupReply& reply);
private:
    Aki::Irc::Socket* _socket;
};

#endif // SOCKETTEST_HPP
