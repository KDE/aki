#include "networkmodel.hpp"
#include "aki.hpp"
#include "utils/sqlnetwork.hpp"
using namespace Aki;

NetworkModel::NetworkModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

NetworkModel::~NetworkModel()
{
    qDeleteAll(_networkList);
}

void
NetworkModel::addNetwork(Aki::SqlNetwork* network)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _networkList.append(network);
    endInsertRows();
}

QVariant
NetworkModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        qxtLog->warning() << "Index is invalid so ignoring data call";
        return QVariant();
    }

    Aki::SqlNetwork* server = _networkList.at(index.row());
    if (!server) {
        qxtLog->warning() << "Uh oh invalid server for row";
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return server->name();
    }
    }

    return QVariant();
}

void
NetworkModel::insertNetwork(int row, SqlNetwork* network)
{
    if (!network) {
        return;
    }

    if (row > rowCount()) {
        addNetwork(network);
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

QList<Aki::SqlNetwork*>
NetworkModel::networks() const
{
    return _networkList;
}

void
NetworkModel::removeNetwork(Aki::SqlNetwork* network)
{
    if (!network) {
        return;
    }

    const int row = _networkList.indexOf(network);
    beginRemoveRows(QModelIndex(), row, row);
    delete takeNetwork(row);
    endRemoveRows();
}

int
NetworkModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return _networkList.count();
}

Aki::SqlNetwork*
NetworkModel::takeNetwork(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::SqlNetwork* network = _networkList.takeAt(row);
    endRemoveRows();

    return network;
}
