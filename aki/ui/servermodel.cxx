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

#include "servermodel.hpp"
#include "sql/server.hpp"
using namespace Aki;

ServerModel::ServerModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

ServerModel::~ServerModel()
{
    qDeleteAll(_serverList);
}

void
ServerModel::addServer(Aki::Sql::Server* server)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _serverList.append(server);
    endInsertRows();
}

QVariant
ServerModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Aki::Sql::Server* server = _serverList.at(index.row());
    if (!server) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return server->name();
        break;
    }
    default: {
        return QVariant();
    }
    }

    return QVariant();
}

Qt::ItemFlags
ServerModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void
ServerModel::insertServer(int row, Aki::Sql::Server* server)
{
    if (!server) {
        return;
    }

    if (row >= rowCount()) {
        addServer(server);
        return;
    } else {
        beginInsertRows(QModelIndex(), 0, 0);
        _serverList.prepend(server);
        endInsertRows();
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    _serverList.insert(row, server);
    endInsertRows();
}

void
ServerModel::removeServer(Aki::Sql::Server* server)
{
    if (!server) {
        return;
    }

    const int row = _serverList.indexOf(server);
    beginRemoveRows(QModelIndex(), row, row);
    delete takeServer(row);
    endRemoveRows();
}

int
ServerModel::rowCount(const QModelIndex&) const
{
    return _serverList.count();
}

const QList<Aki::Sql::Server*>&
ServerModel::servers() const
{
    return _serverList;
}

Aki::Sql::Server*
ServerModel::takeServer(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::Sql::Server* server = _serverList.takeAt(row);
    endRemoveRows();

    return server;
}
