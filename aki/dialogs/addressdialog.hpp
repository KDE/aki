#ifndef AKI_ADDRESSDIALOG_HPP
#define AKI_ADDRESSDIALOG_HPP

#include <KDE/KDialog>

class QCheckBox;
class KIntSpinBox;
class KLineEdit;

namespace Aki
{
class AddressDialog : public KDialog
{
    Q_OBJECT
public:
    explicit AddressDialog(QWidget* parent = 0);
    ~AddressDialog();
    void setAddress(const QString& name);
    QString address() const;
    void setPassword(const QString& name);
    QString password() const;
    void setPort(quint16 port);
    int port() const;
    void setSsl(bool enabled);
    bool isSslEnabled() const;
private:
    KLineEdit* _serverAddressLineEdit;
    KLineEdit* _serverPasswordLineEdit;
    KIntSpinBox* _serverPortSpinBox;
    QCheckBox* _useSslCheckBox;
}; // End of class AddressDialog.
} // end of namespace Aki.

#endif // AKI_ADDRESSDIALOG_HPP
