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

#include "networklist.hpp"
#include "aki.hpp"
#include "debughelper.hpp"
#include "ui/networkmodel.hpp"
#include "utils/sqlidentity.hpp"
#include "utils/sqlnetwork.hpp"
#include <QtCore/QEvent>
using namespace Aki;

NetworkList::NetworkList(QWidget* parent)
    : QListView(parent)
{
    _model = new Aki::NetworkModel(this);
    setModel(_model);
    connect(this, SIGNAL(pressed(QModelIndex)),
            SLOT(slotItemPressed(QModelIndex)));
    connect(this, SIGNAL(clicked(QModelIndex)),
            SLOT(slotItemClicked(QModelIndex)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)),
            SLOT(slotItemDoubleClicked(QModelIndex)));
    connect(this, SIGNAL(activated(QModelIndex)),
            SLOT(slotItemActivated(QModelIndex)));
    connect(this, SIGNAL(entered(QModelIndex)),
            SLOT(slotItemEntered(QModelIndex)));
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(slotItemChanged(QModelIndex)));
    connect(selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            SLOT(slotItemCurrentChanged(QModelIndex,QModelIndex)));
    connect(selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SIGNAL(networkSelectionChanged()));
//     connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
//             SIGNAL(dataChanged(QModelIndex,QModelIndex)));
}

NetworkList::~NetworkList()
{
}

void
NetworkList::addNetwork(Aki::SqlNetwork* network)
{
    insertNetwork(count(), network);
}

int
NetworkList::count() const
{
    return _model->rowCount();
}

Aki::SqlNetwork*
NetworkList::currentNetwork()
{
    QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return 0;
    }

    return networkFromIndex(index);
}

int
NetworkList::currentRow() const
{
    QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return -1;
    }

    return index.row();
}

Aki::NetworkList::List
NetworkList::findNetworks(const QString& name, Qt::MatchFlags flags) const
{
    Aki::NetworkList::List list;

    QModelIndexList indexes = _model->match(_model->index(0), Qt::DisplayRole, name, -1, flags);
    for (int i = 0; i < indexes.size(); ++i) {
        list.append(networkFromIndex(indexes.at(i)));
    }

    return list;
}

QModelIndex
NetworkList::indexFromNetwork(Aki::SqlNetwork* network)
{
    return _model->index(_model->networks().indexOf(network));
}

void
NetworkList::insertNetwork(int row, Aki::SqlNetwork* network)
{
    Q_ASSERT(network);

    if (_model->networks().contains(network)) {
        return;
    }

    _model->insertNetwork(row, network);
}

Aki::SqlNetwork*
NetworkList::item(int row) const
{
    return networkFromIndex(_model->index(row));
}

Aki::SqlNetwork*
NetworkList::networkFromIndex(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return _model->networks().value(index.row());
}

void
NetworkList::repopulateNetwork(Aki::SqlIdentity* identity)
{
    DEBUG_FUNC_NAME;

    foreach(Aki::SqlNetwork* network, _model->networks()) {
        DEBUG_TEXT2("Removing Network: %1", network->name());
        _model->removeNetwork(network);
    }

    Aki::SqlNetwork::List list = Aki::SqlNetwork::networksForIdentity(identity);
    if (list.isEmpty()) {
        DEBUG_TEXT2("List is empty for Identity: %1", identity->name());
        return;
    }

    foreach (Aki::SqlNetwork* network, list) {
        DEBUG_TEXT2("Adding new Network: %1", network->name());
        addNetwork(network);
    }

    if (count()) {
        setCurrentRow(0);
    }
}

int
NetworkList::row(Aki::SqlNetwork* network) const
{
    return _model->networks().indexOf(network);
}

Aki::NetworkList::List
NetworkList::selectedNetworks() const
{
    Aki::NetworkList::List list;
    QModelIndexList indexes = selectionModel()->selectedRows();
    foreach (const QModelIndex& index, indexes) {
        list.append(networkFromIndex(index));
    }

    return list;
}

void
NetworkList::setCurrentNetwork(Aki::SqlNetwork* network)
{
    setCurrentNetwork(network, QItemSelectionModel::ClearAndSelect);
}

void
NetworkList::setCurrentNetwork(Aki::SqlNetwork* network, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(indexFromNetwork(network), command);
}

void
NetworkList::setCurrentRow(int row)
{
    setCurrentRow(row, QItemSelectionModel::ClearAndSelect);
}

void
NetworkList::setCurrentRow(int row, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(_model->index(row), command);
}

Aki::SqlNetwork*
NetworkList::takeNetwork(int row)
{
    return _model->takeNetwork(row);
}

void
NetworkList::slotItemActivated(const QModelIndex& index)
{
    emit networkActivated(networkFromIndex(index));
}

void
NetworkList::slotItemChanged(const QModelIndex& index)
{
    emit networkChanged(networkFromIndex(index));
}

void
NetworkList::slotItemClicked(const QModelIndex& index)
{
    emit networkClicked(networkFromIndex(index));
}

void
NetworkList::slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    QPersistentModelIndex persistentCurrent = current;
    Aki::SqlNetwork* currentNetwork = item(persistentCurrent.row());
    Q_UNUSED(currentNetwork);
    emit currentNetworkChanged(currentNetwork, item(previous.row()));
    emit networkCurrentRowChanged(persistentCurrent.row());
}

void
NetworkList::slotItemDoubleClicked(const QModelIndex& index)
{
    emit networkDoubleClicked(networkFromIndex(index));
}

void
NetworkList::slotItemEntered(const QModelIndex& index)
{
    emit networkEntered(networkFromIndex(index));
}

void
NetworkList::slotItemPressed(const QModelIndex& index)
{
    emit networkPressed(networkFromIndex(index));
}
