/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

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
class UserItem
    : public QTreeWidgetItem
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
