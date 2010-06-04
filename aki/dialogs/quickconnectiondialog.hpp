#ifndef AKI_QUICKCONNECTIONDIALOG_HPP
#define AKI_QUICKCONNECTIONDIALOG_HPP

#include "ui_quickconnectiondialog.h"
#include <KDE/KDialog>

namespace Aki
{
class QuickConnectionDialog : public KDialog,
                              private Ui::QuickConnection
{
    Q_OBJECT
public:
    explicit QuickConnectionDialog(QWidget* parent = 0);
    ~QuickConnectionDialog();
    bool isSslEnabled() const;
    QString nickname() const;
    QString serverAddress() const;
    QString serverPassword() const;
    quint16 serverPort() const;
    void setNickname(const QString& nickname);
    void setServerAddress(const QString& address);
    void setServerPassword(const QString& password);
    void setServerPort(quint16 port);
    void setSsl(bool enabled);
}; // End of class QuickConnectionDialog.
} // End of namespace Aki.

#endif // AKI_QUICKCONNECTIONDIALOG_HPP
