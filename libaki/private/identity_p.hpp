#ifndef AKI_IDENTITY_P_HPP
#define AKI_IDENTITY_P_HPP

#include <QtCore/QStringList>

namespace Aki
{
class Identity;
class IdentityPrivate
{
public:
    IdentityPrivate();
    QStringList nicknameList;
    QString awayMessage;
    QString awayNickname;
    QString name;
    QString kickMessage;
    QString partMessage;
    QString quitMessage;
    QString realName;
    QString returnMessage;
    bool isAwayMessagesEnabled;
    bool isMarkLastPositionEnabled;
}; // End of class IdentityPrivate.
} // End of namespace Aki.

#endif // AKI_IDENTITY_P_HPP
