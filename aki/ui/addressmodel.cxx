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

#include "addressmodel.hpp"
#include "aki.hpp"
#include "debughelper.hpp"
#include "utils/sqladdress.hpp"
#include <KDE/KIcon>
using namespace Aki;

AddressModel::AddressModel(QObject* parent)
    : QAbstractListModel(parent)
{
    _addressList.clear();
}

AddressModel::~AddressModel()
{
    qDeleteAll(_addressList);
}

void
AddressModel::addAddress(Aki::SqlAddress* address)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _addressList.append(address);
    endInsertRows();
}

QList<Aki::SqlAddress*>
AddressModel::addresses() const
{
    return _addressList;
}

QVariant
AddressModel::data(const QModelIndex& index, int role) const
{
    DEBUG_FUNC_NAME;
    if (!index.isValid()) {
        DEBUG_TEXT("Index is invalid so ignoring data call");
        return QVariant();
    }

    Aki::SqlAddress* address = _addressList.at(index.row());
    if (!address) {
        DEBUG_TEXT("Uh oh invalid address for row");
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return QString("%1:%2").arg(address->address(), QString::number(address->port()));
    }
    case Qt::DecorationRole: {
        if (address->isSslEnabled()) {
            return KIcon("document-encrypt");
        }
        return QVariant();
    }
    }

    return QVariant();
}

Qt::ItemFlags
AddressModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void
AddressModel::insertAddress(int row, Aki::SqlAddress* address)
{
    if (!address) {
        return;
    }

    if (row >= rowCount()) {
        addAddress(address);
        return;
    } else if (row <= 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        _addressList.prepend(address);
        endInsertRows();
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    _addressList.insert(row, address);
    endInsertRows();
}

void
AddressModel::removeAddress(Aki::SqlAddress* address)
{
    if (!address) {
        return;
    }

    const int row = _addressList.indexOf(address);
    beginRemoveRows(QModelIndex(), row, row);
    delete takeAddress(row);
    endRemoveRows();
}

int
AddressModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return _addressList.count();
}

Aki::SqlAddress*
AddressModel::takeAddress(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::SqlAddress* address = _addressList.takeAt(row);
    endRemoveRows();

    return address;
}
