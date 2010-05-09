#include "chatparser_p.hpp"
#include "chatparser.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
using namespace Aki;
using namespace boost;

ChatParserPrivate::ChatParserPrivate(ChatParser* qq)
    : QObject(),
    _q(qq)
{
}

void
ChatParserPrivate::setupFunctions()
{
}

bool
ChatParserPrivate::help(const QString& command)
{
    foreach (const ChatParserPrivate::HelpHook& hook, helpHash) {
        if (hook(command)) {
            return true;
        }
    }
    return false;
}

bool
ChatParserPrivate::command(const QString& command)
{
    foreach (const ChatParserPrivate::CommandHook& hook, commandHash) {
        if (hook(command)) {
            return true;
        }
    }
    return false;
}

#ifdef BOOST_NO_EXCEPTIONS

void
boost::throw_exception(const std::exception&)
{
}

#endif
