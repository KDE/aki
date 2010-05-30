#ifndef AKI_ADDRESSMODEL_HPP
#define AKI_ADDRESSMODEL_HPP

#include <QtCore/QAbstractItemModel>

namespace Aki
{
class SqlAddress;
class AddressModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AddressModel(QObject* parent = 0);
    ~AddressModel();
    void addAddress(Aki::SqlAddress* address);
    QList<Aki::SqlAddress*> addresses() const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    void insertAddress(int row, Aki::SqlAddress* address);
    void removeAddress(Aki::SqlAddress* address);
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Aki::SqlAddress* takeAddress(int row);
private:
    QList<Aki::SqlAddress*> _addressList;
}; // End of class AddressModel.
} // End of namespace Aki.

#endif // AKI_ADDRESSMODEL_HPP
