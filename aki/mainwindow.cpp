/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "mainwindow.h"
#include "akiapplication.h"
#include "akistatusbar.h"
#include "akiversion.h"
#include "config/identityconfig.h"
#include "dbus.h"
#include "dialogs/charselectordialog.h"
#include "dialogs/identitydialog.h"
#include "dialogs/quickconnectiondialog.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/welcomedialog.h"
#include "interfaces/settingspageinterface.h"
#include "plugin/pluginmanager.h"
#include "plugin/plugin.h"
#include "settings.h"
#include "ui/basewindow.h"
#include "ui/channelwindow.h"
#include "ui/querywindow.h"
#include "ui/serverview.h"
#include "ui/serverwindow.h"
#include "ui/statuswindow.h"
#include <Aki/Irc/Ctcp>
#include <Aki/Irc/Socket>
#include <KAction>
#include <KActionCollection>
#include <KDebug>
#include <KDialog>
#include <KLocale>
#include <KMessageBox>
#include <KNotifyConfigWidget>
#include <KApplication>
#include <KStandardDirs>
#include <KXMLGUIFactory>
#include <QCloseEvent>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent),
    m_statusBar(new AkiStatusBar(this)),
    m_settingsDialog(0)
{
    setWindowTitle(i18n("Aki IRC Client %1", AKI_VERSION_STR));
    setStatusBar(m_statusBar);

    createMenus();

    KStandardDirs dirs;
    if (dirs.findResource("config", "akiidentityrc").isEmpty()) {
        WelcomeDialog *dlg = new WelcomeDialog(this);
        dlg->exec();
    }

    m_identityDialog = new IdentityDialog(this);

    m_view = new Aki::ServerView(this);
    setCentralWidget(m_view);

    m_dbus = new DBus(this);
    m_dbus->setServerView(m_view);
}

MainWindow::~MainWindow()
{
    Aki::PluginManager::self()->unloadPlugins();
}

void
MainWindow::addSettingsPage(Aki::SettingsPageInterface *page)
{
    m_settingsDialog->addPage(page);
}

void
MainWindow::removeSettingsPage(Aki::SettingsPageInterface *page)
{
    m_settingsDialog->removePage(page);
}

void
MainWindow::addGui(Aki::Plugin *plugin)
{
    guiFactory()->addClient(plugin);
}

void
MainWindow::removeGui(Aki::Plugin *plugin)
{
    guiFactory()->removeClient(plugin);
}

