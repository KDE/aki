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

#include "basesocket.hpp"
#include "debughelper.hpp"
#include "irc/rfc2812.hpp"
#include "irc/private/basesocket_p.hpp"
#include <QtCore/QTextCodec>
#include <QtNetwork/QNetworkProxy>
using namespace Aki::Irc;

BaseSocket::BaseSocket(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::Irc::BaseSocketPrivate(this));
    setName("Default");

    _d->socket = new QSslSocket(this);
    Q_CHECK_PTR(_d->socket);

    connect(_d->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            SLOT(socketState(QAbstractSocket::SocketState)));
    connect(_d->socket, SIGNAL(error(QAbstractSocket::SocketError)),
            SLOT(error(QAbstractSocket::SocketError)));
    connect(_d->socket, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));
    connect(_d->socket, SIGNAL(readyRead()),
            SLOT(readyRead()));
}

BaseSocket::BaseSocket(const QString& name, QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::Irc::BaseSocketPrivate(this));
    setName(name);

    _d->socket = new QSslSocket(this);
    Q_CHECK_PTR(_d->socket);

    connect(_d->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            SLOT(socketState(QAbstractSocket::SocketState)));
    connect(_d->socket, SIGNAL(error(QAbstractSocket::SocketError)),
            SLOT(error(QAbstractSocket::SocketError)));
    connect(_d->socket, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));
    connect(_d->socket, SIGNAL(readyRead()),
            SLOT(readyRead()));
}

BaseSocket::~BaseSocket()
{
}

const QStringList&
BaseSocket::addressList() const
{
    return _d->addressList;
}

void
BaseSocket::appendNick(const QString& nick)
{
    if (not _d->nickList.contains(nick)) {
        _d->nickList.append(nick);
        setCurrentNick(nick);
    }
}

QTextCodec*
BaseSocket::codec() const
{
    return _d->codec;
}

void
BaseSocket::connectToHost()
{
    const QString addy = addressList().value(_d->addressListIndex);
    if (addy.contains('/')) {
        const QStringList tmp = addy.split('/');
        _d->address = tmp[0];

        bool ok = false;
        _d->port = tmp[1].toUShort(&ok);

        if (not ok) {
            _d->port = 6667;
        }
    } else {
        _d->address = addy;
        _d->port = 6667;
    }

    connectToHost(currentAddress(), currentPort());
}

void
BaseSocket::connectToHost(const QString& address, quint16 port)
{
    if (isSslEnabled()) {
        _d->socket->connectToHostEncrypted(address, port);
    } else {
        _d->socket->connectToHost(address, port);
    }
}

QString
BaseSocket::currentAddress() const
{
    return _d->address;
}

QString
BaseSocket::currentNick() const
{
    return _d->nickList.value(_d->nickListIndex);
}

quint16
BaseSocket::currentPort() const
{
    return _d->port;
}

void
BaseSocket::disconnectFromHost()
{
    _d->socket->disconnectFromHost();
    _d->isSelfDisconnect = true;
}

QString
BaseSocket::identName() const
{
    return _d->identName;
}

void
BaseSocket::ignoreSslErrors()
{
    _d->socket->ignoreSslErrors();
}

bool
BaseSocket::isAutoIdentifyEnabled() const
{
    return _d->isAutoIdentify;
}

bool
BaseSocket::isAutoReconnectEnabled() const
{
    return _d->isAutoReconnect;
}

bool
BaseSocket::isSslEnabled() const
{
    return _d->isSsl;
}

QString
BaseSocket::name() const
{
    return _d->name;
}

QString
BaseSocket::nextAvailableNick() const
{
    const int nextIndex = _d->nickListIndex + 1;

    if (nextIndex > nickList().count()) {
        return nickList()[0];
    } else {
        return nickList()[nextIndex];
    }
}

QStringList
BaseSocket::nickList() const
{
    return _d->nickList;
}

QSslCertificate
Aki::Irc::BaseSocket::peerCertificate() const
{
    return _d->socket->peerCertificate();
}

QString
BaseSocket::realName() const
{
    return _d->realName;
}

int
BaseSocket::retryAttemptCount() const
{
    return _d->retryAttempts;
}

int
BaseSocket::retryInterval() const
{
    return _d->retryInterval;
}

void
BaseSocket::sendMessage(const Aki::Irc::Message& message)
{
    DEBUG_FUNC_NAME;
    DEBUG_TEXT(_d->codec->fromUnicode(message.plainText()).replace("\r\n", ""))
    _d->socket->write(_d->codec->fromUnicode(message.plainText() + "\r\n"));
    _d->socket->flush();
}

QString
BaseSocket::serverPassword() const
{
    return _d->serverPassword;
}

QString
BaseSocket::serviceName() const
{
    return _d->serviceName;
}

QString
BaseSocket::servicePassword() const
{
    return _d->servicePassword;
}

QSslCipher
Aki::Irc::BaseSocket::sessionCipher() const
{
    return _d->socket->sessionCipher();
}

void
BaseSocket::setAddressList(const QStringList& addresses)
{
    if (_d->addressList.isEmpty()) {
        _d->addressList = addresses;
    }
}

void
BaseSocket::setAutoIdentify(bool enable)
{
    _d->isAutoIdentify = enable;
}

void
BaseSocket::setAutoReconnect(bool enable)
{
    _d->isAutoReconnect = enable;
}

void
BaseSocket::setCurrentNick(const QString& nick)
{
    const int index = _d->nickList.indexOf(nick);
    if (index >= 0) {
        _d->nickListIndex = index;
    }
}

void
BaseSocket::setEncoding(const QByteArray& name)
{
    _d->codec = QTextCodec::codecForName(name);
}

void
BaseSocket::setIdentName(const QString& identity)
{
    _d->identName = identity;
}

void
BaseSocket::setName(const QString& name)
{
    _d->name = name;
}

void
BaseSocket::setNickList(const QStringList& nicks)
{
    if (!nicks.isEmpty()) {
        _d->nickList = nicks;
    }
}

void
BaseSocket::setProxy(Aki::Irc::BaseSocket::ProxyType type, const QString& hostname, quint16 port,
                 const QString& user, const QString& password)
{
    QNetworkProxy proxy;
    proxy.setHostName(hostname);
    proxy.setPort(port);
    proxy.setUser(user);
    proxy.setPassword(password);

    switch (type) {
    case Aki::Irc::BaseSocket::Http: {
        proxy.setType(QNetworkProxy::HttpProxy);
        break;
    }
    case Aki::Irc::BaseSocket::NoProxy: {
        proxy.setType(QNetworkProxy::NoProxy);
        break;
    }
    case Aki::Irc::BaseSocket::Socks5: {
        proxy.setType(QNetworkProxy::Socks5Proxy);
        break;
    }
    default: {
        proxy.setType(QNetworkProxy::NoProxy);
        break;
    }
    }

    _d->socket->setProxy(proxy);
}

void
BaseSocket::setRealName(const QString& name)
{
    _d->realName = name;
}

void
BaseSocket::setRetryAttemptCount(int count)
{
    _d->retryAttempts = count;
}

void
BaseSocket::setRetryInterval(int seconds)
{
    _d->retryInterval = seconds;
}

void
BaseSocket::setServerPassword(const QString& password)
{
    _d->serverPassword = password;
}

void
BaseSocket::setServiceName(const QString& name)
{
    _d->serviceName = name;
}

void
BaseSocket::setServicePassword(const QString& password)
{
    _d->servicePassword = password;
}

void
BaseSocket::setSsl(bool enable)
{
    _d->isSsl = enable;
}

#include "irc/basesocket.moc"
