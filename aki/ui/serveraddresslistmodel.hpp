#ifndef AKI_SERVERADDRESSLISTMODEL_HPP
#define AKI_SERVERADDRESSLISTMODEL_HPP

#include <QAbstractListModel>

namespace Aki
{
class ServerAddressList;
class SqlAddress;
class ServerAddressListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlAddress*> List;
    explicit ServerAddressListModel(Aki::ServerAddressList* parent = 0);
    ~ServerAddressListModel();
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    void addAddress(Aki::SqlAddress* address);
    void insertAddress(int row, Aki::SqlAddress* address);
    void removeAddress(Aki::SqlAddress* address);
    Aki::SqlAddress* address(const QModelIndex& index);
    Aki::SqlAddress* address(const QModelIndex& index) const;
    Aki::ServerAddressListModel::List address() const;
    Aki::SqlAddress* takeAt(int row);
private:
    Aki::ServerAddressListModel::List _addressList;
}; // End of class ServerAddressListModel.
} // End of namespace Aki.

#endif // AKI_SERVERADDRESSLISTMODEL_HPP
