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

#include "nicklistdelegate.h"
#include "nicklistmodel.h"
using namespace Aki;

namespace Aki
{
class NickListDelegatePrivate
{
public:
    NickListDelegatePrivate()
        : awayColor(Qt::gray),
        onlineColor(Qt::black)
    {
    }

    QColor awayColor;
    QColor onlineColor;
}; // End of class NickListDelegatePrivate.
} // End of namespace Aki.

NickListDelegate::NickListDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    d.reset(new Aki::NickListDelegatePrivate);
}

NickListDelegate::~NickListDelegate()
{
}

QColor
NickListDelegate::awayColor() const
{
    return d->awayColor;
}

QColor
NickListDelegate::onlineColor() const
{
    return d->onlineColor;
}

void
NickListDelegate::setAwayColor(const QColor &color)
{
    d->awayColor = color;
}

void
NickListDelegate::setOnlineColor(const QColor &color)
{
    d->onlineColor = color;
}

void
NickListDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);

    if (!index.isValid()) {
        return;
    }

    if (!index.data(NickListModel::IrcUserAwayRole).toBool()) {
        option->palette.setColor(QPalette::Text, onlineColor());
    } else {
        option->palette.setColor(QPalette::Text, awayColor());
    }
}
