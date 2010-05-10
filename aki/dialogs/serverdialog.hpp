#ifndef SERVERDIALOG_HPP
#define SERVERDIALOG_HPP

#include "server.hpp"
#include "ui_serverdialog.h"
#include <KDE/KDialog>

namespace Aki
{
class ServerDialog : public KDialog,
                     private Ui::ServerDialog
{
    Q_OBJECT
public:
    explicit ServerDialog(QWidget* parent = 0);
    virtual ~ServerDialog();
    void setupActions();
    void setupDialog();
    void setupIcons();
private Q_SLOTS:
    void slotIdentityActivated(Aki::SqlIdentity* identity);
    void slotAddNetworkClicked();
    void slotRemoveNetworkClicked();
    void slotEditNetworkClicked();
    void slotImportNetworksClicked();
    void slotExportNetworksClicked();
    void slotRetryIntervalEditingFinished();
    void slotRetryAttemptsEditingFinished();
    void slotConnectOnStartupClicked(bool checked);
    void slotCustomEncodingClicked(bool checked);
    void slotEncodingActivated(const QString& encoding);
    void slotServiceNameEditingFinished();
    void slotServicePasswordEditingFinished();
private:
    void createNewNetwork(const QString& name);
}; // End of class ServerDialog.
} // End of namespace Aki.

#endif // SERVERDIALOG_HPP
