#ifndef AKI_CHANNELMODEL_HPP
#define AKI_CHANNELMODEL_HPP

#include <QtCore/QAbstractItemModel>

namespace Aki
{
class SqlChannel;
class ChannelModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ChannelModel(QObject* parent = 0);
    ~ChannelModel();
    void addChannel(Aki::SqlChannel* channel);
    QList<Aki::SqlChannel*> channels() const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void insertChannel(int row, Aki::SqlChannel* channel);
    void removeChannel(Aki::SqlChannel* channel);
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Aki::SqlChannel* takeChannel(int row);
private:
    QList<Aki::SqlChannel*> _channelList;
}; // End of class ChannelModel.
} // End of namespace Aki.

#endif // AKI_CHANNELMODEL_HPP
