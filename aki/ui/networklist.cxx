#include "networklist.hpp"
#include "aki.hpp"
#include "ui/networklistmodel.hpp"
#include "utils/sqlidentity.hpp"
#include "utils/sqlserver.hpp"
using namespace Aki;

NetworkList::NetworkList(QWidget* parent)
    : QListView(parent),
    _currentIdentity(QString())
{
    _modelList.clear();
}

NetworkList::~NetworkList()
{
    foreach (const QString& identity, _modelList.keys()) {
        delete takeIdentity(identity);
    }
}

void
NetworkList::addIdentity(const QString& identity)
{
    qxtLog->info() << QString("Adding Identity: %1").arg(identity);
    _modelList.insert(identity, new Aki::NetworkListModel);
    if (_modelList.count() == 1) {
        qxtLog->info() << "Setting current identity since it's the only one";
        setCurrentIdentity(identity);
    }
    qxtLog->info() << QString("Current identity count: %1").arg(_modelList.count());
}

void
NetworkList::addNetwork(Aki::SqlServer* network)
{
    qxtLog->info() << QString("Adding network: %1").arg(network->name());
    currentModel()->addNetwork(network);
    setCurrentNetwork(network);
    qxtLog->info() << QString("Current network count: %1").arg(currentModel()->networks().count());
}

int
NetworkList::count() const
{
    return currentModel()->rowCount();
}

Aki::NetworkListModel*
NetworkList::currentModel() const
{
    return _modelList[currentIdentity()];
}

Aki::NetworkListModel* NetworkList::currentModel()
{
    return _modelList[currentIdentity()];
}

QString
NetworkList::currentIdentity() const
{
    return _currentIdentity;
}

Aki::SqlServer*
NetworkList::currentNetwork()
{
    return currentModel()->server(selectionModel()->currentIndex());
}

Aki::SqlServer*
NetworkList::currentNetwork() const
{
    return currentModel()->server(selectionModel()->currentIndex());
}

int
NetworkList::currentRow() const
{
    return selectionModel()->currentIndex().row();
}

QList<Aki::SqlServer*>
NetworkList::findItems(const QString& name, Qt::MatchFlags flags) const
{
    Aki::NetworkListModel::List list;
    Aki::NetworkListModel* model = currentModel();

    QModelIndexList indexes = model->match(model->index(0), Qt::DisplayRole, name, -1, flags);
    for (int i = 0; i < indexes.size(); ++i) {
        list.append(model->server(indexes.at(i)));
    }

    return list;
}

void
NetworkList::insertNetwork(int row, SqlServer* network)
{
    qxtLog->info() << QString("Adding network: %1").arg(network->name());
    currentModel()->insertNetwork(row, network);
    qxtLog->info() << QString("Current network count: %1").arg(currentModel()->networks().count());
}

Aki::SqlServer*
NetworkList::network(int row) const
{
    return currentModel()->networks().value(row, 0);
}

int
NetworkList::row(const Aki::SqlServer* network) const
{
    return currentModel()->networks().indexOf(const_cast<Aki::SqlServer*>(network));
}

Aki::NetworkListModel::List
NetworkList::selectedItems() const
{
    Aki::NetworkListModel::List list;
    QModelIndexList indexList = selectionModel()->selectedRows();
    foreach (const QModelIndex& index, indexList) {
        list.append(currentModel()->server(index));
    }

    return list;
}

void
NetworkList::setCurrentIdentity(const QString& identity)
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

void
NetworkList::setCurrentNetwork(Aki::SqlServer* network)
{
    qxtLog->info() << QString("Setting current network to: %1").arg(network->name());
    setCurrentNetwork(network, QItemSelectionModel::ClearAndSelect);
}

void
NetworkList::setCurrentNetwork(Aki::SqlServer* network, QItemSelectionModel::SelectionFlags command)
{
    const int r = row(network);
    setCurrentRow(r, command);
}

void
NetworkList::setCurrentRow(int row)
{
    qxtLog->info() << QString("Setting current network row to: %1").arg(row);
    setCurrentRow(row, QItemSelectionModel::ClearAndSelect);
}

void
NetworkList::setCurrentRow(int row, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(currentModel()->index(row), command);
}

Aki::NetworkListModel*
NetworkList::takeIdentity(const QString& identity)
{
    qxtLog->info() << "Removing Identity....: " << identity;
    return _modelList.take(identity);
}

Aki::SqlServer*
NetworkList::takeNetwork(int row)
{
    return currentModel()->takeAt(row);
}

void
NetworkList::identityActivated(Aki::SqlIdentity* identity)
{
    if (_modelList.contains(identity->name())) {
        setCurrentIdentity(identity->name());
        if (currentModel()->rowCount() > 0) {
            setCurrentNetwork(network(0));
        }
        delete identity;
        return;
    }
    qxtLog->info() << QString("Identitying is wanting to changed...");
    addIdentity(identity->name());
    setCurrentIdentity(identity->name());
    Aki::SqlServer::List list = Aki::SqlServer::serversForIdentity(identity);
    foreach (Aki::SqlServer* server, list) {
        addNetwork(server);
    }
    if (currentModel()->rowCount() > 0) {
        setCurrentNetwork(network(0));
    }

    delete identity;
}
