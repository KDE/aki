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

#ifndef NICKSORT_H
#define NICKSORT_H

#include "libaki_export.h"
#include <QSortFilterProxyModel>

namespace Aki
{
namespace Irc
{
class User;
} // End of namespace Irc.
class NickListModel;
class LIBAKI_EXPORT NickSort : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit NickSort(Aki::NickListModel *model, QObject *parent = 0);
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    NickListModel* model() const;
    bool compareNicks(const Aki::Irc::User *left, const Aki::Irc::User *right) const;
}; // End of class NickSort.
} // End of namespace Aki.

#endif // NICKSORT_H
