#ifndef AKI_IRC_MESSAGE_P_HPP
#define AKI_IRC_MESSAGE_P_HPP

#include "irc/nickinfo.hpp"
#include "irc/replycodes.hpp"
#include <QtCore/QString>
#include <QtCore/QStringList>

namespace Aki
{
namespace Irc
{
class Message;
class MessagePrivate
{
public:
    explicit MessagePrivate(Aki::Irc::Message* qq);
    /**
     * Removes string from the @p line to starting at 0 index to the first whitespace.
     *
     * @param line Modifies the string after it is done.
     *
     * @return String that was taken.
     */
    QString removeStringToFirstWhitespace(QString* line);
    /**
     * Removes the string from the @p line starting at @p start and ending at @p stop.
     *
     * @param line Modifies to string after it is done.
     * @param start Start position index.
     * @param stop Stop position index.
     *
     * @return String that was taken.
     */
    QString removeStringToFirstWhitespace(QString* line, int start, int stop);
public:
    Aki::Irc::NickInfo sender;
    QString command;
    QString message;
    QStringList params;
    Aki::Irc::ReplyCodes numeric;
    bool isNumeric;
private:
    Aki::Irc::Message* _q;
}; // End of class MessagePrivate.
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_MESSAGE_P_HPP
