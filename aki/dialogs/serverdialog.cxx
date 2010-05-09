#include "serverdialog.hpp"
#include "aki.hpp"
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
    qDeleteAll(list);
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
            SLOT(slotIdentityActivated(Aki::SqlIdentity*)));
}

void
ServerDialog::setupIcons()
{
}

void
Aki::ServerDialog::slotIdentityActivated(Aki::SqlIdentity* identity)
{
    Q_UNUSED(identity);
}
