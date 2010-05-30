#include "addressmodel.hpp"
#include "aki.hpp"
#include "utils/sqladdress.hpp"
#include <KDE/KIcon>
using namespace Aki;

AddressModel::AddressModel(QObject* parent)
    : QAbstractListModel(parent)
{
    _addressList.clear();
}

AddressModel::~AddressModel()
{
    kDebug() << _addressList.count();
    qDeleteAll(_addressList);
}

void
AddressModel::addAddress(Aki::SqlAddress* address)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _addressList.append(address);
    endInsertRows();
}

QList<Aki::SqlAddress*>
AddressModel::addresses() const
{
    return _addressList;
}

QVariant
AddressModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        qxtLog->warning() << "Index is invalid so ignoring data call";
        return QVariant();
    }

    Aki::SqlAddress* address = _addressList.at(index.row());
    if (!address) {
        qxtLog->warning() << "Uh oh invalid address for row";
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return QString("%1:%2").arg(address->address(), QString::number(address->port()));
    }
    case Qt::DecorationRole: {
        if (address->isSslEnabled()) {
            return KIcon("document-encrypt");
        }
        return QVariant();
    }
    }

    return QVariant();
}

Qt::ItemFlags
AddressModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void
AddressModel::insertAddress(int row, Aki::SqlAddress* address)
{
    if (!address) {
        return;
    }

    if (row >= rowCount()) {
        addAddress(address);
        return;
    } else if (row <= 0) {
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
AddressModel::removeAddress(Aki::SqlAddress* address)
{
    if (!address) {
        return;
    }

    const int row = _addressList.indexOf(address);
    beginRemoveRows(QModelIndex(), row, row);
    delete takeAddress(row);
    endRemoveRows();
}

int
AddressModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return _addressList.count();
}

Aki::SqlAddress*
AddressModel::takeAddress(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::SqlAddress* address = _addressList.takeAt(row);
    endRemoveRows();

    return address;
}
