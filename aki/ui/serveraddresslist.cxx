#include "serveraddresslist.hpp"
#include "aki.hpp"
#include "ui/serveraddresslistmodel.hpp"
#include "utils/sqladdress.hpp"
using namespace Aki;

ServerAddressList::ServerAddressList(QWidget* parent)
    : QListView(parent),
    _currentIdentity(QString())
{
    _addressHash.clear();
}

ServerAddressList::~ServerAddressList()
{
    foreach (const QString& identity, _addressHash.keys()) {
        delete takeIdentity(identity);
    }
}

void
ServerAddressList::addAddress(Aki::SqlAddress* address)
{
    qxtLog->info() << QString("Adding address: %1").arg(address->address());
    currentModel()->addAddress(address);
    qxtLog->info() << QString("Current address count: %1").arg(currentModel()->address().count());
}

void
ServerAddressList::addIdentity(const QString& identity)
{
    qxtLog->info() << QString("Adding Identity: %1").arg(identity);
    _addressHash.insert(identity, new Aki::ServerAddressListModel);
    const int count = _addressHash.count();
    if (count == 1) {
        qxtLog->info() << "Setting current identity since it's the only one";
        setCurrentIdentity(identity);
    }
    qxtLog->info() << QString("Current identity count: %1").arg(count);
}

Aki::SqlAddress*
ServerAddressList::address(int row) const
{
    return currentModel()->address().value(row, 0);
}

int
ServerAddressList::count()
{
    return currentModel()->rowCount();
}

QString
ServerAddressList::currentIdentity() const
{
    return _currentIdentity;
}

Aki::SqlAddress*
ServerAddressList::currentAddress()
{
    return currentModel()->address(selectionModel()->currentIndex());
}

Aki::SqlAddress*
ServerAddressList::currentAddress() const
{
    return currentModel()->address(selectionModel()->currentIndex());
}

Aki::ServerAddressListModel*
ServerAddressList::currentModel()
{
    return _addressHash[currentIdentity()];
}

Aki::ServerAddressListModel*
ServerAddressList::currentModel() const
{
    return _addressHash[currentIdentity()];
}

int
ServerAddressList::currentRow() const
{
    return selectionModel()->currentIndex().row();
}

QList<Aki::SqlAddress*>
ServerAddressList::findItems(const QString& name, Qt::MatchFlags flags) const
{
    Aki::ServerAddressListModel::List list;
    Aki::ServerAddressListModel* model = currentModel();

    QModelIndexList indexes = model->match(model->index(0), Qt::DisplayRole, name, -1, flags);
    for (int i = 0; i < indexes.size(); ++i) {
        list.append(model->address(indexes.at(i)));
    }

    return list;
}

void
ServerAddressList::insertAddress(int row, Aki::SqlAddress* address)
{
    qxtLog->info() << QString("Adding network: %1").arg(address->address());
    currentModel()->insertAddress(row, address);
    qxtLog->info() << QString("Current network count: %1").arg(currentModel()->address().count());
}

int
ServerAddressList::row(Aki::SqlAddress* address) const
{
    return currentModel()->address().indexOf(const_cast<Aki::SqlAddress*>(address));
}

QList<Aki::SqlAddress*>
ServerAddressList::selectedItems() const
{
    Aki::ServerAddressListModel::List list;
    QModelIndexList indexList = selectionModel()->selectedRows();
    foreach (const QModelIndex& index, indexList) {
        list.append(currentModel()->address(index));
    }

    return list;
}

void
ServerAddressList::setCurrentIdentity(const QString& identity)
{
    if (_currentIdentity == identity) {
        qxtLog->info() << "Ignoring set current identity since it's the same"
                          " one being requested.";
        return;
    }

    _currentIdentity = identity;
    setModel(currentModel());
    qxtLog->info() << QString("Settings model to %1").arg(_currentIdentity);
}

Aki::ServerAddressListModel*
ServerAddressList::takeIdentity(const QString& identity)
{
    return _addressHash.take(identity);
}

Aki::SqlAddress*
ServerAddressList::takeAddress(int row)
{
    return currentModel()->takeAt(row);
}
