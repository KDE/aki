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

#include "basesocket_p.hpp"
#include "debughelper.hpp"
#include "irc/message.hpp"
#include <QtCore/QTextCodec>
#include <unicode/ucsdet.h>
using namespace Aki;
using namespace Irc;

BaseSocketPrivate::BaseSocketPrivate(Aki::Irc::BaseSocket* qq)
    : socket(0),
    codec(0),
    identName(QString()),
    name(QString()),
    realName(QString()),
    serverPassword(QString()),
    serviceName(QString()),
    servicePassword(QString()),
    isAutoIdentify(false),
    isAutoReconnect(false),
    isSelfDisconnect(false),
    isSsl(false),
    port(0),
    addressListIndex(0),
    nickListIndex(0),
    retryAttempts(0),
    retryInterval(0),
    _q(qq)
{
    addressList.clear();
}

QByteArray
BaseSocketPrivate::detectUnicode(const QByteArray& data) const
{
    QByteArray encoding("");
    UErrorCode status = U_ZERO_ERROR;
    UCharsetDetector* detector = ucsdet_open(&status);

    if (detector and not U_FAILURE(status)) {
        ucsdet_setText(detector, data.constData(), data.length(), &status);
        if (not U_FAILURE(status)) {
            const UCharsetMatch* match = ucsdet_detect(detector, &status);
            if (match and not U_FAILURE(status)) {
                encoding = ucsdet_getName(match, &status);
            }
        }
    }

    if (U_FAILURE(status)) {
        kError() << QString("Detecting encoding failed: %1").arg(u_errorName(status));
    }

    ucsdet_close(detector);
    return encoding;
}

void
BaseSocketPrivate::error(QAbstractSocket::SocketError error)
{
    emit _q->error(static_cast<Aki::Irc::BaseSocket::SocketError>(error));
}

void
BaseSocketPrivate::readyRead()
{
    data += socket->readAll();

    int i = -1;
    while ((i = data.indexOf("\r\n")) != -1) {
        QByteArray line = data.left(i).trimmed();
        data = data.mid(i + 2);
        if (!line.isEmpty()) {
            QTextCodec* codec = QTextCodec::codecForName(detectUnicode(line));
            emit _q->rawMessageReceived(codec->toUnicode(line));
        }
    }
}

void
BaseSocketPrivate::socketState(QAbstractSocket::SocketState state)
{
    emit _q->stateChanged(static_cast<Aki::Irc::BaseSocket::SocketState>(state));
}

void
BaseSocketPrivate::sslErrors(const QList<QSslError>& errors)
{
    emit _q->sslErrors(errors);
}
