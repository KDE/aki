#include "channelmodel.hpp"
#include "aki.hpp"
#include "utils/sqlchannel.hpp"
#include <KDE/KIcon>
using namespace Aki;

ChannelModel::ChannelModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

ChannelModel::~ChannelModel()
{
}

void
ChannelModel::addChannel(Aki::SqlChannel* channel)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _channelList.append(channel);
    endInsertRows();
}

QList<Aki::SqlChannel*>
ChannelModel::channels() const
{
    return _channelList;
}

QVariant
ChannelModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        qxtLog->warning() << "Index is invalid so ignoring data call";
        return QVariant();
    }

    Aki::SqlChannel* channel = _channelList.at(index.row());
    if (!channel) {
        qxtLog->warning() << "Uh oh invalid server for row";
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return channel->channel();
    }
    default: {
        break;
    }
    }

    return QVariant();
}

void
ChannelModel::insertChannel(int row, Aki::SqlChannel* channel)
{
    if (!channel) {
        return;
    }

    if (row > rowCount()) {
        addChannel(channel);
        return;
    } else if (row < 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        _channelList.prepend(channel);
        endInsertRows();
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    _channelList.insert(row, channel);
    endInsertRows();
}

void
ChannelModel::removeChannel(Aki::SqlChannel* channel)
{
    if (!channel) {
        return;
    }

    const int row = _channelList.indexOf(channel);
    beginRemoveRows(QModelIndex(), row, row);
    delete takeChannel(row);
    endRemoveRows();
}

int
ChannelModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return _channelList.count();
}

Aki::SqlChannel*
ChannelModel::takeChannel(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::SqlChannel* channel = _channelList.takeAt(row);
    endRemoveRows();

    return channel;
}
