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

void
BaseSocketPrivate::appendSslError(QList<Aki::Irc::BaseSocket::SslError>& list,
                              Aki::Irc::BaseSocket::SslError error)
{
    if (list.contains(Aki::Irc::BaseSocket::NoError)) {
        list.removeAll(Aki::Irc::BaseSocket::NoError);
    }

    if (list.contains(error)) {
        list.removeAll(error);
        list.append(error);
    }
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
BaseSocketPrivate::error(KTcpSocket::Error error)
{
    Aki::Irc::BaseSocket::SocketError myError = Aki::Irc::BaseSocket::UnknownError;

    switch (error) {
    case KTcpSocket::ConnectionRefusedError: {
        myError = Aki::Irc::BaseSocket::ConnectionRefusedError;
        break;
    }
    case KTcpSocket::HostNotFoundError: {
        myError = Aki::Irc::BaseSocket::HostNotFoundError;
        break;
    }
    case KTcpSocket::NetworkError: {
        myError = Aki::Irc::BaseSocket::NetworkError;
        break;
    }
    case KTcpSocket::RemoteHostClosedError: {
        myError = Aki::Irc::BaseSocket::RemoteHostClosedError;
        break;
    }
    case KTcpSocket::SocketAccessError: {
        myError = Aki::Irc::BaseSocket::SocketAccessError;
        break;
    }
    case KTcpSocket::SocketResourceError: {
        myError = Aki::Irc::BaseSocket::SocketResourceError;
        break;
    }
    case KTcpSocket::SocketTimeoutError: {
        myError = Aki::Irc::BaseSocket::SocketTimeoutError;
        break;
    }
    case KTcpSocket::UnknownError: {
        myError = Aki::Irc::BaseSocket::UnknownError;
        break;
    }
    case KTcpSocket::UnsupportedSocketOperationError: {
        myError = Aki::Irc::BaseSocket::UnsupportedSocketOperationError;
        break;
    }
    default: {
        myError = Aki::Irc::BaseSocket::UnknownError;
    }
    }

    emit _q->error(myError);
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
BaseSocketPrivate::socketState(KTcpSocket::State state)
{
    Aki::Irc::BaseSocket::SocketState myState = Aki::Irc::BaseSocket::UnconnectState;

    switch (state) {
    case KTcpSocket::ClosingState: {
        myState = Aki::Irc::BaseSocket::ClosingState;
        break;
    }
    case KTcpSocket::ConnectedState: {
        myState = Aki::Irc::BaseSocket::ConnectedState;
        break;
    }
    case KTcpSocket::ConnectingState: {
        myState = Aki::Irc::BaseSocket::ConnectingState;
        break;
    }
    case KTcpSocket::HostLookupState: {
        myState = Aki::Irc::BaseSocket::HostLookupState;
        break;
    }
    case KTcpSocket::UnconnectedState: {
        myState = Aki::Irc::BaseSocket::UnconnectState;
        break;
    }
    default: {
    }
    }

    emit _q->stateChanged(myState);
}

void
BaseSocketPrivate::sslErrors(const QList<KSslError>& errors)
{
    QListIterator<KSslError> errorIter(errors);

    // Predefine it with NoError.
    QList<Aki::Irc::BaseSocket::SslError> myError =
        QList<Aki::Irc::BaseSocket::SslError>() << Aki::Irc::BaseSocket::NoError;

    while (errorIter.hasNext()) {
        KSslError error = errorIter.next();

        switch (error.error()) {
        case KSslError::CertificateSignatureFailed: {
            appendSslError(myError, Aki::Irc::BaseSocket::CertificateSignatureFailed);
            break;
        }
        case KSslError::ExpiredCertificate: {
            appendSslError(myError, Aki::Irc::BaseSocket::ExpiredCertificate);
            break;
        }
        case KSslError::HostNameMismatch: {
            appendSslError(myError, Aki::Irc::BaseSocket::HostNameMismatch);
            break;
        }
        case KSslError::InvalidCertificate: {
            appendSslError(myError, Aki::Irc::BaseSocket::InvalidCertificate);
            break;
        }
        case KSslError::InvalidCertificateAuthorityCertificate: {
            appendSslError(myError, Aki::Irc::BaseSocket::InvalidCertificateAuthorityCertificate);
            break;
        }
        case KSslError::InvalidCertificatePurpose: {
            appendSslError(myError, Aki::Irc::BaseSocket::InvalidCertificatePurpose);
            break;
        }
        case KSslError::NoError: {
            if (!myError.isEmpty()) {
                myError.clear();
                myError.append(Aki::Irc::BaseSocket::NoError);
            }
            break;
        }
        case KSslError::NoPeerCertificate: {
            appendSslError(myError, Aki::Irc::BaseSocket::NoPeerCertificate);
            break;
        }
        case KSslError::PathLengthExceeded: {
            appendSslError(myError, Aki::Irc::BaseSocket::PathLengthExceeded);
            break;
        }
        case KSslError::RejectedCertificate: {
            appendSslError(myError, Aki::Irc::BaseSocket::RejectedCertificate);
            break;
        }
        case KSslError::RevokedCertificate: {
            appendSslError(myError, Aki::Irc::BaseSocket::RevokedCertificate);
            break;
        }
        case KSslError::SelfSignedCertificate: {
            appendSslError(myError, Aki::Irc::BaseSocket::SelfSignedCertificate);
            break;
        }
        case KSslError::UnknownError: {
            appendSslError(myError, Aki::Irc::BaseSocket::UnknownSslError);
            break;
        }
        case KSslError::UntrustedCertificate: {
            appendSslError(myError, Aki::Irc::BaseSocket::UntrustedCertificate);
            break;
        }
        default: {
            break;
        }
        }

        emit _q->sslErrors(myError);
    }
}
