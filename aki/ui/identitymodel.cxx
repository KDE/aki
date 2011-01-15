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

#include "identitymodel.hpp"
#include "aki.hpp"
#include "debughelper.hpp"
#include "utils/sqlidentity.hpp"
using namespace Aki;

IdentityModel::IdentityModel(QObject* parent)
    : QAbstractListModel(parent)
{
    _identityList.clear();
}

IdentityModel::~IdentityModel()
{
    qDeleteAll(_identityList);
}

void
IdentityModel::addIdentity(Aki::SqlIdentity* identity)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _identityList.append(identity);
    endInsertRows();
}

QVariant
IdentityModel::data(const QModelIndex& index, int role) const
{
    DEBUG_FUNC_NAME;
    if (!index.isValid()) {
        DEBUG_TEXT("Index is invalid so ignoring data call");
        return QVariant();
    }

    Aki::SqlIdentity* identity = _identityList.value(index.row());
    if (!identity) {
        DEBUG_TEXT("Uh oh invalid identity at row");
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return identity->name();
    }
    default: {
        break;
    }
    }

    return QVariant();
}

QList<Aki::SqlIdentity*>
IdentityModel::identities() const
{
    return _identityList;
}

void
IdentityModel::insertIdentity(int row, Aki::SqlIdentity* identity)
{
    if (!identity) {
        return;
    }

    if (row > rowCount()) {
        addIdentity(identity);
        return;
    } else if (row < 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        _identityList.prepend(identity);
        endInsertRows();
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    _identityList.insert(row, identity);
    endInsertRows();
}

void
IdentityModel::removeIdentity(Aki::SqlIdentity* identity)
{
    if (!identity) {
        return;
    }

    const int row = _identityList.indexOf(identity);
    delete takeIdentity(row);
}

int
IdentityModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return _identityList.count();
}

Aki::SqlIdentity*
IdentityModel::takeIdentity(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::SqlIdentity* identity = _identityList.takeAt(row);
    endInsertRows();

    return identity;
}
