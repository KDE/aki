#ifndef AKI_SQLIDENTITY_HPP
#define AKI_SQLIDENTITY_HPP

#include "libaki_export.hpp"
#include <QtCore/QObject>
#include <QtCore/QStringList>

namespace Aki
{
class SqlIdentityPrivate;
class LIBAKI_EXPORT SqlIdentity : public QObject
{
    Q_OBJECT
public:
    virtual ~SqlIdentity();
    bool operator==(const Aki::SqlIdentity& rhs) const;
    bool operator!=(const Aki::SqlIdentity& rhs) const;
    static QStringList identityNames();
    static Aki::SqlIdentity* newIdentity(const QString& name);
    static Aki::SqlIdentity* findIdentity(const QString& name);
    int id() const;
    void setAwayMessage(const QString& message);
    QString awayMessage() const;
    void setAwayNickname(const QString& nickname);
    QString awayNickname() const;
    void setName(const QString& name);
    QString name() const;
    void setKickMessage(const QString& message);
    QString kickMessage() const;
    void setEnableMessages(bool enabled);
    bool isMessagesEnabled() const;
    void setMarkLastPosition(bool enabled);
    bool isMarkLastPositionEnabled() const;
    void setPartMessage(const QString& message);
    QString partMessage() const;
    void setQuitMessage(const QString& message);
    QString quitMessage() const;
    void setRealName(const QString& name);
    QString realName() const;
    void setReturnMessage(const QString& message);
    QString returnMessage() const;
    bool save();
    bool remove();
protected:
    explicit SqlIdentity(QObject* parent = 0);
    void setId(int id);
private:
    friend class SqlIdentityPrivate;
    QScopedPointer<SqlIdentityPrivate> _d;
}; // End of class SqlIdentity.
} // End of namespace Aki.

#endif // AKI_SQLIDENTITY_HPP
