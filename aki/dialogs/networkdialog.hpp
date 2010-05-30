#ifndef AKI_NETWORKDIALOG_HPP
#define AKI_NETWORKDIALOG_HPP

#include "ui_networkdialog.h"
#include <KDialog>

namespace Aki
{
class SqlIdentity;
class NetworkDialog : public KDialog,
                      private Ui::NetworkDialog
{
    Q_OBJECT
public:
    explicit NetworkDialog(QWidget* parent = 0);
    ~NetworkDialog();
private Q_SLOTS:
    void slotAddAddressClicked();
    void slotAddChannelClicked();
    void slotAddNetworkClicked();
    void slotAutoIdentifiedClicked(bool checked);
    void slotAutoJoinChannelsClicked(bool checked);
    void slotAutoReconnectClicked(bool checked);
    void slotConnectOnStartupClicked(bool checked);
    void slotConnectToRandomServersClicked(bool checked);
    void slotCustomEncodingClicked(bool checked);
    void slotEditAddressClicked();
    void slotEditChannelClicked();
    void slotEditNetworkClicked();
    void slotEncodingActivated(const QString& encoding);
    void slotIdentityActivated(Aki::SqlIdentity* identity);
    void slotNetworkCurrentRowChanged(int currentRow);
    void slotRemoveAddressClicked();
    void slotRemoveChannelClicked();
    void slotRemoveNetworkClicked();
    void slotRetryAttemptsValueChanged(int count);
    void slotRetryIntervalValueChanged(int seconds);
    void slotServiceNameTextEdited(const QString& name);
    void slotServicePasswordTextEdited(const QString& password);
private:
    void createNewNetwork(const QString& name);
    void load();
    void loadNetwork(Aki::SqlNetwork* network);
    void setupActions();
    void setupDialog();
    void setupIcons();
}; // End of class NetworkDialog.
} // End of namespace Aki.

#endif // AKI_NETWORKDIALOG_HPP
