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

#ifndef AKI_IRC_BASESOCKET_HPP
#define AKI_IRC_BASESOCKET_HPP

#include "aki.hpp"
#include "irc/message.hpp"
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslCipher>
#include <QtNetwork/QSslError>

class QTextCodec;

namespace Aki
{
namespace Irc
{
class BaseSocketPrivate;
class LIBAKI_EXPORT BaseSocket
    : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList addressList READ addressList WRITE setAddressList)
    Q_PROPERTY(QString currentAddress READ currentAddress)
    Q_PROPERTY(quint16 currentPort READ currentPort)
    Q_PROPERTY(QString currentNick READ currentNick)
    Q_PROPERTY(QString identName READ identName WRITE setIdentName)
    Q_PROPERTY(bool isAutoIdentifyEnabled READ isAutoIdentifyEnabled WRITE setAutoIdentify)
    Q_PROPERTY(bool isAutoReconnect READ isAutoReconnectEnabled WRITE setAutoReconnect)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString realName READ realName WRITE setRealName)
    Q_PROPERTY(int retryAttemptCount READ retryAttemptCount WRITE setRetryAttemptCount)
    Q_PROPERTY(int retryInterval READ retryInterval WRITE setRetryInterval)
    Q_PROPERTY(QString serviceName READ serviceName WRITE setServiceName)
    Q_PROPERTY(QString servicePassword READ servicePassword WRITE setServicePassword)
    Q_PROPERTY(QString serverPassword READ serverPassword WRITE setServerPassword)
    Q_PROPERTY(bool isSslEnabled READ isSslEnabled WRITE setSsl)
public:
    enum SocketState {
        UnconnectState = 0,
        HostLookupState = 1,
        ConnectingState = 2,
        ConnectedState = 3,
        ClosingState = 4
    }; // End of enum SocketState.

    enum SocketError {
        ConnectionRefusedError = 0,
        RemoteHostClosedError = 1,
        HostNotFoundError = 2,
        SocketAccessError = 3,
        SocketResourceError = 4,
        SocketTimeoutError = 5,
        NetworkError = 7,
        UnsupportedSocketOperationError = 10,
        ProxyAuthenticationRequiredError = 12,
        SslHandshakeFailedError = 13,
        UnfinishedSocketOperationError = 11,
        ProxyConnectionRefusedError = 14,
        ProxyConnectionClosedError = 15,
        ProxyConnectionTimeoutError = 16,
        ProxyNotFoundError = 17,
        ProxyProtocolError = 18,
        UnknownSocketError = -1
    }; // End of enum SocketError.

    enum ProxyType {
        NoProxy = 0,
        Socks5 = 1,
        Http = 2
    }; // End of enum ProxyType.

    explicit BaseSocket(QObject* parent = 0);
    explicit BaseSocket(const QString& name, QObject* parent = 0);
    virtual ~BaseSocket();
    const QStringList& addressList() const;
    void appendNick(const QString& nick);
    QTextCodec* codec() const;
    QString currentAddress() const;
    QString currentNick() const;
    quint16 currentPort() const;
    void disconnectFromHost();
    QString identName() const;
    bool isAutoIdentifyEnabled() const;
    bool isAutoReconnectEnabled() const;
    bool isSslEnabled() const;
    QString name() const;
    QString nextAvailableNick() const;
    QStringList nickList() const;
    QSslCertificate peerCertificate() const;
    QString realName() const;
    int retryAttemptCount() const;
    int retryInterval() const;
    virtual void sendMessage(const Aki::Irc::Message& message);
    QString serverPassword() const;
    QString serviceName() const;
    QString servicePassword() const;
    QSslCipher sessionCipher() const;
    void setAddressList(const QStringList& addresses);
    void setAutoIdentify(bool enable);
    void setAutoReconnect(bool enable);
    void setCurrentNick(const QString& nick);
    void setEncoding(const QByteArray& name);
    void setIdentName(const QString& identity);
    void setName(const QString& name);
    void setNickList(const QStringList& nicks);
    void setProxy(Aki::Irc::BaseSocket::ProxyType type, const QString& hostname = QString(), quint16 port = 0,
                  const QString& user = QString(), const QString& password = QString());
    void setRealName(const QString& name);
    void setRetryAttemptCount(int count);
    void setRetryInterval(int seconds);
    void setServiceName(const QString& name);
    void setServicePassword(const QString& password);
    void setServerPassword(const QString& password);
    void setSsl(bool enable);
public Q_SLOTS:
    void connectToHost();
    void ignoreSslErrors();
Q_SIGNALS:
    void error(Aki::Irc::BaseSocket::SocketError error);
    void rawMessageReceived(const QString& message);
    void sslErrors(const QList<QSslError>& errors);
    void stateChanged(Aki::Irc::BaseSocket::SocketState state);
protected:
    virtual void connectToHost(const QString& address, quint16 port = 6667);
private:
    Q_PRIVATE_SLOT(_d, void error(QAbstractSocket::SocketError error))
    Q_PRIVATE_SLOT(_d, void readyRead())
    Q_PRIVATE_SLOT(_d, void socketState(QAbstractSocket::SocketState state))
    Q_PRIVATE_SLOT(_d, void sslErrors(const QList<QSslError>& errors))
private:
    AKI_DECLARE_PRIVATE(BaseSocket)
}; // End of class BaseSocket.
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_BASESOCKET_HPP
