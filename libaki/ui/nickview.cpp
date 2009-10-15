/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "nickview.h"
#include "nicklistdelegate.h"
#include "nicklistmodel.h"
#include "nicksort.h"
#include <Aki/Irc/User>
using namespace Aki;

namespace Aki
{
class NickViewPrivate
{
public:
    NickViewPrivate()
        : model(0)
    {
    }
    NickListModel *model;
}; // End of class NickViewPrivate.
} // End of namespace Aki.

NickView::NickView(QWidget *parent)
    : QListView(parent),
    d(new NickViewPrivate)
{
    d->model = new Aki::NickListModel(this);
    Aki::NickSort *sort = new Aki::NickSort(d->model, this);

    setModel(sort);
    model()->sort(0);
    setItemDelegate(new NickListDelegate);
    setSelectionMode(ContiguousSelection);
    setUniformItemSizes(true);
    setUpdatesEnabled(true);
}

NickView::~NickView()
{
    delete d;
}

NickView::NickList
NickView::users() const
{
    return d->model->users();
}

NickView::NickList
NickView::users()
{
    return d->model->users();
}

void
NickView::addUser(Aki::Irc::User *user)
{
    Q_ASSERT(user);

    d->model->addUser(user);
}

int
NickView::count() const
{
    return d->model->count();
}

void
NickView::removeUser(Aki::Irc::User *user)
{
    Q_ASSERT(user);

    if (users().contains(user)) {
        d->model->removeUser(user);
    }
}
