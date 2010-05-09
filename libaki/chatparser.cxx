#include "chatparser.hpp"
#include "private/chatparser_p.hpp"
using namespace Aki;

ChatParser::ChatParser(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::ChatParserPrivate(this));
}

ChatParser::~ChatParser()
{
}

void
ChatParser::parse(const QString& data)
{
    Q_UNUSED(data);
}

void
ChatParser::addHelp(const boost::function<bool (const QString&)>& help)
{
    _d->helpHash.append(help);
}

void
ChatParser::addCommand(const boost::function<bool (const QString&)>& command)
{
    _d->commandHash.append(command);
}
