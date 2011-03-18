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


#include "addresslist.hpp"
#include "sql/address.hpp"
#include "sql/database.hpp"
#include "sql/server.hpp"
#include "ui/addressmodel.hpp"
using namespace Aki;
using namespace Sql;

AddressList::AddressList(QWidget* parent)
    : QListView(parent)
{
    _model = new Aki::AddressModel(this);
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
            SIGNAL(addressSelectionChanged()));
}

AddressList::~AddressList()
{
}

void
AddressList::addAddress(Aki::Sql::Address* address)
{
    // Performs check on address and inserts at the end of the list.
    insertAddress(count(), address);
}

Aki::Sql::Address*
AddressList::address(int row) const
{
    return addressFromIndex(_model->index(row));
}

Aki::Sql::Address*
AddressList::addressFromIndex(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return _model->addresses().at(index.row());
}

int
AddressList::count() const
{
    return _model->rowCount();
}

Aki::Sql::Address*
AddressList::currentAddress() const
{
    QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return 0;
    }

    return addressFromIndex(index);
}

int
AddressList::currentRow() const
{
    QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return -1;
    }

    return index.row();
}

Aki::AddressList::List
AddressList::findAddresses(const QString& name, Qt::MatchFlags flags) const
{
    Aki::AddressList::List list;

    QModelIndexList indexes = _model->match(_model->index(0), Qt::DisplayRole, name, -1, flags);
    foreach (const QModelIndex& index, indexes) {
        list.append(addressFromIndex(index));
    }

    return list;
}

QModelIndex
AddressList::indexFromAddress(Aki::Sql::Address* address)
{
    return _model->index(_model->addresses().indexOf(address));
}

void
AddressList::insertAddress(int row, Aki::Sql::Address* address)
{
    Q_ASSERT(address);

    if (_model->addresses().contains(address)) {
        return;
    }

    _model->insertAddress(row, address);
}

void
AddressList::removeAddress(int row)
{
    delete takeAddress(row);
}

void
AddressList::repopulateAddresses(Aki::Sql::Server* server)
{
    Q_ASSERT(server);

    foreach (Aki::Sql::Address* address, _model->addresses()) {
        removeAddress(row(address));
    }

    const Aki::AddressList::List list = _database->find<Aki::Sql::Address>()
        .where("addressServer = :addressServer").bind("addressServer", server->id())
        .result();

    foreach (Aki::Sql::Address* address, list) {
        addAddress(address);
    }
}

int
AddressList::row(Aki::Sql::Address* address) const
{
    return _model->addresses().indexOf(address);
}

Aki::AddressList::List
AddressList::selectedAddresses() const
{
    Aki::AddressList::List list;

    const QModelIndexList indexes = selectionModel()->selectedRows();
    foreach (const QModelIndex& index, indexes) {
        list.append(addressFromIndex(index));
    }

    return list;
}

void
AddressList::setCurrentAddress(Aki::Sql::Address* address, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(indexFromAddress(address), command);
}

void
AddressList::setCurrentRow(int row, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(_model->index(row), command);
}

void
AddressList::setDatabase(Aki::Sql::Database* database)
{
    Q_ASSERT(database);
    _database = database;
}

void
AddressList::slotItemActivated(const QModelIndex& index)
{
    emit addressActivated(addressFromIndex(index));
}

void
AddressList::slotItemChanged(const QModelIndex& index)
{
    emit addressChanged(addressFromIndex(index));
}

void
AddressList::slotItemClicked(const QModelIndex& index)
{
    emit addressClicked(addressFromIndex(index));
}

void
AddressList::slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    QPersistentModelIndex persistentCurrent = current;
    Aki::Sql::Address* currentAddress = address(persistentCurrent.row());
    emit currentAddressChanged(currentAddress, address(previous.row()));
    emit addressCurrentRowChanged(persistentCurrent.row());
}

void
AddressList::slotItemDoubleClicked(const QModelIndex& index)
{
    emit addressDoubleClicked(addressFromIndex(index));
}

void
AddressList::slotItemEntered(const QModelIndex& index)
{
    emit addressEntered(addressFromIndex(index));
}

void
AddressList::slotItemPressed(const QModelIndex& index)
{
    emit addressPressed(addressFromIndex(index));
}

Aki::Sql::Address*
AddressList::takeAddress(int row)
{
    return _model->takeAddress(row);
}
