#ifndef AKI_IDENTITY_HPP
#define AKI_IDENTITY_HPP

#include "libaki_export.hpp"
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>

namespace Aki
{
class IdentityPrivate;
class LIBAKI_EXPORT Identity
{
public:
    typedef QSharedPointer<Aki::Identity> Ptr;
    typedef QList<Aki::Identity::Ptr> List;
    Identity();
    explicit Identity(const QString& name);
    ~Identity();
    QString awayMessage() const;
    QString awayNickname() const;
    QString name() const;
    QString kickMessage() const;
    bool isAwayMessagesEnabled() const;
    bool isMarkLastPositionEnabled() const;
    QString nickname(int index) const;
    QStringList nicknameList() const;
    QString partMessage() const;
    QString quitMessage() const;
    QString realName() const;
    QString returnMessage() const;
    void setAwayMessage(const QString& message);
    void setAwayNickname(const QString& nick);
    void setName(const QString& name);
    void setKickMessage(const QString& message);
    void setMarkLastPosition(bool enabled);
    void setNickname(int index, const QString& nickname);
    void setNicknameList(const QStringList& nicknames);
    void setPartMessage(const QString& message);
    void setQuitMessage(const QString& message);
    void setRealName(const QString& name);
    void setReturnMessage(const QString& message);
    void setUseAwayMessages(bool enabled);
private:
    friend class IdentityPrivate;
    QScopedPointer<IdentityPrivate> _d;
}; // End of class Identity.
} // End of namespace Aki.

Q_DECLARE_METATYPE(Aki::Identity*)

#endif // AKI_IDENTITY_HPP
