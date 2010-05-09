#ifndef AKI_CHATPARSER_P_HPP
#define AKI_CHATPARSER_P_HPP

#include <QtCore/QList>
#include <QtCore/QObject>
#include <boost/function.hpp>

namespace Aki {
class ChatParser;
class ChatParserPrivate : public QObject
{
    Q_OBJECT
public:
    typedef boost::function<bool (const QString&)> CommandHook;
    typedef CommandHook HelpHook;
    ChatParserPrivate(Aki::ChatParser* qq);
    void setupFunctions();
    bool help(const QString& command);
    bool command(const QString& command);
public:
    QList<HelpHook> helpHash;
    QList<CommandHook> commandHash;
private:
    Aki::ChatParser* _q;
}; // End of class ChatParserPrivate.
} // End of namespace Aki.

#endif // AKI_CHATPARSER_P_HPP
