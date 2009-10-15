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

#include "nicksort.h"
#include "nicklistmodel.h"
#include <Aki/Irc/User>
using namespace Aki;

NickSort::NickSort(Aki::NickListModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setSourceModel(model);
    setDynamicSortFilter(true);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setSortLocaleAware(true);
    setSortRole(Aki::NickListModel::IrcUserRole);
}

bool
NickSort::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    Aki::Irc::User *leftData = sourceModel()->data(left, Aki::NickListModel::IrcUserRole).value<Aki::Irc::User*>();
    Aki::Irc::User *rightData = sourceModel()->data(right, Aki::NickListModel::IrcUserRole).value<Aki::Irc::User*>();

    if (leftData->isOp() && rightData->isOp()) {
        return compareNicks(leftData, rightData);
    } else if (leftData->isOp() && (rightData->isHalfOp() || rightData->isVoice() || rightData->isNormal())) {
        return true;
    } else if (leftData->isHalfOp() && rightData->isOp()) {
        return false;
    } else if (leftData->isHalfOp() && rightData->isHalfOp()) {
        return compareNicks(leftData, rightData);
    } else if (leftData->isHalfOp() && (rightData->isVoice() || rightData->isNormal())) {
        return true;
    } else if (leftData->isVoice() && (rightData->isOp() || rightData->isHalfOp())) {
        return false;
    } else if (leftData->isVoice() && rightData->isVoice()) {
        return compareNicks(leftData, rightData);
    } else if (leftData->isVoice() && rightData->isNormal()) {
        return true;
    } else if (leftData->isNormal() && (rightData->isOp() || rightData->isHalfOp() || rightData->isVoice())) {
        return false;
    } else if (leftData->isNormal() && rightData->isNormal()) {
        return compareNicks(leftData, rightData);
    }

    return true;
}

Aki::NickListModel*
NickSort::model() const
{
    return static_cast<Aki::NickListModel*>(sourceModel());
}

bool
NickSort::compareNicks(const Aki::Irc::User *left, const Aki::Irc::User *right) const
{
    return QString::compare(left->nick(), right->nick(), sortCaseSensitivity()) < 0;
}
