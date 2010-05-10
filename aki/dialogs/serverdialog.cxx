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
    Aki::SqlServer::List list = Aki::SqlServer::serversForIdentity(identity.data());
    foreach (Aki::SqlServer* server, list) {
        networkList->addIdentity(identity->name());
        networkList->addModel(new Aki::NetworkListModel);
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
}

void
ServerDialog::slotEditNetworkClicked()
{
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
}
