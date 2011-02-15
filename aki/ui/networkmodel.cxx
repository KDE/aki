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

#include "networkmodel.hpp"
#include "aki.hpp"
#include "debughelper.hpp"
#include "utils/sqlnetwork.hpp"
using namespace Aki;

NetworkModel::NetworkModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

NetworkModel::~NetworkModel()
{
    qDeleteAll(_networkList);
}

void
NetworkModel::addNetwork(Aki::SqlNetwork* network)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _networkList.append(network);
    endInsertRows();
}

QVariant
NetworkModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const Aki::SqlNetwork* server = _networkList.at(index.row());
    if (!server) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return server->name();
    }
    }

    return QVariant();
}

void
NetworkModel::insertNetwork(int row, SqlNetwork* network)
{
    if (!network) {
        return;
    }

    if (row > rowCount()) {
        addNetwork(network);
        return;
    } else if (row < 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        _networkList.prepend(network);
        endInsertRows();
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    _networkList.insert(row, network);
    endInsertRows();
}

QList<Aki::SqlNetwork*>
NetworkModel::networks() const
{
    return _networkList;
}

void
NetworkModel::removeNetwork(Aki::SqlNetwork* network)
{
    if (!network) {
        return;
    }

    const int row = _networkList.indexOf(network);
    beginRemoveRows(QModelIndex(), row, row);
    delete takeNetwork(row);
    endRemoveRows();
}

int
NetworkModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return _networkList.count();
}

Aki::SqlNetwork*
NetworkModel::takeNetwork(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::SqlNetwork* network = _networkList.takeAt(row);
    endRemoveRows();

    return network;
}
