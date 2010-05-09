#include "identity.hpp"
#include "private/identity_p.hpp"
#include <QtCore/QSharedPointer>
using namespace Aki;

Identity::Identity()
{
    _d.reset(new Aki::IdentityPrivate);
}

Identity::Identity(const QString& name)
{
    _d.reset(new Aki::IdentityPrivate);
    setName(name);
}

Identity::~Identity()
{
}

QString
Identity::awayMessage() const
{
    return _d->awayMessage;
}

QString
Identity::awayNickname() const
{
    return _d->awayNickname;
}

bool
Identity::isAwayMessagesEnabled() const
{
    return _d->isAwayMessagesEnabled;
}

bool
Identity::isMarkLastPositionEnabled() const
{
    return _d->isMarkLastPositionEnabled;
}

QString
Identity::kickMessage() const
{
    return _d->kickMessage;
}

QString
Identity::name() const
{
    return _d->name;
}

QString
Identity::nickname(int index) const
{
    return _d->nicknameList.value(index, QString());
}

QStringList
Identity::nicknameList() const
{
    return _d->nicknameList;
}

QString
Identity::partMessage() const
{
    return _d->partMessage;
}

QString
Identity::quitMessage() const
{
    return _d-> partMessage;
}

QString
Identity::realName() const
{
    return _d->realName;
}

QString
Identity::returnMessage() const
{
    return _d->returnMessage;
}

void
Identity::setAwayMessage(const QString& message)
{
    _d->awayMessage = message;
}

void
Identity::setAwayNickname(const QString& nick)
{
    _d->awayNickname = nick;
}

void
Identity::setKickMessage(const QString& message)
{
    _d->kickMessage = message;
}

void
Identity::setMarkLastPosition(bool enabled)
{
    _d->isMarkLastPositionEnabled = enabled;
}

void
Identity::setName(const QString& name)
{
    _d->name = name;
}

void
Identity::setNickname(int index, const QString& nickname)
{
    _d->nicknameList.replace(index, nickname);
}

void
Identity::setNicknameList(const QStringList& nicknames)
{
    _d->nicknameList = nicknames;
}

void
Identity::setPartMessage(const QString& message)
{
    _d->partMessage = message;
}

void
Identity::setQuitMessage(const QString& message)
{
    _d->quitMessage = message;
}

void
Identity::setRealName(const QString& name)
{
    _d->realName = name;
}

void
Identity::setReturnMessage(const QString& message)
{
    _d->returnMessage = message;
}

void
Identity::setUseAwayMessages(bool enabled)
{
    _d->isAwayMessagesEnabled = enabled;
}
