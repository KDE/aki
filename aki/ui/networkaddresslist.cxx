#include "networkaddresslist.hpp"
#include "aki.hpp"
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
NetworkAddressList::addNetworkAddress(Aki::SqlAddress* address)
{
    insertNetworkAddress(count(), address);
}

int
NetworkAddressList::count() const
{
    return model()->rowCount();
}

Aki::SqlAddress*
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
NetworkAddressList::indexFromNetworkAddress(Aki::SqlAddress* address)
{
    return _model->index(_model->addresses().indexOf(address));
}

void
NetworkAddressList::insertNetworkAddress(int row, Aki::SqlAddress* address)
{
    Q_ASSERT(address);

    if (_model->addresses().contains(address)) {
        return;
    }

    _model->insertAddress(row, address);
}

Aki::SqlAddress*
NetworkAddressList::networkAddress(int row) const
{
    return networkAddressFromIndex(_model->index(row));
}

Aki::SqlAddress*
NetworkAddressList::networkAddressFromIndex(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return _model->addresses().value(index.row());
}

void
NetworkAddressList::repopulateNetworkAddresses(Aki::SqlNetwork* network)
{
    foreach(Aki::SqlAddress* address, _model->addresses()) {
        qxtLog->info() << QString("Removing Address: %1").arg(address->address());
        _model->removeAddress(address);
    }

    if (!network) {
        return;
    }

    Aki::SqlAddress::List list = Aki::SqlAddress::addressListForServer(network);
    if (list.isEmpty()) {
        qxtLog->info() << QString("List is empty for Network: %1").arg(network->name());
        return;
    }

    foreach (Aki::SqlAddress* address, list) {
        qxtLog->info() << QString("Adding new Address: %1").arg(address->address());
        addNetworkAddress(address);
    }

    if (count()) {
        setCurrentRow(0);
    }
}

int
NetworkAddressList::row(Aki::SqlAddress* address) const
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
NetworkAddressList::setCurrentNetworkAddress(Aki::SqlAddress* address,
                                             QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(indexFromNetworkAddress(address), command);
}

void
NetworkAddressList::setCurrentNetworkAddress(Aki::SqlAddress* address)
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

Aki::SqlAddress*
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
    Aki::SqlAddress* currentAddress = networkAddress(persistentCurrent.row());
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
