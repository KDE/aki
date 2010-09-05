#include "mainwindow.hpp"
#include "dialogs/configurationdialog.hpp"
#include "dialogs/identitydialog.hpp"
#include "dialogs/networkdialog.hpp"
#include "dialogs/quickconnectiondialog.hpp"
#include "docks/privatemessages/privatemessagedock.hpp"
#include "interfaces/isettingspage.hpp"
#include "plugin/plugin.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include "ui/serverviewtab.hpp"
#include "ui/systemtray.hpp"
#include "ui/view.hpp"
#include "utils/bookmarkhandler.hpp"
#include "utils/pluginmanager.hpp"
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KActionMenu>
#include <KDE/KCmdLineArgs>
#include <KDE/KNotifyConfigWidget>
#include <KDE/KMenuBar>
#include <KDE/KSelectAction>
#include <KDE/KTabBar>
#include <KDE/KXMLGUIFactory>
#include <QtGui/QListWidget>
using namespace Aki;

AkiWindow::AkiWindow()
    : Aki::MainWindow()
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    args->clear();

    setWindowTitle(i18n("Aki IRC Client"));

    _systemTray = new Aki::SystemTray(this);
    _view = new Aki::View(this);
    setCentralWidget(_view);

#ifdef Q_OS_MAC
    setUnifiedTitleAndToolBarOnMac(true);
#endif

    Aki::ServerViewTab* tab = new Aki::ServerViewTab("Freenode", 0);
    tab->setIcon(KIcon("kde"));
    _view->addWindow(tab);
    Aki::ServerViewTab* tab2 = new Aki::ServerViewTab("EFNet", 0);
    tab2->setIcon(KIcon("amarok"));
    _view->addWindow(tab2);
    Aki::ServerViewTab* tab3 = new Aki::ServerViewTab("Afternet", 0);
    tab3->setIcon(KIcon("aki"));
    _view->addWindow(tab3);

    Aki::PrivateMessageDock* dock = new Aki::PrivateMessageDock(this);
    dock->appendMessage(KDateTime::currentLocalDateTime(), i18n("Test"), i18n("Boner"), i18n("Freenode"));
    dock->appendMessage(KDateTime::currentLocalDateTime(), i18n("Test"), i18n("Boner"), i18n("Freenode"));
    addDock(dock, Qt::BottomDockWidgetArea);

    createMenus();
    createDialogs();
}

AkiWindow::~AkiWindow()
{
}

void
AkiWindow::addDock(Aki::DockWidget* dockWidget, Qt::DockWidgetArea area)
{
    QList<Aki::DockBar*> tbl = dockToolBars();
    foreach (Aki::DockBar* bar, tbl) {
        if (bar->area() == static_cast<Qt::ToolBarArea>(area)) {
            qobject_cast<Aki::DockBar*>(bar)->addDockWidget(dockWidget);
            break;
        }
    }
}

void
AkiWindow::addGui(Aki::Plugin* plugin)
{
    guiFactory()->addClient(plugin);
}

void
AkiWindow::addSettingsPage(Aki::ISettingsPage* page)
{
    Q_ASSERT(page);
    _configDialog->addPage(page);
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

    KActionMenu* bookmarkMenu = new KActionMenu(i18n("Bookmarks"), this);
    actionCollection()->addAction("bookmarks", bookmarkMenu);
    new Aki::BookmarkHandler(bookmarkMenu->menu(), actionCollection(), this);

    action = new KAction(KIcon("user-properties"), i18n("Identities..."), this);
    actionCollection()->addAction("settingsIdentityList", action);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(slotIdentityListTriggered()));

    KStandardAction::showMenubar(this, SLOT(slotShowMenubar()), actionCollection());
    KStandardAction::configureNotifications(this, SLOT(slotConfigureNotifications()), actionCollection());
    KStandardAction::preferences(this, SLOT(slotPreferencesTriggered()), actionCollection());
    setupGUI(Default);
}

void
AkiWindow::createDialogs()
{
    Aki::PluginManager::self()->initialize(this);
    _configDialog = new Aki::ConfigurationDialog(this);
    Aki::PluginManager::self()->loadPlugins();
}

void
AkiWindow::removeDock(Aki::DockWidget* dockWidget)
{
    QList<Aki::DockBar*> tbl = dockToolBars();
    foreach (Aki::DockBar* dockBar, tbl) {
        if (dockBar) {
            QList<QAction*> al = dockBar->actions();
            foreach (QAction* action, al) {
                Aki::DockButton* bar =
                    qobject_cast<Aki::DockButton*>(dockBar->widgetForAction(action));
                if (bar && bar->dockWidget() == dockWidget) {
                    dockBar->removeAction(action);
                    return;
                }
            }
        }
    }
}

void
AkiWindow::removeGui(Aki::Plugin* plugin)
{
    guiFactory()->removeClient(plugin);
}

void
AkiWindow::removeSettingsPage(Aki::ISettingsPage* page)
{
    Q_ASSERT(page);
    _configDialog->removePage(page);
}

void
AkiWindow::slotConfigureNotifications()
{
    KNotifyConfigWidget::configure(this);
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
AkiWindow::slotPreferencesTriggered()
{
    _configDialog->exec();
}

void
AkiWindow::slotShowMenubar()
{
    if (menuBar()->isHidden()) {
        menuBar()->show();
    } else if (menuBar()->isVisible()) {
        menuBar()->hide();
    }
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
