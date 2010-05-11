#ifndef AKI_ADDRESSLISTMODEL_HPP
#define AKI_ADDRESSLISTMODEL_HPP

#include <QAbstractListModel>

namespace Aki
{
class AddressList;
class SqlAddress;
class AddressListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlAddress*> List;
    explicit AddressListModel(Aki::AddressList* parent = 0);
    ~AddressListModel();
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    void addAddress(Aki::SqlAddress* address);
    void insertAddress(int row, Aki::SqlAddress* address);
    void removeAddress(Aki::SqlAddress* address);
    Aki::SqlAddress* address(const QModelIndex& index);
    Aki::SqlAddress* address(const QModelIndex& index) const;
    Aki::AddressListModel::List address() const;
    Aki::SqlAddress* takeAt(int row);
private:
    Aki::AddressListModel::List _addressList;
}; // End of class AddressListModel.
} // End of namespace Aki.

#endif // AKI_ADDRESSLISTMODEL_HPP
