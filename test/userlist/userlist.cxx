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

    Aki::Irc::User* user = new Aki::Irc::User(this);
    user->setNickInfo(Aki::Irc::NickInfo("chakie_work!~chakie@dsl-tkubrasgw1-fe00fa00-178.dhcp.inet.fi"));
    user->setModes("v");
    addUser(user);

    user = new Aki::Irc::User(this);
    user->setNickInfo(Aki::Irc::NickInfo("AkiBot!~testBot@2001:55c:62c6:e118:222:3fff:fef6:89bf"));
    user->setModes("h");
    addUser(user);

    user = new Aki::Irc::User(this);
    user->setNickInfo(Aki::Irc::NickInfo("oracal!~oracal@cpc2-lewi1-0-0-cust326.2-4.cable.virginmedia.com"));
    user->setModes("o");
    addUser(user);

    user = new Aki::Irc::User(this);
    user->setNickInfo(Aki::Irc::NickInfo("hunter!~hunter@dslb-088-064-050-135.pools.arcor-ip.net"));
    addUser(user);
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
