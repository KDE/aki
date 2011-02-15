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

#include "channelmodel.hpp"
#include "sql/channel.hpp"
#include <KDE/KIcon>
using namespace Aki;

ChannelModel::ChannelModel(QObject* parent)
    : QAbstractListModel(parent)
{
    _channelList.clear();
}

ChannelModel::~ChannelModel()
{
    qDeleteAll(_channelList);
}

void
ChannelModel::addChannel(Aki::Sql::Channel* channel)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _channelList.append(channel);
    endInsertRows();
}

const QList<Aki::Sql::Channel*>&
ChannelModel::channels() const
{
    return _channelList;
}

QVariant
ChannelModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const Aki::Sql::Channel* channel = _channelList.at(index.row());
    if (!channel) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return channel->name();
    }
    case Qt::DecorationRole: {
        if (!channel->password().isEmpty()) {
            return KIcon("document-encrypt");
        } else {
            return QVariant();
        }
    }
    default: {
        return QVariant();
    }
    }

    return QVariant();
}

Qt::ItemFlags
ChannelModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void
ChannelModel::insertChannel(int row, Aki::Sql::Channel* channel)
{
    if (!channel) {
        return;
    }

    if (row >= rowCount()) {
        addChannel(channel);
        return;
    } else if (row <= 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        _channelList.prepend(channel);
        endInsertRows();
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    _channelList.insert(row, channel);
    endInsertRows();
}

int
ChannelModel::rowCount(const QModelIndex&) const
{
    return _channelList.count();
}

Aki::Sql::Channel*
ChannelModel::takeChannel(int row)
{
    return _channelList.takeAt(row);
}