void
MainWindow::createMenus()
{
    m_akiNewServerTabAction = new KAction(this);
    m_akiNewServerTabAction->setText(i18n("New Server Tab"));
    actionCollection()->addAction("newServerTab", m_akiNewServerTabAction);

    m_akiQuickConnectionAction = new KAction(this);
    m_akiQuickConnectionAction->setText(i18n("Quick Connect..."));
    actionCollection()->addAction("quickConnection", m_akiQuickConnectionAction);
    connect(m_akiQuickConnectionAction, SIGNAL(triggered(bool)),
            SLOT(slotQuickConnection()));

    m_akiReconnectAction = new KAction(this);
    m_akiReconnectAction->setText(i18n("Reconnect"));
    m_akiReconnectAction->setIcon(KIcon("edit-redo"));
    actionCollection()->addAction("reconnect", m_akiReconnectAction);
    connect(m_akiReconnectAction, SIGNAL(triggered(bool)),
            SLOT(slotReconnectTriggered()));

    m_akiDisconnectAction = new KAction(this);
    m_akiDisconnectAction->setText(i18n("Disconnect"));
    m_akiDisconnectAction->setIcon(KIcon("edit-delete"));
    actionCollection()->addAction("disconnect", m_akiDisconnectAction);
    connect(m_akiDisconnectAction, SIGNAL(triggered(bool)),
            SLOT(slotDisconnectTriggered()));

    m_akiQuitAction = new KAction(this);
    m_akiQuitAction->setText(i18nc("Quit application", "&Quit"));
    m_akiQuitAction->setShortcut(Qt::CTRL + Qt::Key_Q);
    m_akiQuitAction->setIcon(KIcon("application-exit"));
    actionCollection()->addAction("quit", m_akiQuitAction);
    connect(m_akiQuitAction, SIGNAL(triggered(bool)),
            SLOT(quitApplication()));

    m_windowCharSelectorAction = new KAction(this);
    m_windowCharSelectorAction->setText(i18n("Character Selector..."));
    m_windowCharSelectorAction->setIcon(KIcon("character-set"));
    m_windowCharSelectorAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_C);
    actionCollection()->addAction("charselector", m_windowCharSelectorAction);
    connect(m_windowCharSelectorAction, SIGNAL(triggered(bool)),
            SLOT(slotCharSelectorDialog()));
    connect(CharSelectorDialog::self(), SIGNAL(charSelected(QChar)),
            SLOT(slotCharSelected(QChar)));

    m_windowIdentityAction = new KAction(this);
    m_windowIdentityAction->setText(i18n("Identities..."));
    m_windowIdentityAction->setIcon(KIcon("user-properties"));
    actionCollection()->addAction("identity", m_windowIdentityAction);
    connect(m_windowIdentityAction, SIGNAL(triggered(bool)),
            SLOT(slotIdentityDialog()));

    m_windowUrlWatcherAction = new KAction(this);
    m_windowUrlWatcherAction->setShortcut(Qt::CTRL + Qt::Key_R);
    m_windowUrlWatcherAction->setText(i18n("U&RL Watcher..."));
    actionCollection()->addAction("urlWatcher", m_windowUrlWatcherAction);

    m_windowClearAllWindowsAction = new KAction(this);
    m_windowClearAllWindowsAction->setText(i18n("Clear All Windows"));
    actionCollection()->addAction("clearAllWindows", m_windowClearAllWindowsAction);

    m_windowMessageLogAction = new KAction(this);
    m_windowMessageLogAction->setText(i18n("Message Log..."));
    actionCollection()->addAction("messageLog", m_windowMessageLogAction);

    KStandardAction::preferences(this, SLOT(slotPreferences()), actionCollection());
    KStandardAction::configureNotifications(this, SLOT(openConfigureNotifications()),
                                            actionCollection());
    setupGUI();
}

AkiStatusBar*
MainWindow::statusBar()
{
    return m_statusBar;
}

void
MainWindow::slotPreferences()
{
    m_settingsDialog->exec();
}

void
MainWindow::openConfigureNotifications()
{
    KNotifyConfigWidget::configure(this);
}

void
MainWindow::slotIdentityDialog()
{
    m_identityDialog->show();
}

Aki::ServerView*
MainWindow::mainView()
{
    return m_view;
}

void
MainWindow::slotCharSelectorDialog()
{
    CharSelectorDialog::self()->show();
}

void
MainWindow::closeEvent(QCloseEvent *event)
{
    if (Aki::Settings::enableSystemTray()) {
        if (Aki::Settings::minimizeToTrayOnClose()) {
            hide();
            event->ignore();
        }
    } else {
        quitApplication();
    }
}

