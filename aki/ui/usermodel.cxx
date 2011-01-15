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

#include "usermodel.hpp"
#include "irc/user.hpp"
#include <KDE/KIcon>
using namespace Aki;

UserModel::UserModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

UserModel::~UserModel()
{
    qDeleteAll(_userList);
    _userList.clear();
}

void
UserModel::addUser(Aki::Irc::User* user)
{
    if (_userList.contains(user)) {
        return;
    }

    int row = (_userList.count() == 0) ? 0 : _userList.count();

    beginInsertRows(QModelIndex(), row, row);
    _userList.insert(row, user);
    endInsertRows();
}

int
UserModel::count() const
{
    return _userList.count();
}

QVariant
UserModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() > count()) {
        return QVariant();
    }

    switch (role) {
    case Qt::ToolTipRole: {
        return toolTip(_userList.at(index.row()));
    }
    case Qt::DisplayRole: {
        return _userList.at(index.row())->nickInfo().nick();
    }
    case Qt::DecorationRole: {
        Aki::Irc::User* user = _userList.at(index.row());
        if (user->isAway()) {
            return KIcon("aki_away");
        } else if (user->isOp()) {
            return KIcon("aki_operator");
        } else if (user->isHalfOp()) {
            return KIcon("aki_halfop");
        } else if (user->isVoice()) {
            return KIcon("aki_voice");
        } else {
            return KIcon("aki_normal");
        }
    }
    case Aki::UserModel::IrcUserAwayRole: {
        return _userList.at(index.row())->isAway();
    }
    case Aki::UserModel::IrcUserRole: {
        return QVariant::fromValue(_userList.at(index.row()));
    }
    default: {
        return QVariant();
    }
    }

    return QVariant();
}

Qt::ItemFlags
UserModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void
UserModel::removeUser(Aki::Irc::User* user)
{
    const int row = _userList.indexOf(user);
    if (row == -1) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    _userList.removeAt(row);
    endRemoveRows();
}

int
UserModel::rowCount(const QModelIndex&) const
{
    return _userList.count();
}

bool
UserModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    if (role != Aki::UserModel::IrcUserRole) {
        return false;
    }

    _userList[index.row()] = value.value<Aki::Irc::User*>();
    emit dataChanged(index, index);
    return true;
}

QString
UserModel::toolTip(Aki::Irc::User*) const
{
    return QString();
}

Aki::UserModel::List
UserModel::users()
{
    return _userList;
}

Aki::UserModel::List
UserModel::users() const
{
    return _userList;
}
