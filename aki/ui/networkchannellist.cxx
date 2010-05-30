#include "networkchannellist.hpp"
#include "aki.hpp"
#include "ui/channelmodel.hpp"
#include "utils/sqlchannel.hpp"
#include "utils/sqlnetwork.hpp"
using namespace Aki;

NetworkChannelList::NetworkChannelList(QWidget* parent)
    : QListView(parent)
{
    _model = new Aki::ChannelModel(this);
    setModel(_model);

    connect(this, SIGNAL(pressed(QModelIndex)),
            SLOT(slotItemPressed(QModelIndex)));
    connect(this, SIGNAL(clicked(QModelIndex)),
            SLOT(slotItemClicked(QModelIndex)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)),
            SLOT(slotItemDoubleClicked(QModelIndex)));
    connect(this, SIGNAL(activated(QModelIndex)),
            SLOT(slotItemActivated(QModelIndex)));
    connect(this, SIGNAL(entered(QModelIndex)),
            SLOT(slotItemEntered(QModelIndex)));
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(slotItemChanged(QModelIndex)));
    connect(selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            SLOT(slotItemCurrentChanged(QModelIndex,QModelIndex)));
    connect(selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SIGNAL(networkChannelSelectionChanged()));
    //connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
    //        SLOT(dataChanged(QModelIndex,QModelIndex)));
}

NetworkChannelList::~NetworkChannelList()
{
}

void
NetworkChannelList::addNetworkChannel(Aki::SqlChannel* channel)
{
    _model->addChannel(channel);
}

int
NetworkChannelList::count() const
{
    return _model->rowCount();
}

Aki::SqlChannel*
NetworkChannelList::currentNetworkChannel() const
{
    QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return 0;
    }

    return networkChannelFromIndex(index);
}

int
NetworkChannelList::currentRow() const
{
    QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return -1;
    }

    return index.row();
}

Aki::NetworkChannelList::List
NetworkChannelList::findItems(const QString& name, Qt::MatchFlags flags) const
{
    Aki::NetworkChannelList::List list;

    QModelIndexList indexes = _model->match(_model->index(0), Qt::DisplayRole, name, -1, flags);
    for (int i = 0; i < indexes.size(); ++i) {
        list.append(networkChannelFromIndex(indexes.at(i)));
    }

    return list;
}

QModelIndex
NetworkChannelList::indexFromNetworkChannel(Aki::SqlChannel* channel)
{
    return _model->index(_model->channels().indexOf(channel));
}

void
NetworkChannelList::insertNetworkChannel(int row, Aki::SqlChannel* channel)
{
    Q_ASSERT(channel);

    if (_model->channels().contains(channel)) {
        return;
    }

    _model->insertChannel(row, channel);
}

Aki::SqlChannel*
NetworkChannelList::networkChannel(int row) const
{
    return networkChannelFromIndex(_model->index(row));
}

Aki::SqlChannel*
NetworkChannelList::networkChannelFromIndex(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return _model->channels().value(index.row());
}

void
NetworkChannelList::repopulateNetworkChannels(Aki::SqlNetwork* network)
{
    foreach (Aki::SqlChannel* channel, _model->channels()) {
        qxtLog->info() << QString("Removing Channel: %1").arg(channel->channel());
        _model->removeChannel(channel);
    }

    if (!network) {
        return;
    }

    Aki::SqlChannel::List list = Aki::SqlChannel::channelListForServer(network);
    if (list.isEmpty()) {
        qxtLog->info() << QString("List is empty for Network: %1").arg(network->name());
        return;
    }

    foreach (Aki::SqlChannel* channel, list) {
        qxtLog->info() << QString("Adding new Channel: %1").arg(channel->channel());
        addNetworkChannel(channel);
    }
}

int
NetworkChannelList::row(Aki::SqlChannel* channel) const
{
    return _model->channels().indexOf(channel);
}

Aki::NetworkChannelList::List
NetworkChannelList::selectedNetworkChannels() const
{
    Aki::NetworkChannelList::List list;

    QModelIndexList indexes = selectionModel()->selectedRows();
    foreach (const QModelIndex& index, indexes) {
        list.append(networkChannelFromIndex(index));
    }

    return list;
}

void
NetworkChannelList::setCurrentNetworkChannel(Aki::SqlChannel* channel, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(indexFromNetworkChannel(channel), command);
}

void
NetworkChannelList::setCurrentNetworkChannel(Aki::SqlChannel* channel)
{
    setCurrentNetworkChannel(channel, QItemSelectionModel::ClearAndSelect);
}

void
NetworkChannelList::setCurrentRow(int row, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(_model->index(row), command);
}

void
NetworkChannelList::setCurrentRow(int row)
{
    setCurrentRow(row, QItemSelectionModel::ClearAndSelect);
}

Aki::SqlChannel*
NetworkChannelList::takeNetworkChannel(int row)
{
    return _model->takeChannel(row);
}
