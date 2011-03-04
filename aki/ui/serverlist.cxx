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

#include "serverlist.hpp"
#include "sql/database.hpp"
#include "sql/identity.hpp"
#include "sql/server.hpp"
#include "ui/servermodel.hpp"
using namespace Aki;

ServerList::ServerList(QWidget* parent)
    : QListView(parent)
{
    _model = new Aki::ServerModel(this);
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
            SIGNAL(serverSelectionChanged()));
}

ServerList::~ServerList()
{
}

void
ServerList::addServer(Aki::Sql::Server* server)
{
    insertServer(count(), server);
}

int
ServerList::count() const
{
    return model()->rowCount();
}

int
ServerList::currentRow() const
{
    const QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return -1;
    }

    return index.row();
}

Aki::Sql::Server*
ServerList::currentServer() const
{
    const QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return 0;
    }

    return serverFromIndex(index);
}

Aki::ServerList::List
ServerList::findItems(const QString& name, Qt::MatchFlags flags) const
{
    Aki::ServerList::List list;

    const QModelIndexList indexes = model()->match(_model->index(0), Qt::DisplayRole, name, -1, flags);
    foreach (const QModelIndex& index, indexes) {
        list.append(serverFromIndex(index));
    }

    return list;
}

QModelIndex
ServerList::indexFromServer(Aki::Sql::Server* server)
{
    return _model->index(_model->servers().indexOf(server));
}

void
ServerList::insertServer(int row, Aki::Sql::Server* server)
{
    Q_ASSERT(server);

    if (_model->servers().contains(server)) {
        return;
    }

    _model->insertServer(row, server);
}

void
ServerList::removeServer(int row)
{
    qDebug() << _database->remove(server(row));
    delete takeServer(row);
}

void
ServerList::repopulateServers(Aki::Sql::Identity* identity)
{
    Q_ASSERT(identity);

    foreach (Aki::Sql::Server* server, _model->servers()) {
        removeServer(row(server));
    }

    const Aki::ServerList::List list = _database->find<Aki::Sql::Server>()
        .where("serverIdentity = :serverIdentity").bind("serverIdentity", identity->id())
        .result();

    foreach (Aki::Sql::Server* server, list) {
        addServer(server);
    }
}

int
ServerList::row(Aki::Sql::Server* server) const
{
    return _model->servers().indexOf(server);
}

Aki::ServerList::List
ServerList::selectedServers() const
{
    Aki::ServerList::List list;

    const QModelIndexList indexes = selectionModel()->selectedRows();
    foreach (const QModelIndex& index, indexes) {
        list.append(serverFromIndex(index));
    }

    return list;
}

Aki::Sql::Server*
ServerList::server(int row) const
{
    return serverFromIndex(_model->index(row));
}

Aki::Sql::Server*
ServerList::serverFromIndex(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return _model->servers().at(index.row());
}

void
ServerList::setCurrentRow(int row)
{
    const QModelIndex index = _model->index(row);
    if (selectionMode() == SingleSelection) {
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    } else if (selectionMode() == NoSelection) {
        selectionModel()->setCurrentIndex(index,QItemSelectionModel::NoUpdate);
    } else {
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
    }
}

void
ServerList::setCurrentServer(Aki::Sql::Server* server)
{
    setCurrentRow(row(server));
}

void
ServerList::setDatabase(Aki::Sql::Database* database)
{
    Q_ASSERT(database);
    _database = database;
}

void
ServerList::slotItemActivated(const QModelIndex& index)
{
    emit serverActivated(serverFromIndex(index));
}

void
ServerList::slotItemChanged(const QModelIndex& index)
{
    emit serverChanged(serverFromIndex(index));
}

void
ServerList::slotItemClicked(const QModelIndex& index)
{
    emit serverClicked(serverFromIndex(index));
}

void
ServerList::slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    QPersistentModelIndex persistentCurrent = current;
    Aki::Sql::Server* currentServer = server(persistentCurrent.row());
    emit currentServerChanged(currentServer, server(previous.row()));
    emit serverCurrentRowChanged(persistentCurrent.row());
}

void
ServerList::slotItemDoubleClicked(const QModelIndex& index)
{
    emit serverDoubleClicked(serverFromIndex(index));
}

void
ServerList::slotItemEntered(const QModelIndex& index)
{
    emit serverEntered(serverFromIndex(index));
}

void
ServerList::slotItemPressed(const QModelIndex& index)
{
    emit serverPressed(serverFromIndex(index));
}

Aki::Sql::Server*
ServerList::takeServer(int row)
{
    return _model->takeServer(row);
}
