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

#include "userlistsort.hpp"
#include "usermodel.hpp"
#include "irc/user.hpp"
using namespace Aki;
using namespace Irc;

UserListSort::UserListSort(Aki::UserModel* model, QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setSortLocaleAware(true);
    setSortRole(Aki::UserModel::IrcUserRole);
    setSourceModel(model);
}

UserListSort::~UserListSort()
{
}

bool
UserListSort::compareNicks(const Aki::Irc::User* left, const Aki::Irc::User* right) const
{
    return QString::compare(left->nickInfo().nick(), right->nickInfo().nick(), sortCaseSensitivity()) < 0;
}

bool
UserListSort::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
    const Aki::Irc::User* leftData = sourceModel()->data(left, Aki::UserModel::IrcUserRole).value<Aki::Irc::User*>();
    const Aki::Irc::User* rightData = sourceModel()->data(right, Aki::UserModel::IrcUserRole).value<Aki::Irc::User*>();

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

Aki::UserModel*
UserListSort::model() const
{
    return static_cast<Aki::UserModel*>(sourceModel());
}
