#ifndef AKI_IDENTITYMODEL_HPP
#define AKI_IDENTITYMODEL_HPP

#include <QtCore/QAbstractItemModel>

namespace Aki
{
class SqlIdentity;
class IdentityModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit IdentityModel(QObject* parent = 0);
    ~IdentityModel();
    void addIdentity(Aki::SqlIdentity* identity);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void insertIdentity(int row, Aki::SqlIdentity* identity);
    QList<Aki::SqlIdentity*> identities() const;
    void removeIdentity(Aki::SqlIdentity* identity);
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Aki::SqlIdentity* takeIdentity(int row);
private:
    QList<Aki::SqlIdentity*> _identityList;
}; // End of class IdentityModel.
} // End of namespace Aki.

#endif // AKI_IDENTITYMODEL_HPP
