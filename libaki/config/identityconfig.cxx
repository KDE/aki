#include "identityconfig.hpp"
#include <KDE/KUser>
using namespace Aki;

IdentityConfig::IdentityConfig(QObject* parent)
    : ConfigFile(QLatin1String("akiidentityrc"), parent)
{
}

IdentityConfig::~IdentityConfig()
{
}

QString
IdentityConfig::awayMessage()
{
    return currentGroup().readEntry(QLatin1String("AwayMessage"),
                                    QString(QLatin1String("/me is away from the computer now.")));
}

QString
IdentityConfig::awayNickname()
{
    KUser user(KUser::UseRealUserID);
    return currentGroup().readEntry(QLatin1String("AwayNickname"),
                                    user.loginName() + QLatin1String("|away"));
}

bool
IdentityConfig::isAwayMessagesEnabled()
{
    return currentGroup().readEntry(QLatin1String("AwayMessagesEnabled"), false);
}

bool
IdentityConfig::isMarkLastPositionEnabled()
{
    return currentGroup().readEntry(QLatin1String("MarkLastPositionEnabled"), false);
}

QString
IdentityConfig::kickMessage()
{
    return currentGroup().readEntry(QLatin1String("KickMessage"),
                                    QString(QLatin1String("Bye bye!! :)")));
}

QString
IdentityConfig::name()
{
    return currentGroup(),name();
}

QStringList
IdentityConfig::nicknameList()
{
    KUser user(KUser::UseRealUserID);
    QString nickname(user.loginName() + QLatin1Char(',') + user.loginName() + QLatin1String("_,")
                     + user.loginName() + QLatin1String("__"));
    QString tmp = currentGroup().readEntry(QLatin1String("Nicknames"), nickname);

    return tmp.split(QLatin1Char(','), QString::SkipEmptyParts);
}

QString
IdentityConfig::partMessage()
{
    return currentGroup().readEntry(QLatin1String("PartMessage"),
                                    QString(QLatin1String("Aki IRC Client: %v")));
}

QString
IdentityConfig::quitMessage()
{
    return currentGroup().readEntry(QLatin1String("QuitMessage"),
                                    QString(QLatin1String("Aki IRC Client: %v")));
}

QString
IdentityConfig::realName()
{
    KUser user(KUser::UseRealUserID);
    QString name = user.property(KUser::FullName).toString();
    if (name.isEmpty() || name.isNull()) {
        name.clear();
    }

    return currentGroup().readEntry(QLatin1String("RealName"), name);
}

QString
IdentityConfig::returnMessage()
{
    return currentGroup().readEntry(QLatin1String("ReturnMessage"),
                                    QString(QLatin1String("/me is back!")));
}

void
IdentityConfig::setAwayMessage(const QString& message)
{
    currentGroup().writeEntry(QLatin1String("AwayMessage"), message);
}

void
IdentityConfig::setAwayNickname(const QString& nick)
{
    currentGroup().writeEntry(QLatin1String("AwayNickname"), nick);
}

void
IdentityConfig::setKickMessage(const QString& message)
{
    currentGroup().writeEntry(QLatin1String("KickMessage"), message);
}

void
IdentityConfig::setMarkLastPosition(bool enabled)
{
    currentGroup().writeEntry(QLatin1String("MarkLastPositionEnabled"), enabled);
}

void
IdentityConfig::setName(const QString& name)
{
    if(this->name() != name) {
        deleteGroup(this->name());
        createGroup(name);
    }
}

void
IdentityConfig::setNicknameList(const QStringList& nicknames)
{
    currentGroup().writeEntry(QLatin1String("Nicknames"), nicknames);
}

void
IdentityConfig::setPartMessage(const QString& message)
{
    currentGroup().writeEntry(QLatin1String("PartMessage"), message);
}

void
IdentityConfig::setQuitMessage(const QString& message)
{
    currentGroup().writeEntry(QLatin1String("QuitMessage"), message);
}

void
IdentityConfig::setRealName(const QString& name)
{
    currentGroup().writeEntry(QLatin1String("RealName"), name);
}

void
IdentityConfig::setReturnMessage(const QString& message)
{
    currentGroup().writeEntry(QLatin1String("ReturnMessage"), message);
}

void
IdentityConfig::setUseAwayMessages(bool enabled)
{
    currentGroup().writeEntry(QLatin1String("AwayMessagesEnabled"), enabled);
}
