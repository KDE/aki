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

#include "sockettest.hpp"

SocketTest::SocketTest(QObject* parent)
    : QObject(parent),
    _socket(new Aki::Irc::Socket("Freenode", parent))
{
    QStringList addresses;
    addresses << "irc.freenode.net/6667";
    _socket->setAddressList(addresses);
    _socket->setAutoIdentify(false);
    _socket->setAutoReconnect(true);
    _socket->setEncoding("UTF-8");
    _socket->setIdentName("testBot");
    _socket->setNickList(QStringList()<< "testBot1" << "testBot2" << "testBot3");
    _socket->setRealName("Aki test bot");
    _socket->setRetryAttemptCount(10);
    _socket->setRetryInterval(10);
    _socket->setServerPassword(QString());
    _socket->setServiceName("nickserv");
    _socket->setServicePassword(QString());
    _socket->setSsl(false);

    connect(_socket, SIGNAL(onMotdMessage(Aki::Irc::MotdReply)),
            SLOT(slotOnMotdMessage(Aki::Irc::MotdReply)));
    connect(_socket, SIGNAL(onNoticeReply(Aki::Irc::NoticeReply)),
            SLOT(slotOnNoticeReply(Aki::Irc::NoticeReply)));
    connect(_socket, SIGNAL(onStartupReply(Aki::Irc::StartupReply)),
            SLOT(slotOnStartupReply(Aki::Irc::StartupReply)));
    connect(_socket, SIGNAL(onLUserMessage(Aki::Irc::LUserReply)),
            SLOT(slotOnLUserReply(Aki::Irc::LUserReply)));
}

SocketTest::~SocketTest()
{
}

void
SocketTest::connectToHost()
{
    Q_ASSERT(_socket);
    _socket->connectToHost();
}

void
SocketTest::slotOnLUserReply(const Aki::Irc::LUserReply& reply)
{
    qDebug() << reply.message();
}

void
SocketTest::slotOnMotdMessage(const Aki::Irc::MotdReply& reply)
{
    qDebug() << reply.message();
    qDebug() << reply.isLastMessage();
}

void
SocketTest::slotOnNoticeReply(const Aki::Irc::NoticeReply& reply)
{
    qDebug() << reply.message();
}

void
SocketTest::slotOnStartupReply(const Aki::Irc::StartupReply& reply)
{
    qDebug() << reply.message();
}
