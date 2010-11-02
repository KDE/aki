#ifndef AKI_IRC_MESSAGE_HPP
#define AKI_IRC_MESSAGE_HPP

#include "aki.hpp"
#include "nickinfo.hpp"
#include "replycodes.hpp"
#include <QtCore/QObject>
#include <QtCore/QStringList>

class QTextCodec;
namespace Aki
{
namespace Irc
{
class MessagePrivate;
class LIBAKI_EXPORT Message
{
public:
    Message();
    explicit Message(const QString& line);
    Message(const Aki::Irc::Message& message);
    Aki::Irc::Message& operator=(const Aki::Irc::Message& rhs);
    virtual ~Message();
    bool isNumeric() const;
    QString command() const;
    QString message() const;
    Aki::Irc::ReplyCodes numeric() const;
    QStringList params() const;
    void parse(const QString& str);
    Aki::Irc::NickInfo sender() const;
private:
    AKI_DECLARE_PRIVATE(Message)
}; // End of class Message.
} // End of namespace Irc.
} // End of namespace Aki.

Q_DECLARE_METATYPE(Aki::Irc::Message)

QDebug LIBAKI_EXPORT operator<<(QDebug dbg, const Aki::Irc::Message& message);

#endif // AKI_IRC_MESSAGE_HPP
