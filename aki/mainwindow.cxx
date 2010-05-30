#include "mainwindow.hpp"
#include "aki.hpp"
#include "dialogs/identitydialog.hpp"
#include "dialogs/networkdialog.hpp"
#include "ui/chatview.hpp"
#include "ui/view.hpp"
#include "ui/viewtabbar.hpp"
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KCmdLineArgs>
#include <KDE/KTabBar>
using namespace Aki;

AkiWindow::AkiWindow()
    : Aki::MainWindow()
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    args->clear();
    setTopRightCorner(new Aki::ViewTabBar(this));
    setView(new Aki::View);
    createMenus();
    createDialogs();
}

AkiWindow::~AkiWindow()
{
}

void
AkiWindow::createMenus()
{
    KAction* action = new KAction(i18n("Network List"), this);
    actionCollection()->addAction("akiNetworkList", action);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(serverListTriggered()));

    action = new KAction(i18n("Server Tab"), this);
    actionCollection()->addAction("akiNewServerTab", action);

    action = new KAction(i18n("Channel Tab"), this);
    actionCollection()->addAction("akiNewChannelTab", action);

    action = new KAction(i18n("Quick Connection"), this);
    actionCollection()->addAction("akiQuickConnection", action);

    action = new KAction(KIcon("edit-redo"), i18n("Reconnect"), this);
    actionCollection()->addAction("akiReconnect", action);

    action = new KAction(KIcon("edit-delete"), i18n("Disconnect"), this);
    actionCollection()->addAction("akiDisconnect", action);

    action = new KAction(KIcon("application-exit"), i18n("Quit"), this);
    actionCollection()->addAction("akiQuit", action);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(quitTriggered()));

    action = new KAction(i18n("Identities..."), this);
    actionCollection()->addAction("settingsIdentityList", action);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(identityListTriggered()));
    setupGUI(ToolBar | Keys | StatusBar | Create);
}

void
AkiWindow::createDialogs()
{
}

void
AkiWindow::identityListTriggered()
{
    Aki::IdentityDialog* identityDialog = new Aki::IdentityDialog;
    identityDialog->show();
}

void
AkiWindow::serverListTriggered()
{
    Aki::NetworkDialog* networkDialog = new Aki::NetworkDialog;
    networkDialog->show();
}

void
AkiWindow::quitTriggered()
{
}
