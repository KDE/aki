#include "socket.hpp"
#include "debughelper.hpp"
#include "irc/commandevent.hpp"
#include "irc/ctcpreceiveevent.hpp"
#include "irc/ctcpreplyevent.hpp"
#include "irc/message.hpp"
#include "private/socket_p.hpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QMetaMethod>
using namespace Aki;
using namespace Irc;

Socket::Socket(QObject* parent)
    : Aki::Irc::BaseSocket(parent)
{
    DEBUG_FUNC_NAME;
    _d.reset(new Aki::Irc::SocketPrivate(this));
    connect(this, SIGNAL(error(Aki::Irc::BaseSocket::SocketError)),
            SLOT(error(Aki::Irc::BaseSocket::SocketError)));
    connect(this, SIGNAL(sslErrors(QList<Aki::Irc::BaseSocket::SslError>)),
            SLOT(sslErrors(QList<Aki::Irc::BaseSocket::SslError>)));
    connect(this, SIGNAL(stateChanged(Aki::Irc::BaseSocket::SocketState)),
            SLOT(stateChanged(Aki::Irc::BaseSocket::SocketState)));
    connect(this, SIGNAL(rawMessageReceived(Aki::Irc::Message)),
            SLOT(rawMessageReceived(Aki::Irc::Message)));
}

Socket::Socket(const QString& name, QObject* parent)
    : Aki::Irc::BaseSocket(name, parent)
{
    DEBUG_FUNC_NAME;
    _d.reset(new Aki::Irc::SocketPrivate(this));
    connect(this, SIGNAL(error(Aki::Irc::BaseSocket::SocketError)),
            SLOT(error(Aki::Irc::BaseSocket::SocketError)));
    connect(this, SIGNAL(sslErrors(QList<Aki::Irc::BaseSocket::SslError>)),
            SLOT(sslErrors(QList<Aki::Irc::BaseSocket::SslError>)));
    connect(this, SIGNAL(stateChanged(Aki::Irc::BaseSocket::SocketState)),
            SLOT(stateChanged(Aki::Irc::BaseSocket::SocketState)));
    connect(this, SIGNAL(rawMessageReceived(Aki::Irc::Message)),
            SLOT(rawMessageReceived(Aki::Irc::Message)));
}

Socket::~Socket()
{
    DEBUG_FUNC_NAME;
    disconnectFromHost();
}

void
Socket::connectSlotsBySignals(Socket* socket)
{
    DEBUG_FUNC_NAME;
    if (!socket) {
        return;
    }

    const QMetaObject* otherMetaObject = socket->metaObject();
    Q_ASSERT(otherMetaObject);
    const int otherMethodCount = otherMetaObject->methodCount();

    const QMetaObject* thisMetaObject = metaObject();
    Q_ASSERT(thisMetaObject);
    const int thisMethodCount = thisMetaObject->methodCount();

    QList<QByteArray> connectedSlots;

    for (int i = 0; i < otherMethodCount; ++i) {
        QMetaMethod slot = otherMetaObject->method(i);
        const char* otherSignature = slot.signature();
        Q_ASSERT(otherSignature);

        if (qstrncmp(otherSignature, "slot", 4)) {
            continue;
        }

        for (int j = 0; j < thisMethodCount; ++j) {
            QMetaMethod signal = thisMetaObject->method(j);
            if (signal.parameterTypes() != slot.parameterTypes()) {
                continue;
            }

            if (signal.methodType() == QMetaMethod::Signal) {
                const char* thisSignature = signal.signature();
                Q_ASSERT(thisSignature);

                if (qstrcmp(otherSignature + 4, thisSignature)) {
                    continue;
                }

                QByteArray otherName = QByteArray::fromRawData(otherSignature, qstrlen(otherSignature));
                if (!connectedSlots.contains(otherName)) {
                    connectedSlots << otherName;
                    DEBUG_TEXT3("Connecting signal: %1 To slot: %2", slot.typeName(), signal.typeName())
                    QMetaObject::connect(this, i, socket, j);
                }
            }
        }
    }
}

#include "irc/socket.moc"
