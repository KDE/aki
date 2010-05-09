#ifndef AKI_IDENTITYCONFIG_HPP
#define AKI_IDENTITYCONFIG_HPP

#include "libaki_export.hpp"
#include "configfile.hpp"

namespace Aki
{
class LIBAKI_EXPORT IdentityConfig : public Aki::ConfigFile
{
    Q_OBJECT
public:
    explicit IdentityConfig(QObject* parent = 0);
    virtual ~IdentityConfig();
    QString awayMessage();
    QString awayNickname();
    QString kickMessage();
    bool isAwayMessagesEnabled();
    bool isMarkLastPositionEnabled();
    QStringList nicknameList();
    QString partMessage();
    QString quitMessage();
    QString realName();
    QString returnMessage();
    void setAwayMessage(const QString& message);
    void setAwayNickname(const QString& nick);
    void setKickMessage(const QString& message);
    void setMarkLastPosition(bool enabled);
    void setNicknameList(const QStringList& nicknames);
    void setPartMessage(const QString& message);
    void setQuitMessage(const QString& message);
    void setRealName(const QString& name);
    void setReturnMessage(const QString& message);
    void setUseAwayMessages(bool enabled);
    QString name();
    void setName(const QString& name);
}; // End of class IdentityConfig.
} // End of namespace Aki.

#endif // AKI_IDENTITYCONFIG_HPP
