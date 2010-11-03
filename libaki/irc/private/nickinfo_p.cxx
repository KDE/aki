#include "nickinfo_p.hpp"
#include "irc/nickinfo.hpp"
using namespace Aki::Irc;

NickInfoPrivate::NickInfoPrivate(Aki::Irc::NickInfo* qq)
    : hostmask(QString()),
    _q(qq)
{
}

QString
NickInfoPrivate::host() const
{
    QString tmp = hostmask.section('!', 1);
    return tmp.section('@', 1);
}

QString
NickInfoPrivate::nick() const
{
    return hostmask.section('!', 0, 0);
}

QString
NickInfoPrivate::user() const
{
    QString userHost = hostmask.section('!', 1);
    if (!userHost.isEmpty()) {
        return userHost.section('@', 0, 0);
    }

    return QString();
}
