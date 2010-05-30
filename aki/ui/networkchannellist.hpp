#ifndef AKI_NETWORKCHANNELLIST_HPP
#define AKI_NETWORKCHANNELLIST_HPP

#include <QtGui/QListView>

namespace Aki
{
class ChannelModel;
class SqlChannel;
class SqlNetwork;
class NetworkChannelList : public QListView
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlChannel*> List;
    explicit NetworkChannelList(QWidget* parent = 0);
    ~NetworkChannelList();
    void addNetworkChannel(Aki::SqlChannel* channel);
    int count() const;
    Aki::SqlChannel* currentNetworkChannel() const;
    int currentRow() const;
    Aki::NetworkChannelList::List findItems(const QString& name, Qt::MatchFlags flags) const;
    void insertNetworkChannel(int row, Aki::SqlChannel* channel);
    Aki::SqlChannel* networkChannel(int row) const;
    int row(Aki::SqlChannel* channel) const;
    Aki::NetworkChannelList::List selectedNetworkChannels() const;
    void setCurrentNetworkChannel(Aki::SqlChannel* channel, QItemSelectionModel::SelectionFlags command);
    void setCurrentNetworkChannel(Aki::SqlChannel* channel);
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);
    void setCurrentRow(int row);
    Aki::SqlChannel* takeNetworkChannel(int row);
public Q_SLOTS:
    void repopulateNetworkChannels(Aki::SqlNetwork* network);
Q_SIGNALS:
    void currentNetworkChannelChanged(Aki::SqlChannel* current, Aki::SqlChannel* previous);
    void networkChannelActivated(Aki::SqlChannel* channel);
    void networkChannelChanged(Aki::SqlChannel* channel);
    void networkChannelClicked(Aki::SqlChannel* channel);
    void networkChannelCurrentRowChanged(int row);
    void networkChannelDoubleClicked(Aki::SqlChannel* channel);
    void networkChannelEntered(Aki::SqlChannel* channel);
    void networkChannelPressed(Aki::SqlChannel* channel);
    void networkChannelSelectionChanged();
protected:
    QModelIndex indexFromNetworkChannel(Aki::SqlChannel* channel);
    Aki::SqlChannel* networkChannelFromIndex(const QModelIndex& index) const;
private:
    Aki::ChannelModel* _model;
}; // End of class NetworkChannelList.
} // End of namespace Aki.

#endif // AKI_NETWORKCHANNELLIST_HPP
