#ifndef AKI_IRC_BASESOCKET_P_HPP
#define AKI_IRC_BASESOCKET_P_HPP

#include "irc/basesocket.hpp"
#include <ktcpsocket.h>

class QTextCodec;
namespace Aki
{
namespace Irc
{
class BaseSocketPrivate
{
public:
    explicit BaseSocketPrivate(Aki::Irc::BaseSocket* qq);
    void appendSslError(QList<Aki::Irc::BaseSocket::SslError>& list,
                        Aki::Irc::BaseSocket::SslError error);
    void error(KTcpSocket::Error error);
    void readyRead();
    void socketState(KTcpSocket::State state);
    void sslErrors(const QList<KSslError>& errors);
public:
    KTcpSocket* socket;
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
