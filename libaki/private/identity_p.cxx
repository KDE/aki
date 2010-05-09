#include "identity_p.hpp"
#include "identity.hpp"
using namespace Aki;

IdentityPrivate::IdentityPrivate()
    : awayMessage(QString()),
    awayNickname(QString()),
    name(QString()),
    kickMessage(QString()),
    partMessage(QString()),
    quitMessage(QString()),
    realName(QString()),
    returnMessage(QString()),
    isAwayMessagesEnabled(false),
    isMarkLastPositionEnabled(false)
{
    nicknameList.clear();
}
