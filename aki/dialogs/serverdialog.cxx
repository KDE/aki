#include "serverdialog.hpp"
#include "aki.hpp"
#include "ui/networklistmodel.hpp"
#include "utils/database.hpp"
#include "utils/sqlidentity.hpp"
#include "utils/sqlserver.hpp"
#include <KDE/KDebug>
#include <KDE/KInputDialog>
#include <KDE/KMessageBox>
#include <QtCore/QTextCodec>
using namespace Aki;

ServerDialog::ServerDialog(QWidget* parent)
    : KDialog(parent)
{
    Aki::Database::open(Aki::databaseFile());
    Aki::Database* db = new Aki::Database(this);
    Q_UNUSED(db);
    //db->createDefaultTables();
    setupDialog();
    setupActions();
    setupIcons();

    QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(identitySelector->currentText()));
    networkList->addIdentity(identity->name());
    networkList->addModel(new Aki::NetworkListModel);
    Aki::SqlServer::List list = Aki::SqlServer::serversForIdentity(identity.data());
    foreach (Aki::SqlServer* server, list) {
        networkList->addNetwork(server);
    }
}

ServerDialog::~ServerDialog()
{
    Aki::Database::close();
}

void
ServerDialog::setupActions()
{
    connect(identitySelector, SIGNAL(identityActivated(Aki::SqlIdentity*)),
            networkList, SLOT(identityActivated(Aki::SqlIdentity*)));
    connect(identitySelector, SIGNAL(identityActivated(Aki::SqlIdentity*)),
            SLOT(slotIdentityActivated(Aki::SqlIdentity*)));
    connect(addNetworkButton, SIGNAL(clicked(bool)),
            SLOT(slotAddNetworkClicked()));
    connect(editNetworkButton, SIGNAL(clicked(bool)),
            SLOT(slotEditNetworkClicked()));
    connect(removeNetworkButton, SIGNAL(clicked(bool)),
            SLOT(slotRemoveNetworkClicked()));
    connect(importNetworksButton, SIGNAL(clicked(bool)),
            SLOT(slotImportNetworksClicked()));
    connect(exportNetworksButton, SIGNAL(clicked(bool)),
            SLOT(slotExportNetworksClicked()));
    connect(retryIntervalSpinBox, SIGNAL(editingFinished()),
            SLOT(slotRetryIntervalEditingFinished()));
    connect(retryAttemptsSpinBox, SIGNAL(editingFinished()),
            SLOT(slotRetryAttemptsEditingFinished()));
    connect(connectOnStartupCheckBox, SIGNAL(clicked(bool)),
            SLOT(slotConnectOnStartupClicked(bool)));
    connect(customEncodingGroupBox, SIGNAL(clicked(bool)),
            SLOT(slotCustomEncodingClicked(bool)));
    connect(encodingComboBox, SIGNAL(activated(QString)),
            SLOT(slotEncodingActivated(QString)));
    connect(serviceNameLineEdit, SIGNAL(editingFinished()),
            SLOT(slotServiceNameEditingFinished()));
    connect(servicePasswordLineEdit, SIGNAL(editingFinished()),
            SLOT(slotServicePasswordEditingFinished()));
}

void
ServerDialog::setupDialog()
{
    QWidget* widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);
    setButtons(User1 | Close);
    setButtonText(User1, i18n("Connect"));
    setButtonIcon(User1, KIcon("network-connect"));
    resize(widget->width(), widget->height());
    enableButton(User1, false);

    /*removeNetworkButton->setDisabled(true);
    editNetworkButton->setDisabled(true);
    exportNetworksButton->setDisabled(true);*/
}

void
ServerDialog::setupIcons()
{
    addNetworkButton->setIcon(KIcon("list-add"));
    removeNetworkButton->setIcon(KIcon("list-remove"));
    editNetworkButton->setIcon(KIcon("edit-rename"));
}

void
ServerDialog::slotIdentityActivated(SqlIdentity* identity)
{
    Q_UNUSED(identity);
    /*if (!networkList->currentNetwork()) {
        removeNetworkButton->setDisabled(true);
        editNetworkButton->setDisabled(true);
    } else {
        removeNetworkButton->setEnabled(true);
        editNetworkButton->setEnabled(true);
    }

    if (networkList->currentModel()->rowCount() == 0) {
        exportNetworksButton->setDisabled(true);
    } else {
        exportNetworksButton->setEnabled(true);
    }*/
}

