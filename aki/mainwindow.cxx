/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "mainwindow.hpp"
#include "dbus.hpp"
#include "dialogs/configurationdialog.hpp"
#include "dialogs/identitydialog.hpp"
#include "dialogs/networkdialog.hpp"
#include "dialogs/quickconnectiondialog.hpp"
#include "docks/privatemessages/privatemessagedock.hpp"
#include "interfaces/isettingspage.hpp"
#include "plugin/plugin.hpp"

#ifdef Q_OS_WIN
#include "ui/blureffect.hpp"
#endif // Q_OS_WIN

#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include "ui/serverviewtab.hpp"
#include "ui/systemtray.hpp"
#include "ui/view.hpp"
#include "utils/bookmarkhandler.hpp"
#include "utils/indicationsystem.hpp"
#include "utils/pluginmanager.hpp"
#include "utils/themestylemanager.hpp"
#include "irc/nickinfo.hpp"
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

    setCaption(i18n("Aki IRC Client"));
    new Aki::DBus(this);

    _systemTray = new Aki::SystemTray(this);
    _view = new Aki::View(this);
    setCentralWidget(_view);

#ifdef Q_OS_WIN
    if (Aki::BlurEffect::isCompositionEnabled()) {
        Aki::BlurEffect::extendFrameIntoClientArea(this);
    }
#endif // Q_OS_WIN

#ifdef Q_OS_MAC
    setUnifiedTitleAndToolBarOnMac(true);
#endif // Q_OS_MAC

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
    addDock(dock, Qt::BottomDockWidgetArea);

    createMenus();
    createDialogs();

    Aki::IndicationSystem::self()->setMainWindow(this);
    Aki::IndicationSystem::self()->addChannelMessage("#Test", "Hello");
    Aki::IndicationSystem::self()->addPrivateMessage(Aki::Irc::NickInfo(""), "hello");
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
AkiWindow::addSettingsPage(Aki::ISettingsPage* page)
{
    Q_ASSERT(page);
    _configDialog->addPage(page);
}

void
AkiWindow::createAction(const QString& actionName, const QString& description, const char* slot,
                        const KIcon& icon)
{
    KAction* action = new KAction(icon, description, this);
    actionCollection()->addAction(actionName, action);
    connect(action, SIGNAL(triggered(bool)), slot);
}

void
AkiWindow::createMenus()
{
    createAction("akiNetworkList", i18n("Network List"), SLOT(slotNetworkListTriggered()), KIcon("network-workgroup"));
    createAction("akiQuickConnection", i18n("Quick Connection"), SLOT(slotQuickConnectionTriggered()),
                 KIcon("network-connect"));
    createAction("akiReconnect", i18n("Reconnect"), 0, KIcon("edit-redo"));
    createAction("akiDisconnect", i18n("Disconnect"), 0, KIcon("edit-delete"));
    createAction("akiQuit", i18n("Quit"), SLOT(slotQuitTriggered()), KIcon("application-exit"));
    createAction("settingsIdentityList", i18n("Identities..."), SLOT(slotIdentityListTriggered()), KIcon("user-properties"));

    KActionMenu* bookmarkMenu = new KActionMenu(i18n("Bookmarks"), this);
    actionCollection()->addAction("bookmarks", bookmarkMenu);
    new Aki::BookmarkHandler(bookmarkMenu->menu(), actionCollection(), this);

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
AkiWindow::removeGui(Aki::Plugin* plugin)
{
    Q_ASSERT(plugin);
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
