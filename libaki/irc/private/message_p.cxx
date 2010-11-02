#include "message_p.hpp"
#include "irc/message.hpp"
#include <QtCore/QTextCodec>
using namespace Aki;
using namespace Irc;

MessagePrivate::MessagePrivate(Aki::Irc::Message* qq)
    : sender(Aki::Irc::NickInfo()),
    command(QString()),
    message(QString()),
    params(QString()),
    numeric(Aki::Irc::RPL_NULL),
    isNumeric(false),
    _q(qq)
{
}

QString
MessagePrivate::removeStringToFirstWhitespace(QString* line)
{
    QString tmp = line->left(line->indexOf(' '));
    line->remove(0, tmp.length() + 1);
    return tmp;
}

QString
MessagePrivate::removeStringToFirstWhitespace(QString* line, int start, int stop)
{
    QString tmp = line->mid(start, line->indexOf(' ') - stop);
    line->remove(0, tmp.length() + 1 + stop);
    return tmp;
}
