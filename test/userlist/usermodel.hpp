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

#ifndef AKI_USERMODEL_HPP
#define AKI_USERMODEL_HPP

#include <QtCore/QAbstractListModel>

namespace Aki
{
namespace Irc
{
class User;
} // End of namespace Irc.
class UserModel
    : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NickRole {
        IrcUserRole = Qt::UserRole,
        IrcUserAwayRole
    }; // End of class NickRole.
    typedef QList<Aki::Irc::User*> List;

    UserModel(QObject* parent = 0);
    ~UserModel();
    void addUser(Aki::Irc::User* user);
    int count() const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    void removeUser(Aki::Irc::User* user);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    QString toolTip(Aki::Irc::User* user) const;
    Aki::UserModel::List users();
    Aki::UserModel::List users() const;
private:
    Aki::UserModel::List _userList;
}; // End of class UserModel.
} // End of namespace Aki.

Q_DECLARE_METATYPE(Aki::UserModel*)

#endif // AKI_USERMODEL_HPP
