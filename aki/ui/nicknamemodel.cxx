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

#include "nicknamemodel.hpp"
#include "sql/nickname.hpp"
using namespace Aki;

NicknameModel::NicknameModel(QObject* parent)
    : QAbstractListModel(parent)
{
    _nicknameList.clear();
}

NicknameModel::~NicknameModel()
{
    qDeleteAll(_nicknameList);
}

void
NicknameModel::addNickname(Aki::Sql::Nickname* nickname)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _nicknameList.append(nickname);
    endInsertRows();
}

QVariant
NicknameModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Aki::Sql::Nickname* nickname = _nicknameList.at(index.row());
    if (!nickname) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return nickname->nickname();
    }
    default: {
        break;
    }
    }

    return QVariant();
}

void
NicknameModel::insertNickname(int row, Aki::Sql::Nickname* nickname)
{
    if (!nickname) {
        return;
    }

    if (row > rowCount()) {
        addNickname(nickname);
        return;
    } else if (row < 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        _nicknameList.prepend(nickname);
        endInsertRows();
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    _nicknameList.insert(row, nickname);
    endInsertRows();
}

const QList<Aki::Sql::Nickname*>&
NicknameModel::nicknames() const
{
    return _nicknameList;
}

void
NicknameModel::removeNickname(Aki::Sql::Nickname* nickname)
{
    if (!nickname) {
        return;
    }

    const int row = _nicknameList.indexOf(nickname);
    beginRemoveRows(QModelIndex(), row, row);
    delete takeNickname(row);
    endRemoveRows();
}

int
NicknameModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return _nicknameList.count();
}

Aki::Sql::Nickname*
NicknameModel::takeNickname(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::Sql::Nickname* nickname = _nicknameList.takeAt(row);
    endRemoveRows();
    return nickname;
}
