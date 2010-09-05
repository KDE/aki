#ifndef AKI_SERVERVIEWTABPRIVATE_HPP
#define AKI_SERVERVIEWTABPRIVATE_HPP

#include <QtCore/QObject>

class QSplitter;

namespace Aki
{
namespace Irc
{
class Socket;
} // End of namespace Irc.
class ChannelContainer;
class ServerViewTab;
class ServerViewTabPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ServerViewTabPrivate(Aki::ServerViewTab* qq);
public:
    Aki::Irc::Socket* socket;
    Aki::ChannelContainer* channelContainer;
    QSplitter* verticalSplitter;
private:
    Aki::ServerViewTab* _q;
}; // End of class ServerViewTabPrivate.
} // End of namespace Aki.

#endif // AKI_SERVERVIEWTABPRIVATE_HPP
