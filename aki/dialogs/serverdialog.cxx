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
    db->createDefaultTables();
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
}

void
ServerDialog::setupDialog()
{
    QWidget* widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);
    setButtons(User1 | Close);
    setButtonText(User1, i18n("Connect"));
    setButtonIcon(User1, KIcon(QLatin1String("network-connect")));
    resize(widget->width(), widget->height());
    enableButton(User1, false);

    connect(identitySelector, SIGNAL(identityActivated(Aki::SqlIdentity*)),
            networkList, SLOT(identityActivated(Aki::SqlIdentity*)));
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
}

void
ServerDialog::setupIcons()
{
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
            slotAddNetworkClicked();
        } else {
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
            slotEditNetworkClicked();
        } else {
            currentNetwork->setName(network);
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
        currentNetwork->remove();
        networkList->takeNetwork(networkList->row(currentNetwork));
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
