/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
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

#include "userlist.hpp"
#include "userlistdelegate.hpp"
#include "userlistsort.hpp"
#include "usermodel.hpp"
#include "irc/user.hpp"
using namespace Aki;

UserList::UserList(QWidget* parent)
    : QListView(parent),
    _userModel(new Aki::UserModel(this))
{
    Aki::UserListSort* sort = new Aki::UserListSort(_userModel, this);

    setModel(sort);
    model()->sort(0);

    setItemDelegate(new Aki::UserListDelegate);
    setSelectionMode(QListView::SingleSelection);
    setUniformItemSizes(true);
    setUpdatesEnabled(true);
}

UserList::~UserList()
{
}

void
UserList::addUser(Aki::Irc::User* user)
{
    Q_ASSERT(user);
    _userModel->addUser(user);
}

int
UserList::count() const
{
    return _userModel->count();
}

void
UserList::removeUser(Aki::Irc::User* user)
{
    Q_ASSERT(user);
    if (users().contains(user)) {
        _userModel->removeUser(user);
    }
}

Aki::UserList::List
UserList::users()
{
    return _userModel->users();
}

Aki::UserList::List
UserList::users() const
{
    return _userModel->users();
}
