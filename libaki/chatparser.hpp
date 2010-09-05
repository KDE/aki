#ifndef AKI_CHATPARSER_HPP
#define AKI_CHATPARSER_HPP

#include "aki.hpp"
#include <QtCore/QObject>
#include <boost/function.hpp>

namespace Aki
{
class ChatParserPrivate;
class ChatParser : public QObject
{
    Q_OBJECT
public:
    ChatParser(QObject* parent = 0);
    virtual ~ChatParser();
    void parse(const QString& data);
    void addHelp(const boost::function<bool (const QString&)>& help);
    void addCommand(const boost::function<bool (const QString&)>& command);
private:
    AKI_DECLARE_PRIVATE(ChatParser)
}; // End of class ChatParser.
} // End of namespace Aki.

#endif // AKI_CHATPARSER_HPP
