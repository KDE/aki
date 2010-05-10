#ifndef AKI_NETWORKLISTMODEL_HPP
#define AKI_NETWORKLISTMODEL_HPP

#include <QAbstractListModel>

namespace Aki
{
class NetworkList;
class SqlServer;
class NetworkListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlServer*> List;
    explicit NetworkListModel(Aki::NetworkList* parent = 0);
    ~NetworkListModel();
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    void addNetwork(Aki::SqlServer* network);
    void insertNetwork(int row, Aki::SqlServer* network);
    void removeNetwork(Aki::SqlServer* network);
    Aki::SqlServer* server(const QModelIndex& index);
    Aki::SqlServer* server(const QModelIndex& index) const;
    Aki::NetworkListModel::List networks() const;
    Aki::SqlServer* takeAt(int row);
private:
    QList<Aki::SqlServer*> _networkList;
}; // End of class NetworkListModel.
} // End of namespace Aki.

#endif // AKI_NETWORKLISTMODEL_HPP
