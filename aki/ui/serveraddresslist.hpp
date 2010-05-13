#ifndef AKI_SERVERADDRESSLIST_HPP
#define AKI_SERVERADDRESSLIST_HPP

#include <QtGui/QListView>

namespace Aki
{
class ServerAddressListModel;
class SqlAddress;
class ServerAddressList : public QListView
{
    Q_OBJECT
public:
    typedef QHash<QString, Aki::ServerAddressListModel*> Hash;
    explicit ServerAddressList(QWidget* parent = 0);
    ~ServerAddressList();
    void addIdentity(const QString& identity);
    void addAddress(Aki::SqlAddress* address);
    int count();
    Aki::ServerAddressListModel* currentModel();
    Aki::ServerAddressListModel* currentModel() const;
    QString currentIdentity() const;
    Aki::SqlAddress* currentAddress();
    Aki::SqlAddress* currentAddress() const;
    int currentRow() const;
    QList<Aki::SqlAddress*> findItems(const QString& name, Qt::MatchFlags flags) const;
    void insertAddress(int row, Aki::SqlAddress* address);
    Aki::SqlAddress* address(int row) const;
    int row(Aki::SqlAddress* address) const;
    QList<Aki::SqlAddress*> selectedItems() const;
    void setCurrentIdentity(const QString& identity);
    Aki::SqlAddress* takeAddress(int row);
    Aki::ServerAddressListModel* takeIdentity(const QString& identity);
private:
    Aki::ServerAddressList::Hash _addressHash;
    QString _currentIdentity;
}; // End of class ServerAddressList.
} // End of namespace Aki.

#endif // AKI_SERVERADDRESSLIST_HPP
