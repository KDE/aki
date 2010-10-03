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

#include "channelmodel.hpp"
#include "aki.hpp"
#include "utils/sqlchannel.hpp"
#include <KDE/KIcon>
using namespace Aki;

ChannelModel::ChannelModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

ChannelModel::~ChannelModel()
{
}

void
ChannelModel::addChannel(Aki::SqlChannel* channel)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _channelList.append(channel);
    endInsertRows();
}

QList<Aki::SqlChannel*>
ChannelModel::channels() const
{
    return _channelList;
}

QVariant
ChannelModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        qxtLog->warning() << "Index is invalid so ignoring data call";
        return QVariant();
    }

    Aki::SqlChannel* channel = _channelList.at(index.row());
    if (!channel) {
        qxtLog->warning() << "Uh oh invalid server for row";
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return channel->channel();
    }
    default: {
        break;
    }
    }

    return QVariant();
}

void
ChannelModel::insertChannel(int row, Aki::SqlChannel* channel)
{
    if (!channel) {
        return;
    }

    if (row > rowCount()) {
        addChannel(channel);
        return;
    } else if (row < 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        _channelList.prepend(channel);
        endInsertRows();
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    _channelList.insert(row, channel);
    endInsertRows();
}

void
ChannelModel::removeChannel(Aki::SqlChannel* channel)
{
    if (!channel) {
        return;
    }

    const int row = _channelList.indexOf(channel);
    beginRemoveRows(QModelIndex(), row, row);
    delete takeChannel(row);
    endRemoveRows();
}

int
ChannelModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return _channelList.count();
}

Aki::SqlChannel*
ChannelModel::takeChannel(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::SqlChannel* channel = _channelList.takeAt(row);
    endRemoveRows();

    return channel;
}
