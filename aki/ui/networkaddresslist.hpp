#ifndef AKI_NETWORKADDRESSLIST_HPP
#define AKI_NETWORKADDRESSLIST_HPP

#include <QtGui/QListView>

namespace Aki
{
class AddressModel;
class SqlAddress;
class SqlNetwork;
class NetworkAddressList : public QListView
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlAddress*> List;
    explicit NetworkAddressList(QWidget* parent = 0);
    ~NetworkAddressList();
    void addNetworkAddress(Aki::SqlAddress* address);
    int count() const;
    Aki::SqlAddress* currentNetworkAddress() const;
    int currentRow() const;
    Aki::NetworkAddressList::List findItems(const QString& name, Qt::MatchFlags flags) const;
    void insertNetworkAddress(int row, Aki::SqlAddress* network);
    Aki::SqlAddress* networkAddress(int row) const;
    int row(Aki::SqlAddress* address) const;
    Aki::NetworkAddressList::List selectedNetworkAddresses() const;
    void setCurrentNetworkAddress(Aki::SqlAddress* address, QItemSelectionModel::SelectionFlags command);
    void setCurrentNetworkAddress(Aki::SqlAddress* address);
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);
    void setCurrentRow(int row);
    Aki::SqlAddress* takeNetworkAddress(int row);
public Q_SLOTS:
    void repopulateNetworkAddresses(Aki::SqlNetwork* network);
Q_SIGNALS:
    void currentNetworkAddressChanged(Aki::SqlAddress* current, Aki::SqlAddress* previous);
    void networkAddressActivated(Aki::SqlAddress* address);
    void networkAddressChanged(Aki::SqlAddress* address);
    void networkAddressClicked(Aki::SqlAddress* address);
    void networkAddressCurrentRowChanged(int row);
    void networkAddressDoubleClicked(Aki::SqlAddress* address);
    void networkAddressEntered(Aki::SqlAddress* address);
    void networkAddressPressed(Aki::SqlAddress* address);
    void networkAddressSelectionChanged();
private Q_SLOTS:
    void slotItemActivated(const QModelIndex& index);
    void slotItemChanged(const QModelIndex& index);
    void slotItemClicked(const QModelIndex& index);
    void slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
    void slotItemDoubleClicked(const QModelIndex& index);
    void slotItemEntered(const QModelIndex& index);
    void slotItemPressed(const QModelIndex& index);
protected:
    Aki::SqlAddress* networkAddressFromIndex(const QModelIndex& index) const;
    QModelIndex indexFromNetworkAddress(Aki::SqlAddress* address);
private:
    Aki::AddressModel* _model;
}; // End of class NetworkAddressList.
} // End of namespace Aki.

#endif // AKI_NETWORKADDRESSLIST_HPP