void
ServerDialog::slotAddNetworkClicked()
{
    bool ok = false;
    const QString network = KInputDialog::getText(i18n("Add New Network"), i18n("Enter a new network name."),
                                                  QString(), &ok, this);
    // Check to see if the network already exists.
    const bool emptyNetwork = networkList->findItems(network, Qt::MatchExactly).isEmpty();
    // Check that the user clicked okay.
    if (ok) {
        if (!emptyNetwork) {
            KMessageBox::error(this, i18n("Unable to add network. Network already exists."));
            // Re show thie dialog.
            slotAddNetworkClicked();
        } else {
            /* Create the new network with the name
            that the user chosed */
            createNewNetwork(network);
        }
    }
}

void
ServerDialog::slotEditNetworkClicked()
{
    bool ok = false;
    // Get the current network.
    Aki::SqlServer* currentNetwork = networkList->currentNetwork();

    // Can we safely remove this check?
    if (!currentNetwork) {
        return;
    }

    const QString network = KInputDialog::getText(i18n("Edit Network"), i18n("Enter a new network name."),
                                                  currentNetwork->name(), &ok, this);
    // Check to see if the network already exists.
    const bool emptyNetwork = networkList->findItems(network, Qt::MatchExactly).isEmpty();
    // Check that the user clicked okay.
    if (ok) {
        if (!emptyNetwork) {
            KMessageBox::error(this, i18n("Unable to edit network. Network already exists."));
            // Re show this dialog.
            slotEditNetworkClicked();
        } else {
            // Change the network name.
            currentNetwork->setName(network);
            // Save it to the database.
            currentNetwork->save();
        }
    }
}

void
ServerDialog::slotExportNetworksClicked()
{
}

void
ServerDialog::slotImportNetworksClicked()
{
}

void
ServerDialog::slotRemoveNetworkClicked()
{
    // Get the current network.
    Aki::SqlServer* currentNetwork = networkList->currentNetwork();

    // Can we safely remove this check?
    if (!currentNetwork) {
        return;
    }

    int result = KMessageBox::warningYesNo(this, i18n("You are about to remove %1. This cannot be undone.",
                                                      currentNetwork->name()),
                                           i18n("Comfirm Network Deletion"));
    switch (result) {
    case KMessageBox::Yes: {
        // Remove the network from the server.
        currentNetwork->remove();
        // Take the network from the network list.
        delete networkList->takeNetwork(networkList->row(currentNetwork));
        break;
    }
    }
}

void
ServerDialog::createNewNetwork(const QString& name)
{
    // Get the current Identity name.
    const QString identityName = identitySelector->currentText();
    // Get the current identity for the name.
    QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(identityName));
    // Get the new server.
    Aki::SqlServer* server = Aki::SqlServer::newServer(name, identity.data());
    // Append it to the network list.
    networkList->addNetwork(server);
}

void
ServerDialog::slotRetryIntervalEditingFinished()
{
    const int value = retryIntervalSpinBox->value();
    Aki::SqlServer* server = networkList->currentNetwork();
    server->setRetryInterval(value);
    server->save();
}

void
ServerDialog::slotRetryAttemptsEditingFinished()
{
    const int value = retryAttemptsSpinBox->value();
    Aki::SqlServer* server = networkList->currentNetwork();
    server->setRetryAttempts(value);
    server->save();
}

void
ServerDialog::slotConnectOnStartupClicked(bool checked)
{
    Aki::SqlServer* server = networkList->currentNetwork();
    server->setConnectOnStartup(checked);
    server->save();
}

void
ServerDialog::slotCustomEncodingClicked(bool checked)
{
    Aki::SqlServer* server = networkList->currentNetwork();
    server->setDefaultEncoding(!checked);
    server->save();
}

void
ServerDialog::slotEncodingActivated(const QString& encoding)
{
    Aki::SqlServer* server = networkList->currentNetwork();
    server->setEncoding(encoding.toLatin1());
    server->save();
}

void
ServerDialog::slotServiceNameEditingFinished()
{
    Aki::SqlServer* server = networkList->currentNetwork();
    server->setServiceName(serviceNameLineEdit->text());
    server->save();
}

void
ServerDialog::slotServicePasswordEditingFinished()
{
    Aki::SqlServer* server = networkList->currentNetwork();
    server->setServicePassword(servicePasswordLineEdit->text());
    server->save();
}
