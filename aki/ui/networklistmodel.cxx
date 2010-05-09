#include "networklistmodel.hpp"
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
NetworkListModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid()) {
        return 0;
    }

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
NetworkListModel::insertNetwork(SqlServer* network)
{
    if (!network) {
        return;
    }

    int row = _networkList.count();
    if (_networkList.count() == 0) {
        row = 0;
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
