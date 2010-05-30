#ifndef AKI_IDENTITYCOMBOBOX_HPP
#define AKI_IDENTITYCOMBOBOX_HPP

#include <KDE/KComboBox>

namespace Aki
{
class IdentityModel;
class SqlIdentity;
class IdentityComboBox : public KComboBox
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlIdentity*> List;
    explicit IdentityComboBox(QWidget* parent = 0);
    ~IdentityComboBox();
    void addIdentity(Aki::SqlIdentity* identity);
    Aki::SqlIdentity* currentIdentity() const;
    Aki::IdentityComboBox::List findIdentities(const QString& name, Qt::MatchFlags flags);
    void insertIdentity(int row, Aki::SqlIdentity* identity);
    Aki::SqlIdentity* identity(int index) const;
    int row(Aki::SqlIdentity* identity) const;
    void setCurrentIdentity(Aki::SqlIdentity* identity);
    Aki::SqlIdentity* takeIdentity(int index);
Q_SIGNALS:
    void identityActivated(Aki::SqlIdentity* identity);
    void currentIndexChanged(Aki::SqlIdentity* identity);
protected:
    QModelIndex indexFromIdentity(Aki::SqlIdentity* identity) const;
    Aki::SqlIdentity* identityFromIndex(const QModelIndex& index) const;
private Q_SLOTS:
    void slotActivated(int index);
    void slotCurrentIndexChanged(int index);
private:
    Aki::IdentityModel* _model;
}; // End of class IdentityComboBox.
} // End of namespace Aki.

#endif // AKI_IDENTITYCOMBOBOX_HPP
