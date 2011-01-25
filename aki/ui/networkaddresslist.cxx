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

#include "networkaddresslist.hpp"
#include "aki.hpp"
#include "debughelper.hpp"
#include "ui/addressmodel.hpp"
#include "utils/sqladdress.hpp"
#include "utils/sqlnetwork.hpp"
using namespace Aki;

NetworkAddressList::NetworkAddressList(QWidget* parent)
    : QListView(parent)
{
    _model = new Aki::AddressModel(this);
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
            SIGNAL(networkAddressSelectionChanged()));
    //connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
    //        SLOT(dataChanged(QModelIndex,QModelIndex)));
}

NetworkAddressList::~NetworkAddressList()
{
}

void
NetworkAddressList::addNetworkAddress(Aki::Sql::Address* address)
{
    insertNetworkAddress(count(), address);
}

int
NetworkAddressList::count() const
{
    return model()->rowCount();
}

Aki::Sql::Address*
NetworkAddressList::currentNetworkAddress() const
{
    QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return 0;
    }

    return networkAddressFromIndex(index);
}

int
NetworkAddressList::currentRow() const
{
    QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return -1;
    }

    return index.row();
}

Aki::NetworkAddressList::List
NetworkAddressList::findItems(const QString& name, Qt::MatchFlags flags) const
{
    Aki::NetworkAddressList::List list;
    QModelIndexList indexes = _model->match(_model->index(0), Qt::DisplayRole, name, -1, flags);
    for (int i = 0; i < indexes.size(); ++i) {
        list.append(networkAddressFromIndex(indexes.at(i)));
    }

    return list;
}

QModelIndex
NetworkAddressList::indexFromNetworkAddress(Aki::Sql::Address* address)
{
    return _model->index(_model->addresses().indexOf(address));
}

void
NetworkAddressList::insertNetworkAddress(int row, Aki::Sql::Address* address)
{
    Q_ASSERT(address);

    if (_model->addresses().contains(address)) {
        return;
    }

    _model->insertAddress(row, address);
}

Aki::Sql::Address*
NetworkAddressList::networkAddress(int row) const
{
    return networkAddressFromIndex(_model->index(row));
}

Aki::Sql::Address*
NetworkAddressList::networkAddressFromIndex(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return _model->addresses().value(index.row());
}

void
NetworkAddressList::repopulateNetworkAddresses(Aki::Sql::Server* network)
{
    /*DEBUG_FUNC_NAME;
    foreach(Aki::Sql::Address* address, _model->addresses()) {
        DEBUG_TEXT2("Removing Address: %1", address->address());
        _model->removeAddress(address);
    }

    if (!network) {
        return;
    }

    Aki::Sql::Address::List list;// = Aki::Sql::Address::addressListForServer(network);
    if (list.isEmpty()) {
        DEBUG_TEXT2("List is empty for Network: %1", network->name());
        return;
    }

    foreach (Aki::Sql::Address* address, list) {
        DEBUG_TEXT2("Adding new Address: %1", address->address());
        addNetworkAddress(address);
    }

    if (count()) {
        setCurrentRow(0);
    }*/
}

int
NetworkAddressList::row(Aki::Sql::Address* address) const
{
    return _model->addresses().indexOf(address);
}

Aki::NetworkAddressList::List
NetworkAddressList::selectedNetworkAddresses() const
{
    Aki::NetworkAddressList::List list;

    QModelIndexList indexes = selectionModel()->selectedRows();
    foreach (const QModelIndex& index, indexes) {
        list.append(networkAddressFromIndex(index));
    }

    return list;
}

void
NetworkAddressList::setCurrentNetworkAddress(Aki::Sql::Address* address,
                                             QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(indexFromNetworkAddress(address), command);
}

void
NetworkAddressList::setCurrentNetworkAddress(Aki::Sql::Address* address)
{
    setCurrentNetworkAddress(address, QItemSelectionModel::ClearAndSelect);
}

void
NetworkAddressList::setCurrentRow(int row, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(_model->index(row), command);
}

void
NetworkAddressList::setCurrentRow(int row)
{
    setCurrentRow(row, QItemSelectionModel::ClearAndSelect);
}

Aki::Sql::Address*
NetworkAddressList::takeNetworkAddress(int row)
{
    return _model->takeAddress(row);
}

void
NetworkAddressList::slotItemActivated(const QModelIndex& index)
{
    emit networkAddressActivated(networkAddressFromIndex(index));
}

void
NetworkAddressList::slotItemChanged(const QModelIndex& index)
{
    emit networkAddressChanged(networkAddressFromIndex(index));
}

void
NetworkAddressList::slotItemClicked(const QModelIndex& index)
{
    emit networkAddressClicked(networkAddressFromIndex(index));
}

void
NetworkAddressList::slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    QPersistentModelIndex persistentCurrent = current;
    Aki::Sql::Address* currentAddress = networkAddress(persistentCurrent.row());
    emit currentNetworkAddressChanged(currentAddress, networkAddress(previous.row()));
    emit networkAddressCurrentRowChanged(persistentCurrent.row());
}

void
NetworkAddressList::slotItemDoubleClicked(const QModelIndex& index)
{
    emit networkAddressDoubleClicked(networkAddressFromIndex(index));
}

void
NetworkAddressList::slotItemEntered(const QModelIndex& index)
{
    emit networkAddressEntered(networkAddressFromIndex(index));
}

void
NetworkAddressList::slotItemPressed(const QModelIndex& index)
{
    emit networkAddressPressed(networkAddressFromIndex(index));
}
