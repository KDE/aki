#include "mainwindow.hpp"
#include "aki.hpp"
#include "dialogs/configurationdialog.hpp"
#include "dialogs/identitydialog.hpp"
#include "dialogs/networkdialog.hpp"
#include "dialogs/quickconnectiondialog.hpp"
#include "interfaces/isettingspage.hpp"
#include "plugin/plugin.hpp"
#include "ui/view.hpp"
#include "ui/viewtabbar.hpp"
#include "utils/pluginmanager.hpp"
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KCmdLineArgs>
#include <KDE/KTabBar>
#include <KDE/KXMLGUIFactory>
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

    Aki::PluginManager::self()->initialize(this);
    Aki::PluginManager::self()->loadPlugins();
}

AkiWindow::~AkiWindow()
{
}

void
AkiWindow::addGui(Aki::Plugin* plugin)
{
    guiFactory()->addClient(plugin);
}

void
AkiWindow::addSettingsPage(Aki::ISettingsPage*)
{
#warning Implement addSettingsPage
}

void
AkiWindow::createMenus()
{
    KAction* action = new KAction(KIcon("network-workgroup"), i18n("Network List"), this);
    actionCollection()->addAction("akiNetworkList", action);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(slotNetworkListTriggered()));

    action = new KAction(KIcon("network-connect"), i18n("Quick Connection"), this);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(slotQuickConnectionTriggered()));
    actionCollection()->addAction("akiQuickConnection", action);

    action = new KAction(KIcon("edit-redo"), i18n("Reconnect"), this);
    actionCollection()->addAction("akiReconnect", action);

    action = new KAction(KIcon("edit-delete"), i18n("Disconnect"), this);
    actionCollection()->addAction("akiDisconnect", action);

    action = new KAction(KIcon("application-exit"), i18n("Quit"), this);
    actionCollection()->addAction("akiQuit", action);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(slotQuitTriggered()));

    action = new KAction(KIcon("user-properties"), i18n("Identities..."), this);
    actionCollection()->addAction("settingsIdentityList", action);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(slotIdentityListTriggered()));

    KStandardAction::preferences(this, SLOT(slotPreferencesTriggered()), actionCollection());
    setupGUI(ToolBar | Keys | StatusBar | Create);
}

void
AkiWindow::createDialogs()
{
    _configDialog = new Aki::ConfigurationDialog(this);
}

void
AkiWindow::removeGui(Aki::Plugin* plugin)
{
    guiFactory()->removeClient(plugin);
}

void
AkiWindow::removeSettingsPage(Aki::ISettingsPage*)
{
#warning Implement removeSettingsPage
}

void
AkiWindow::slotIdentityListTriggered()
{
    Aki::IdentityDialog* identityDialog = new Aki::IdentityDialog;
    identityDialog->show();
}

void
AkiWindow::slotNetworkListTriggered()
{
    Aki::NetworkDialog* networkDialog = new Aki::NetworkDialog;
    networkDialog->show();
}

void
AkiWindow::slotQuickConnectionTriggered()
{
    Aki::QuickConnectionDialog* quickConnectionDialog = new Aki::QuickConnectionDialog;
    quickConnectionDialog->show();
}

void
AkiWindow::slotQuitTriggered()
{
}

void
AkiWindow::slotPreferencesTriggered()
{
    _configDialog->exec();
}
