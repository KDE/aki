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

#include "sockettest.hpp"
#include "irc/rfc2812.hpp"
#include "irc/text.hpp"

SocketTest::SocketTest(QObject* parent)
    : QObject(parent),
    _socket(new Aki::Irc::Socket("Freenode", parent))
{
    _socket->setAddressList(QStringList() << "irc.freenode.net");
    _socket->setAutoIdentify(false);
    _socket->setAutoReconnect(true);
    _socket->setEncoding("UTF-8");
    _socket->setIdentName("testBot");
    _socket->setNickList(QStringList() << "AkiBot" << "akibot_" << "akibot__");
    _socket->setRealName("Aki test bot");
    _socket->setRetryAttemptCount(10);
    _socket->setRetryInterval(10);
    _socket->setServerPassword(QString());
    _socket->setServiceName("nickserv");
    _socket->setSsl(true);
    _socket->setSasl(false);

    connect(_socket, SIGNAL(onActionReply(Aki::Irc::ActionReply)),
            SLOT(slotOnActionReply(Aki::Irc::ActionReply)));
    connect(_socket, SIGNAL(onAwayReply(Aki::Irc::AwayReply)),
            SLOT(slotOnAwayReply(Aki::Irc::AwayReply)));
    connect(_socket, SIGNAL(onChannelCreationTimeReply(Aki::Irc::ChannelCreationTimeReply)),
            SLOT(slotOnChannelCreationReply(Aki::Irc::ChannelCreationTimeReply)));
    connect(_socket, SIGNAL(onChannelMessageReply(Aki::Irc::ChannelMessageReply)),
            SLOT(slotOnChannelMessageReply(Aki::Irc::ChannelMessageReply)));
    connect(_socket, SIGNAL(onChannelModeReply(Aki::Irc::ChannelModeReply)),
            SLOT(slotOnChannelModeReply(Aki::Irc::ChannelModeReply)));
    connect(_socket, SIGNAL(onJoinReply(Aki::Irc::JoinReply)),
            SLOT(slotOnJoinReply(Aki::Irc::JoinReply)));
    connect(_socket, SIGNAL(onKickReply(Aki::Irc::KickReply)),
            SLOT(slotOnKickReply(Aki::Irc::KickReply)));
    connect(_socket, SIGNAL(onMotdReply(Aki::Irc::MotdReply)),
            SLOT(slotOnMotdReply(Aki::Irc::MotdReply)));
    connect(_socket, SIGNAL(onNickReply(Aki::Irc::NickReply)),
            SLOT(slotOnNickReply(Aki::Irc::NickReply)));
    connect(_socket, SIGNAL(onPrivateMessageReply(Aki::Irc::PrivateMessageReply)),
            SLOT(slotOnPrivateMessageReply(Aki::Irc::PrivateMessageReply)));
    connect(_socket, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(slotSslErrors(QList<QSslError>)));
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
SocketTest::dumpCipher(const QSslCipher& cipher)
{
    qDebug() << "\n== Cipher ==";
    qDebug() << "Authentication:\t\t" << cipher.authenticationMethod();
    qDebug() << "Encryption:\t\t" << cipher.encryptionMethod();
    qDebug() << "Key Exchange:\t\t" << cipher.keyExchangeMethod();
    qDebug() << "Cipher Name:\t\t" << cipher.name();
    qDebug() << "Protocol:\t\t" << cipher.protocolString();
    qDebug() << "Supported Bits:\t\t" << cipher.supportedBits();
    qDebug() << "Used Bits:\t\t" << cipher.usedBits();
}

void
SocketTest::dumpCertificate(const QSslCertificate& cert)
{
    qDebug() << cert.toPem();

    qDebug() << "== Subject Info ==\b";
    qDebug() << "CommonName:\t\t" << cert.subjectInfo( QSslCertificate::CommonName );
    qDebug() << "Organization:\t\t" << cert.subjectInfo( QSslCertificate::Organization );
    qDebug() << "LocalityName:\t\t" << cert.subjectInfo( QSslCertificate::LocalityName );
    qDebug() << "OrganizationalUnitName:\t" << cert.subjectInfo( QSslCertificate::OrganizationalUnitName );
    qDebug() << "StateOrProvinceName:\t" << cert.subjectInfo( QSslCertificate::StateOrProvinceName );

    QMultiMap<QSsl::AlternateNameEntryType, QString> altNames = cert.alternateSubjectNames();
    if ( !altNames.isEmpty() ) {
        qDebug() << "Alternate Subject Names (DNS):";
        foreach (const QString& altName, altNames.values(QSsl::DnsEntry)) {
            qDebug() << altName;
        }

        qDebug() << "Alternate Subject Names (Email):";
        foreach (const QString &altName, altNames.values(QSsl::EmailEntry)) {
            qDebug() << altName;
        }
    }

    qDebug() << "\n== Issuer Info ==";
    qDebug() << "CommonName:\t\t" << cert.issuerInfo( QSslCertificate::CommonName );
    qDebug() << "Organization:\t\t" << cert.issuerInfo( QSslCertificate::Organization );
    qDebug() << "LocalityName:\t\t" << cert.issuerInfo( QSslCertificate::LocalityName );
    qDebug() << "OrganizationalUnitName:\t" << cert.issuerInfo( QSslCertificate::OrganizationalUnitName );
    qDebug() << "StateOrProvinceName:\t" << cert.issuerInfo( QSslCertificate::StateOrProvinceName );

    qDebug() << "\n== Certificate ==";
    //qDebug() << "Serial Number:\t\t" << cert.serialNumber(); // This seems buggy
    qDebug() << "Effective Date:\t\t" << cert.effectiveDate().toString();
    qDebug() << "Expiry Date:\t\t" << cert.expiryDate().toString();
    qDebug() << "Valid:\t\t\t" << (cert.isValid() ? "Yes" : "No");
}

void
SocketTest::slotOnActionReply(const Aki::Irc::ActionReply& reply)
{
    qDebug() << QString("[%1]-[%2]: * %3").arg(reply.channel(), reply.reply().sender().nick(), reply.param());
}

void
SocketTest::slotOnAwayReply(const Aki::Irc::AwayReply& reply)
{
    qDebug() << reply.nick() << ": " << reply.message();
}

void
SocketTest::slotOnChannelCreationReply(const Aki::Irc::ChannelCreationTimeReply& reply)
{
    qDebug() << QString("[%1] Created: %2").arg(reply.channel(), reply.time().toString(Qt::SystemLocaleDate));
}

void
SocketTest::slotOnChannelMessageReply(const Aki::Irc::ChannelMessageReply& reply)
{
    qDebug() << QString("[%1]-[%2]: %3").arg(reply.channel(), reply.reply().sender().nick(), reply.message());
}

void
SocketTest::slotOnChannelModeReply(const Aki::Irc::ChannelModeReply& reply)
{
    QString info;
    QStringList enabledModes;
    QStringList disabledModes;
    bool isEnabled = false;
    foreach (const QChar& ch, reply.modes()) {
        if (ch == '+') {
            isEnabled = true;
        } else if (ch == '-') {
            isEnabled = false;
        } else if (ch == 'n') {
            if (isEnabled) {
                enabledModes << "No outside messages allowed";
            } else {
                disabledModes << "Outside messages allowed";
            }
        } else if (ch == 't') {
            if (isEnabled) {
                enabledModes << "Topic restriction";
            } else {
                disabledModes << "No topic restriction";
            }
        } else if (ch == 's') {
            if (isEnabled) {
                enabledModes << "Secret channel";
            } else {
                disabledModes << "Channel is not secret";
            }
        } else if (ch == 'p') {
            if (isEnabled) {
                enabledModes << "Private channel";
            } else {
                disabledModes << "Channel is not private";
            }
        } else if (ch == 'm') {
            if (isEnabled) {
                enabledModes << "Moderated channel";
            } else {
                disabledModes << "Channel is not moderated";
            }
        } else if (ch == 'i') {
            if (isEnabled) {
                enabledModes << "Invite only";
            } else {
                disabledModes << "Channel is not moderated";
            }
        } else if (ch == 'r') {
            if (isEnabled) {
                enabledModes << "Registered only";
            } else {
                disabledModes << "Channel is open to anyone";
            }
        } else if (ch == 'c') {
            if (isEnabled) {
                enabledModes << "No colour allowed";
            } else {
                disabledModes << "Colour is allowed";
            }
        } else if (ch == 'g') {
            if (isEnabled) {
                enabledModes << "Free invites";
            } else {
                disabledModes << "Ops can only send invites";
            }
        } else if (ch == 'z') {
            if (isEnabled) {
                enabledModes << "Operator moderation";
            } else {
                disabledModes << "Not operator moderated";
            }
        } else if (ch == 'L') {
            if (isEnabled) {
                enabledModes << "Large ban list";
            } else {
                disabledModes << "No large ban list enabled";
            }
        } else if (ch == 'P') {
            if (isEnabled) {
                enabledModes << "Permanent channel";
            } else {
                disabledModes << "Not a permanet channel";
            }
        } else if (ch == 'F') {
            if (isEnabled) {
                enabledModes << "Free targets";
            } else {
                disabledModes << "Only ops can sent forwards";
            }
        } else if (ch == 'Q') {
            if (isEnabled) {
                enabledModes << "Forwards are disabled";
            } else {
                disabledModes << "Forwards are enabled";
            }
        } else if (ch == 'C') {
            if (isEnabled) {
                enabledModes << "CTCP is disabled";
            } else {
                disabledModes << "CTCP is enabled";
            }
        }
    }

    if (enabledModes.count()) {
        qDebug() << QString("[%1] Channel modes enabled: %2").arg(reply.channel(), enabledModes.join(", "));
    }

    if (disabledModes.count()) {
        qDebug() << QString("[%1] Channel modes disabled: %2").arg(reply.channel(), disabledModes.join(", "));
    }
}

void
SocketTest::slotOnJoinReply(const Aki::Irc::JoinReply& reply)
{
    if (reply.reply().sender().nick() == _socket->currentNick()) {
        qDebug() << "You joined: " << reply.channel();
    } else {
        qDebug() << reply.reply().sender().nick() << " joined: " << reply.channel();
    }
}

void
SocketTest::slotOnKickReply(const Aki::Irc::KickReply& reply)
{
    if (reply.message().isEmpty()) {
        qDebug() << "[" << reply.channel() << "] " << reply.reply().sender().nick() << " kicked " << reply.nick();
    } else {
        qDebug() << "[" << reply.channel() << "] " << reply.reply().sender().nick() << " kicked " << reply.nick() <<
                 " with message: " << reply.message();
    }
}

void
SocketTest::slotOnMotdReply(const Aki::Irc::MotdReply& reply)
{
    if (!reply.isLastMessage()) {
        qDebug() << "[MOTD] " << reply.message();
    } else {
        //_socket->sendMessage(Aki::Irc::Rfc2812::join("#botters"));
        //_socket->sendMessage(Aki::Irc::Rfc2812::join("#botters-test"));
        _socket->sendMessage(Aki::Irc::Rfc2812::join("#bba"));
        _socket->sendMessage(Aki::Irc::Rfc2812::mode("#bba"));
        _socket->sendMessage(Aki::Irc::Rfc2812::away("I'm away"));
    }
}

void
SocketTest::slotOnNickReply(const Aki::Irc::NickReply& reply)
{
    qDebug() << reply.oldNick() << " changed nick to " << reply.newNick();
}

void
SocketTest::slotOnNoticeReply(const Aki::Irc::NoticeReply& reply)
{
    qDebug() << "[NOTICE] " << reply.message();
}

void
SocketTest::slotOnPrivateMessageReply(const Aki::Irc::PrivateMessageReply& reply)
{
    qDebug() << "You received a private message from: " << reply.reply().sender().nick();
    qDebug() << "Message: " << reply.message();
}

void
SocketTest::slotOnQuitReply(const Aki::Irc::QuitReply& reply)
{
    if (reply.message().isEmpty()) {
        qDebug() << reply.reply().sender().nick() << " left the server.";
    } else {
        qDebug() << reply.reply().sender().nick() << " left the server: " << reply.message();
    }
}

void
SocketTest::slotSslErrors(const QList<QSslError>& errors)
{
    Q_UNUSED(errors)
    qDebug() << "=== Peer Certificate ===";

    QSslCertificate cert = _socket->peerCertificate();
    dumpCertificate(cert);

    QSslCipher cipher = _socket->sessionCipher();
    dumpCipher(cipher);

    _socket->ignoreSslErrors();
}
