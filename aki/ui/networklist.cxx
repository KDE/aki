#include "networklist.hpp"
#include "ui/networklistmodel.hpp"
#include "utils/sqlserver.hpp"
using namespace Aki;

NetworkList::NetworkList(QWidget* parent)
    : QListView(parent),
    _modelCurrentRow(-1)
{
    _modelList.clear();
}

NetworkList::~NetworkList()
{
    qDeleteAll(_modelList);
}

void
NetworkList::addModel(Aki::NetworkListModel* model)
{
    _modelList.append(model);
    if (modelCount() == 0) {
        setCurrentModel(model);
    }
}

void
NetworkList::addNetwork(Aki::SqlServer* network)
{
    currentModel()->insertNetwork(network);
}

int
NetworkList::count() const
{
    return currentModel()->rowCount();
}

Aki::NetworkListModel*
NetworkList::currentModel() const
{
    return _modelList.at(_modelCurrentRow);
}

int
NetworkList::currentModelRow() const
{
    return _modelCurrentRow;
}

Aki::SqlServer*
NetworkList::currentNetwork() const
{
    return currentModel()->server(selectionModel()->currentIndex());
}

int NetworkList::currentRow() const
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
    _modelList.insert(row, model);
    if (modelCount() == 1) {
        setCurrentModel(model);
    }
}

void
NetworkList::insertNetwork(int row, SqlServer* network)
{
    Q_UNUSED(row);
    currentModel()->insertNetwork(network);
}

Aki::NetworkListModel*
NetworkList::model(int row) const
{
    return _modelList.value(row, 0);
}

int
NetworkList::modelCount() const
{
    return _modelList.count();
}

Aki::SqlServer*
NetworkList::network(int row) const
{
    return currentModel()->networks().value(row, 0);
}

int
NetworkList::row(const Aki::NetworkListModel* model) const
{
    return _modelList.indexOf(const_cast<Aki::NetworkListModel*>(model));
}

int
NetworkList::row(const Aki::SqlServer* network) const
{
    return currentModel()->networks().indexOf(const_cast<Aki::SqlServer*>(network));
}

QList<Aki::SqlServer*>
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
NetworkList::setCurrentModel(Aki::NetworkListModel* model)
{
    if (_modelCurrentRow == row(model)) {
        return;
    }

    _modelCurrentRow = row(model);
    setModel(_modelList.at(_modelCurrentRow));
}

void
NetworkList::setCurrentNetwork(Aki::SqlServer* network)
{
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
    return _modelList.takeAt(row);
}

Aki::SqlServer*
NetworkList::takeNetwork(int row)
{
    return currentModel()->takeAt(row);
}
