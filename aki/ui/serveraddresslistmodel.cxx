#include "serveraddresslistmodel.hpp"
#include "ui/serveraddresslist.hpp"
#include "utils/sqladdress.hpp"
#include <KIcon>
using namespace Aki;

ServerAddressListModel::ServerAddressListModel(Aki::ServerAddressList* parent)
    : QAbstractListModel(parent)
{
    _addressList.clear();
}

ServerAddressListModel::~ServerAddressListModel()
{
    qDeleteAll(_addressList);
}

void
ServerAddressListModel::addAddress(Aki::SqlAddress* address)
{
    insertAddress(rowCount(), address);
}

Aki::SqlAddress*
ServerAddressListModel::address(const QModelIndex& index)
{
    if (!index.isValid()) {
        return 0;
    }

    return static_cast<Aki::SqlAddress*>(_addressList.value(index.row(), 0));
}

Aki::SqlAddress*
ServerAddressListModel::address(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return static_cast<Aki::SqlAddress*>(_addressList.value(index.row(), 0));
}

Aki::ServerAddressListModel::List
ServerAddressListModel::address() const
{
    return _addressList;
}

QVariant
ServerAddressListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Aki::SqlAddress* addy = address(index);

    switch (role) {
    case Qt::DisplayRole: {
        return QString("%1:%2").arg(addy->address(), QString::number(addy->port()));
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
ServerAddressListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void
ServerAddressListModel::insertAddress(int row, Aki::SqlAddress* address)
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
ServerAddressListModel::removeAddress(Aki::SqlAddress* address)
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
ServerAddressListModel::rowCount(const QModelIndex&) const
{
    return _addressList.count();
}

bool
ServerAddressListModel::setData(const QModelIndex& index, const QVariant& value, int role)
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
ServerAddressListModel::takeAt(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::SqlAddress* addy = _addressList.takeAt(row);
    endRemoveRows();

    return addy;
}