void
MainWindow::slotQuickConnection()
{
    QuickConnectionDialog dlg;

    switch (dlg.exec()) {
    case KDialog::Accepted: {
        Aki::IdentityConfig *config = new Aki::IdentityConfig(this);
        config->setCurrentGroup("Custom Connection");
        config->setAwayNickname(QString("%1|away").arg(dlg.nick()));
        QStringList nicks;
        nicks << dlg.nick() << dlg.nick() + '_' << dlg.nick() + "__";
        config->setNicknameList(nicks);
        config->setRealName("Aki");

        Aki::Irc::Socket *socket = new Aki::Irc::Socket(dlg.address(), this);
        socket->setAddressList(QStringList() << dlg.address());
        socket->setAutoIdentify(false);
        socket->setAutoReconnect(true);
        socket->setIdentName("Aki");
        socket->setNickList(config->nicknameList());
        socket->setRealName(config->realName());
        socket->setRetryAttemptCount(10);
        socket->setRetryInterval(10);
        socket->setSsl(dlg.isSslEnabled());
        socket->setServicePassword(dlg.pass());

        Aki::Irc::Ctcp *ctcp = new Aki::Irc::Ctcp(this);
        ctcp->setVersionString(Aki::Settings::ctcpVersionMessage());
        ctcp->blockClientInfo(Aki::Settings::ignoreClientInfo());
        ctcp->blockDcc(Aki::Settings::ignoreDcc());
        ctcp->blockPing(Aki::Settings::ignorePing());
        ctcp->blockSource(Aki::Settings::ignoreSource());
        ctcp->blockTime(Aki::Settings::ignoreTime());
        ctcp->blockUserInfo(Aki::Settings::ignoreUserInfo());
        ctcp->blockVersion(Aki::Settings::ignoreVersion());
        socket->setCtcp(ctcp);

        mainView()->addServer(config, socket);
        socket->connectToHost();
        break;
    }
    case KDialog::Rejected: {
        return;
        break;
    }
    }
}

void
MainWindow::slotReconnectTriggered()
{
    Aki::BaseWindow *window = mainView()->currentWindow();
    if (window) {
        switch (window->windowType()) {
        case Aki::BaseWindow::ServerWindow: {
            window->socket()->disconnectFromHost();
            window->socket()->connectToHost();
            break;
        }
        default: {
            break;
        }
        }
    }
}

void
MainWindow::slotDisconnectTriggered()
{
    Aki::BaseWindow *window = mainView()->currentWindow();
    if (window) {
        switch (window->windowType()) {
        case Aki::BaseWindow::ServerWindow: {
            window->socket()->disconnectFromHost();
            break;
        }
        default: {
            break;
        }
        }
    }
}

void
MainWindow::quitApplication()
{
    foreach (Aki::BaseWindow *window, mainView()->serverList()) {
        mainView()->removeServer(window->name());
    }
    kapp->quit();
}

void
MainWindow::slotCharSelected(const QChar &ch)
{
    if (mainView()->currentWindow()->windowType() != Aki::BaseWindow::ServerWindow) {
        return;
    }

    Aki::ServerWindow *server = qobject_cast<Aki::ServerWindow*>(mainView()->currentWindow());
    Aki::BaseWindow *window = server->currentFocusedChannel();
    if (window && window->view()) {
        switch (window->windowType()) {
        case Aki::BaseWindow::ChannelWindow: {
            Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
            QString text = channel->chatInput->text();
            text.append(ch);
            channel->chatInput->setText(text);
            break;
        }
        case Aki::BaseWindow::QueryWindow: {
            Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);
            QString text = query->chatInput->text();
            text.append(ch);
            query->chatInput->setText(text);
            break;
        }
        case Aki::BaseWindow::StatusWindow: {
            Aki::StatusWindow *status = qobject_cast<Aki::StatusWindow*>(window);
            QString text = status->chatInput->text();
            text.append(ch);
            status->chatInput->setText(text);
            break;
        }
        default: {
            return;
            break;
        }
        }
    }
}

void
MainWindow::addDock(QDockWidget *dock, Qt::DockWidgetArea area,
                    Qt::Orientation orientation)
{
    addDockWidget(area, dock, orientation);
}

void
MainWindow::removeDock(QDockWidget *dock)
{
    removeDockWidget(dock);
}

void
MainWindow::loadPlugins()
{
    Aki::PluginManager::self()->init(this);
    m_settingsDialog = new SettingsDialog(this);
    Aki::PluginManager::self()->loadPlugins();
}
