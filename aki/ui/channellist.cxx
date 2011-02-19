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

#include "channellist.hpp"
#include "sql/channel.hpp"
#include "sql/database.hpp"
#include "sql/server.hpp"
#include "ui/channelmodel.hpp"
using namespace Aki;

ChannelList::ChannelList(QWidget* parent)
    : QListView(parent)
{
    _model = new Aki::ChannelModel(this);
    setModel(_model);

    connect(this, SIGNAL(activated(QModelIndex)),
            SLOT(slotItemActivated(QModelIndex)));
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(slotItemChanged(QModelIndex)));
    connect(this, SIGNAL(clicked(QModelIndex)),
            SLOT(slotItemClicked(QModelIndex)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)),
            SLOT(slotItemDoubleClicked(QModelIndex)));
    connect(this, SIGNAL(entered(QModelIndex)),
            SLOT(slotItemEntered(QModelIndex)));
    connect(this, SIGNAL(pressed(QModelIndex)),
            SLOT(slotItemPressed(QModelIndex)));
    connect(selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            SLOT(slotItemCurrentChanged(QModelIndex,QModelIndex)));
    connect(selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SIGNAL(channelSelectionChanged()));
}

ChannelList::~ChannelList()
{
}

void
ChannelList::addChannel(Aki::Sql::Channel* channel)
{
    insertChannel(count(), channel);
}

Aki::Sql::Channel*
ChannelList::channel(int row) const
{
    return channelFromIndex(_model->index(row));
}

Aki::Sql::Channel*
ChannelList::channelFromIndex(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return _model->channels().at(index.row());
}

int
ChannelList::count() const
{
    return _model->rowCount();
}

Aki::Sql::Channel*
ChannelList::currentChannel() const
{
    const QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return 0;
    }

    return channelFromIndex(index);
}

int
ChannelList::currentRow() const
{
    const QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return -1;
    }

    return index.row();
}

Aki::ChannelList::List
ChannelList::findChannels(const QString& name, Qt::MatchFlags flags) const
{
    Aki::ChannelList::List list;
    const QModelIndexList indexes = _model->match(_model->index(0), Qt::DisplayRole, name, -1, flags);
    foreach (const QModelIndex& index, indexes) {
        list.append(channelFromIndex(index));
    }

    return list;
}

QModelIndex
ChannelList::indexFromChannel(Aki::Sql::Channel* channel)
{
    return _model->index(_model->channels().indexOf(channel));
}

void
ChannelList::insertChannel(int row, Aki::Sql::Channel* channel)
{
    Q_ASSERT(channel);

    if (_model->channels().contains(channel)) {
        return;
    }

    _model->insertChannel(row, channel);
}

void
ChannelList::removeChannel(int row)
{
    delete takeChannel(row);
}

void
ChannelList::repopulateChannels(Aki::Sql::Server* server)
{
    for (int i = 0, c = count(); i < c; ++i) {
        removeChannel(i);
    }

    const Aki::ChannelList::List list = _database->find<Aki::Sql::Channel>()
        .where("channelServer = :channelServer").bind("channelServer", server->id())
        .result();

    foreach (Aki::Sql::Channel* channel, list) {
        addChannel(channel);
    }
}

int
ChannelList::row(Aki::Sql::Channel* channel)
{
    return _model->channels().indexOf(channel);
}

Aki::ChannelList::List
ChannelList::selectedAddresses() const
{
    Aki::ChannelList::List list;

    const QModelIndexList indexes = selectionModel()->selectedRows();
    foreach (const QModelIndex& index, indexes) {
        list.append(channelFromIndex(index));
    }

    return list;
}

void
ChannelList::setCurrentChannel(Aki::Sql::Channel* channel, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(indexFromChannel(channel), command);
}

void
ChannelList::setCurrentRow(int row, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(_model->index(row), command);
}

void
ChannelList::setDatabase(Aki::Sql::Database* database)
{
    Q_ASSERT(database);
    _database = database;
}

void
ChannelList::slotItemActivated(const QModelIndex& index)
{
    emit channelActivated(channelFromIndex(index));
}

void
ChannelList::slotItemChanged(const QModelIndex& index)
{
    emit channelChanged(channelFromIndex(index));
}

void
ChannelList::slotItemClicked(const QModelIndex& index)
{
    emit channelClicked(channelFromIndex(index));
}

void
ChannelList::slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    QPersistentModelIndex persistentCurrent = current;
    Aki::Sql::Channel* currentChannel = channel(persistentCurrent.row());
    emit currentChannelChanged(currentChannel, channel(previous.row()));
    emit channelCurrentRowChanged(persistentCurrent.row());
}

void
ChannelList::slotItemDoubleClicked(const QModelIndex& index)
{
    emit channelDoubleClicked(channelFromIndex(index));
}

void
ChannelList::slotItemEntered(const QModelIndex& index)
{
    emit channelEntered(channelFromIndex(index));
}

void
ChannelList::slotItemPressed(const QModelIndex& index)
{
    emit channelPressed(channelFromIndex(index));
}

Aki::Sql::Channel*
ChannelList::takeChannel(int row)
{
    return _model->takeChannel(row);
}
