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

#ifndef AKI_IRC_BASESOCKET_P_HPP
#define AKI_IRC_BASESOCKET_P_HPP

#include "irc/basesocket.hpp"
#include <QtNetwork/QSslSocket>

class QTextCodec;
namespace Aki
{
namespace Irc
{
class BaseSocketPrivate
{
public:
    explicit BaseSocketPrivate(Aki::Irc::BaseSocket* qq);
    void error(QSslSocket::SocketError error);
    void readyRead();
    void socketState(QSslSocket::SocketState state);
    void sslErrors(const QList<QSslError>& errors);
public:
    QSslSocket* socket;
    QTextCodec* codec;
    QStringList addressList;
    QStringList nickList;
    QString address;
    QString identName;
    QString name;
    QString realName;
    QString serverPassword;
    QString serviceName;
    QString servicePassword;
    QByteArray data;
    bool isAutoIdentify;
    bool isAutoReconnect;
    bool isSelfDisconnect;
    bool isSsl;
    quint16 port;
    int addressListIndex;
    int nickListIndex;
    int retryAttempts;
    int retryInterval;
private:
    QByteArray detectUnicode(const QByteArray& data) const;
private:
    Aki::Irc::BaseSocket* _q;
}; // End of class BaseSocketPrivate.
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_BASESOCKET_P_HPP
