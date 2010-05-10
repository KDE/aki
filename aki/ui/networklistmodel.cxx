#include "networklistmodel.hpp"
#include "aki.hpp"
#include "ui/networklist.hpp"
#include "utils/sqlserver.hpp"
using namespace Aki;

NetworkListModel::NetworkListModel(NetworkList* parent)
    : QAbstractListModel(parent)
{
    _networkList.clear();
}

NetworkListModel::~NetworkListModel()
{
    qDeleteAll(_networkList);
}

QVariant
NetworkListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Aki::SqlServer* network = server(index);

    switch (role) {
    case Qt::DisplayRole: {
        return network->name();
    }
    }

    return QVariant();
}

int
NetworkListModel::rowCount(const QModelIndex&) const
{
    return _networkList.count();
}

bool
NetworkListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    Aki::SqlServer* network = server(index);
    if (!network) {
        return false;
    }

    switch (role) {
    case Qt::DisplayRole: {
        network->setName(value.toString());
        break;
    }
    default: {
        return false;
    }
    }

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags
NetworkListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void
NetworkListModel::addNetwork(SqlServer* network)
{
    insertNetwork(rowCount(), network);
}

void
NetworkListModel::insertNetwork(int row, SqlServer* network)
{
    if (!network) {
        return;
    }

    if (row > rowCount()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        _networkList.append(network);
        endInsertRows();
        return;
    } else if (row < 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        _networkList.prepend(network);
        endInsertRows();
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    _networkList.insert(row, network);
    endInsertRows();
}

void
NetworkListModel::removeNetwork(SqlServer* network)
{
    if (!network) {
        return;
    }

    const int row = _networkList.indexOf(network);
    beginRemoveRows(QModelIndex(), row, row);
    delete _networkList.takeAt(row);
    endRemoveRows();
}

Aki::SqlServer*
NetworkListModel::server(const QModelIndex& index)
{
    if (!index.isValid()) {
        return 0;
    }

    return static_cast<Aki::SqlServer*>(_networkList.value(index.row(), 0));
}


Aki::SqlServer*
NetworkListModel::server(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return static_cast<Aki::SqlServer*>(_networkList.value(index.row(), 0));
}

Aki::NetworkListModel::List
NetworkListModel::networks() const
{
    return _networkList;
}

Aki::SqlServer* NetworkListModel::takeAt(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::SqlServer* network = _networkList.takeAt(row);
    endRemoveRows();

    return network;
}
