#ifndef AKI_USERITEM_HPP
#define AKI_USERITEM_HPP

#include "aki.hpp"
#include <QtGui/QTreeWidgetItem>

namespace Aki
{
namespace Irc
{
class User;
}

class UserItemPrivate;
class UserItem : public QTreeWidgetItem
{
public:
    enum Roles {
        AwayRole = Qt::UserRole,
        NickRole,
        ModeRole
    }; // End of enum Roles.

    enum ItemType {
        CategoryType = QTreeWidgetItem::UserType,
        UserType
    }; // End of enum ItemType.

    explicit UserItem(QTreeWidget* parent);
    explicit UserItem(Aki::Irc::User* user, QTreeWidgetItem* parent = 0);
    virtual ~UserItem();
    virtual QVariant data(int column, int role) const;
    virtual bool operator<(const QTreeWidgetItem& other) const;
    virtual void setData(int column, int role, const QVariant& value);
    void setUser(Aki::Irc::User* user);
    Aki::Irc::User* user() const;
private:
    //AKI_DECLARE_PRIVATE(UserItem);
}; // End of class UserItem.
} // End of namespace Aki.

#endif // AKI_USERITEM_HPP
