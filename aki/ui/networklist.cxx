#include "networklist.hpp"
#include "aki.hpp"
#include "ui/networklistmodel.hpp"
#include "utils/sqlidentity.hpp"
#include "utils/sqlserver.hpp"
using namespace Aki;

NetworkList::NetworkList(QWidget* parent)
    : QListView(parent),
    _currentIdentityModel(QString()),
    _modelCurrentRow(-1)
{
    _modelList.clear();
}

NetworkList::~NetworkList()
{
}

void
NetworkList::addIdentity(const QString& identity)
{
    qxtLog->info() << QString(QLatin1String("Adding Identity: %1")).arg(identity);
    _modelList.insert(identity, Aki::NetworkList::List());
    if (_modelList.count() == 1) {
        qxtLog->info() << QLatin1String("Setting current identity since it's the only one");
        setCurrentIdentity(identity);
    }
    qxtLog->info() << QString(QLatin1String("Current identity count: %1")).arg(_modelList.count());
}

void
NetworkList::addModel(Aki::NetworkListModel* model)
{
    qxtLog->info() << QLatin1String("Inserting model");
    currentIdentity().append(model);
    if (modelCount() == 1) {
        qxtLog->info() << QLatin1String("Settings current model to 0 since it's the only one");
        setCurrentModel(0);
    }

    qxtLog->info() << QString(QLatin1String("Current model count: %1")).arg(modelCount());
}

void
NetworkList::addNetwork(Aki::SqlServer* network)
{
    qxtLog->info() << QString(QLatin1String("Adding network: %1")).arg(network->name());
    currentModel()->addNetwork(network);
    setCurrentNetwork(network);
    qxtLog->info() << QString(QLatin1String("Current network count: %1")).arg(currentModel()->networks().count());
}

int
NetworkList::count() const
{
    return currentModel()->rowCount();
}

Aki::NetworkListModel*
NetworkList::currentModel()
{
    return currentIdentity().at(currentModelRow());
}

Aki::NetworkListModel*
NetworkList::currentModel() const
{
    return static_cast<Aki::NetworkListModel*>(model());
}

Aki::NetworkList::List&
NetworkList::currentIdentity()
{
    return _modelList[_currentIdentityModel];
}

const Aki::NetworkList::List
NetworkList::currentIdentity() const
{
    return _modelList[_currentIdentityModel];
}

int
NetworkList::currentModelRow() const
{
    return _modelCurrentRow;
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
NetworkList::insertModel(int row, NetworkListModel* model)
{
    qxtLog->info() << QLatin1String("Inserting model");
    currentIdentity().insert(row, model);
    if (modelCount() == 1) {
        qxtLog->info() << QLatin1String("Settings current model to 0 since it's the only one");
        setCurrentModel(model);
    }
    qxtLog->info() << QString(QLatin1String("Current model count: %1")).arg(modelCount());
}

void
NetworkList::insertNetwork(int row, SqlServer* network)
{
    qxtLog->info() << QString(QLatin1String("Adding network: %1")).arg(network->name());
    currentModel()->insertNetwork(row, network);
    qxtLog->info() << QString(QLatin1String("Current network count: %1")).arg(currentModel()->networks().count());
}

Aki::NetworkListModel*
NetworkList::modelAt(int row) const
{
    return currentIdentity().value(row, 0);
}

int
NetworkList::modelCount() const
{
    return currentIdentity().count();
}

Aki::SqlServer*
NetworkList::network(int row) const
{
    return currentModel()->networks().value(row, 0);
}

int
NetworkList::row(const Aki::NetworkListModel* model) const
{
    return currentIdentity().indexOf(const_cast<Aki::NetworkListModel*>(model));
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
    if (_currentIdentityModel == identity) {
        qxtLog->info() << QLatin1String("Ignoring set current identity since it's the same"
                                        " one being requested.");
        return;
    }

    _currentIdentityModel = identity;
    if (modelCount() != 0) {
        qxtLog->info() << QLatin1String("Changing model to index 0 for new Identity change");
        setCurrentModel(0);
    }
}

void
NetworkList::setCurrentModel(int row)
{
    _modelCurrentRow = row;
    setModel(currentIdentity().at(_modelCurrentRow));
    qxtLog->info() << QString(QLatin1String("Setting model to index: %1")).arg(_modelCurrentRow);
}

void
NetworkList::setCurrentModel(Aki::NetworkListModel* model)
{
    if (_modelCurrentRow == row(model)) {
        return;
    }

    _modelCurrentRow = row(model);
    setModel(currentIdentity().at(_modelCurrentRow));
    qxtLog->info() << QString(QLatin1String("Setting model to index: %1")).arg(_modelCurrentRow);
}

void
NetworkList::setCurrentNetwork(Aki::SqlServer* network)
{
    qxtLog->info() << QString(QLatin1String("Setting current network to: %1")).arg(network->name());
    setCurrentNetwork(network, QItemSelectionModel::ClearAndSelect);
}

void
NetworkList::setCurrentNetwork(Aki::SqlServer* network, QItemSelectionModel::SelectionFlags command)
{
    const int row = this->row(network);
    setCurrentRow(row, command);
}

void
NetworkList::setCurrentRow(int row)
{
    qxtLog->info() << QString(QLatin1String("Setting current network row to: %1")).arg(row);
    setCurrentRow(row, QItemSelectionModel::ClearAndSelect);
}

void
NetworkList::setCurrentRow(int row, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(currentModel()->index(row), command);
}

Aki::NetworkListModel*
NetworkList::takeModel(int row)
{
    return currentIdentity().takeAt(row);
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
        delete identity;
        return;
    }
    qxtLog->info() << QString(QLatin1String("Identitying is wanting to changed..."));
    addIdentity(identity->name());
    setCurrentIdentity(identity->name());
    Aki::SqlServer::List list = Aki::SqlServer::serversForIdentity(identity);
    addModel(new Aki::NetworkListModel(this));
    foreach (Aki::SqlServer* server, list) {
        addNetwork(server);
    }

    delete identity;
}
