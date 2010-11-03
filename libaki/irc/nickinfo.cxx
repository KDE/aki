#include "nickinfo.hpp"
#include "debughelper.hpp"
#include "private/nickinfo_p.hpp"
using namespace Aki;
using namespace Irc;

NickInfo::NickInfo()
{
    _d.reset(new Aki::Irc::NickInfoPrivate(this));
}

NickInfo::NickInfo(const QString& hostmask)
{
    _d.reset(new Aki::Irc::NickInfoPrivate(this));
    setHostmask(hostmask);
}

NickInfo::NickInfo(const Aki::Irc::NickInfo& nickInfo)
{
    _d.reset(new Aki::Irc::NickInfoPrivate(this));
    setHostmask(nickInfo.hostmask());
}

NickInfo::~NickInfo()
{
}

Aki::Irc::NickInfo&
NickInfo::operator=(const Aki::Irc::NickInfo & rhs)
{
    _d.reset(new Aki::Irc::NickInfoPrivate(this));
    setHostmask(rhs.hostmask());

    return *this;
}

QString
NickInfo::host() const
{
    return _d->host();
}

QString
NickInfo::hostmask() const
{
    return _d->hostmask;
}

QString
NickInfo::nick() const
{
    return _d->nick();
}

void
NickInfo::setHostmask(const QString& hostmask)
{
    _d->hostmask = hostmask;
}

QString
NickInfo::user() const
{
    return _d->user();
}
