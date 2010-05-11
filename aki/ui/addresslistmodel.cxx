#include "addresslistmodel.hpp"
#include "ui/addresslist.hpp"
#include "utils/sqladdress.hpp"
#include <KIcon>
using namespace Aki;

AddressListModel::AddressListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    _addressList.clear();
}

AddressListModel::~AddressListModel()
{
    qDeleteAll(_addressList);
}

void
AddressListModel::addAddress(Aki::SqlAddress* address)
{
    insertAddress(rowCount(), address);
}

Aki::SqlAddress*
AddressListModel::address(const QModelIndex& index)
{
    if (!index.isValid()) {
        return 0;
    }

    return static_cast<Aki::SqlAddress*>(_addressList.value(index.row(), 0));
}

Aki::SqlAddress*
AddressListModel::address(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return static_cast<Aki::SqlAddress*>(_addressList.value(index.row(), 0));
}

Aki::AddressListModel::List
AddressListModel::address() const
{
    return _addressList;
}

QVariant
AddressListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Aki::SqlAddress* addy = address(index);

    switch (role) {
    case Qt::DisplayRole: {
        return addy->address();
    }
    case Qt::DecorationRole: {
        if (addy->isSslEnabled()) {
            return KIcon("document-encrypt");
        }
        return QVariant();
    }
    }

    return QVariant();
}

Qt::ItemFlags
AddressListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void
AddressListModel::insertAddress(int row, Aki::SqlAddress* address)
{
    if (!address) {
        return;
    }

    if (row > rowCount()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        _addressList.append(address);
        endInsertRows();
        return;
    } else if (row < 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        _addressList.prepend(address);
        endInsertRows();
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    _addressList.insert(row, address);
    endInsertRows();
}

void
AddressListModel::removeAddress(Aki::SqlAddress* address)
{
    if (!address) {
        return;
    }

    const int row = _addressList.indexOf(address);
    beginRemoveRows(QModelIndex(), row, row);
    delete _addressList.takeAt(row);
    endRemoveRows();
}

int
AddressListModel::rowCount(const QModelIndex&) const
{
    return _addressList.count();
}

bool
AddressListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    Aki::SqlAddress* addy = address(index);
    if (!addy) {
        return false;
    }

    switch (role) {
    case Qt::DisplayRole: {
        addy->setAddress(value.toString());
        break;
    }
    default: {
        return false;
    }
    }

    emit dataChanged(index, index);
    return true;
}

Aki::SqlAddress*
AddressListModel::takeAt(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::SqlAddress* addy = _addressList.takeAt(row);
    endRemoveRows();

    return addy;
}
