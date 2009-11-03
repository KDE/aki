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
#include "akiversion.h"
#include "config/identityconfig.h"
#include "dialogs/charselectordialog.h"
#include "dialogs/identitydialog.h"
#include "dialogs/messagelog.h"
#include "dialogs/quickconnectiondialog.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/welcomedialog.h"
#include "interfaces/settingspageinterface.h"
#include "plugin/plugin.h"
#include "plugin/pluginmanager.h"
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
#include <KLocale>
#include <KMenu>
#include <KMenuBar>
#include <KNotifyConfigWidget>
#include <KStandardDirs>
#include <KUniqueApplication>
#include <KXMLGUIFactory>
#include <QCloseEvent>
#include <QDockWidget>
using namespace Aki;

namespace Aki
{
class MainWindowPrivate
{
public:
    MainWindowPrivate(MainWindow *qq)
        : q(qq),
        mainView(0),
        settingsDialog(0),
        messageLog(0)
    {
    }

    void createMenus()
    {
        // Aki Menu
        akiNewServerTab = new KAction(q);
        akiNewServerTab->setText(i18n("New Server Tab"));
        q->actionCollection()->addAction("newServerTab", akiNewServerTab);

        akiQuickConnection = new KAction(q);
        akiQuickConnection->setText(i18n("Quick Connection..."));
        q->actionCollection()->addAction("quickConnection", akiQuickConnection);
        q->connect(akiQuickConnection, SIGNAL(triggered(bool)),
                   SLOT(quickConnectionTriggered()));

        akiReconnection = new KAction(q);
        akiReconnection->setText(i18n("Reconnect"));
        akiReconnection->setIcon(KIcon("edit-redo"));
        q->actionCollection()->addAction("reconnect", akiReconnection);
        q->connect(akiReconnection, SIGNAL(triggered(bool)),
                   SLOT(reconnectionTriggered()));

        akiDisconnection = new KAction(q);
        akiDisconnection->setText(i18n("Disconnect"));
        akiDisconnection->setIcon(KIcon("edit-delete"));
        q->actionCollection()->addAction("disconnect", akiDisconnection);
        q->connect(akiDisconnection, SIGNAL(triggered(bool)),
                   SLOT(disconnectionTriggered()));

        akiQuit = new KAction(q);
        akiQuit->setText(i18nc("Quit application", "&Quit"));
        akiQuit->setIcon(KIcon("application-exit"));
        akiQuit->setShortcut(Qt::CTRL + Qt::Key_Q);
        q->actionCollection()->addAction("quit", akiQuit);
        q->connect(akiQuit, SIGNAL(triggered(bool)),
                   SLOT(quitTriggered()));

        // Aki Window menu
        windowCharSelector = new KAction(q);
        windowCharSelector->setText(i18n("Character Selector..."));
        windowCharSelector->setIcon(KIcon("character-set"));
        windowCharSelector->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_C);
        q->actionCollection()->addAction("charSelector", windowCharSelector);
        q->connect(windowCharSelector, SIGNAL(triggered(bool)),
                   SLOT(charSelectorTriggered()));
        q->connect(Aki::CharSelectorDialog::self(), SIGNAL(charSelected(QChar)),
                   SLOT(charSelected(QChar)));

        windowIdentity = new KAction(q);
        windowIdentity->setText(i18n("Identities..."));
        windowIdentity->setIcon(KIcon("user-properties"));
        q->actionCollection()->addAction("identity", windowIdentity);
        q->connect(windowIdentity, SIGNAL(triggered(bool)),
                   SLOT(identityTriggered()));

        windowUrlWatcher = new KAction(q);
        windowUrlWatcher->setText(i18n("U&RL Watcher..."));
        q->actionCollection()->addAction("urlWatcher", windowUrlWatcher);
        q->connect(windowUrlWatcher, SIGNAL(triggered(bool)),
                   SLOT(urlWatcherTriggered()));

        windowClearAllWindows = new KAction(q);
        windowClearAllWindows->setText(i18n("Clear All Windows"));
        q->actionCollection()->addAction("clearAllWindows", windowClearAllWindows);
        q->connect(windowClearAllWindows, SIGNAL(triggered(bool)),
                   SLOT(clearAllWindowsTriggered()));

        windowMessageLog = new KAction(q);
        windowMessageLog->setText(i18n("Message Log..."));
        q->actionCollection()->addAction("messageLog", windowMessageLog);
        q->connect(windowMessageLog, SIGNAL(triggered(bool)),
                   SLOT(messageLogTriggered()));

        KStandardAction::preferences(q, SLOT(preferencesTriggered()), q->actionCollection());
        KStandardAction::configureNotifications(q, SLOT(configureNotificationsTriggered()),
                                                q->actionCollection());

        q->setupGUI();
    }

    void quickConnectionTriggered()
    {
        Aki::QuickConnectionDialog dlg;

        if (dlg.exec() == KDialog::Accepted) {
            createCustomConnection(dlg.address(), dlg.port(), dlg.nick(), dlg.pass(),
                                   dlg.isSslEnabled());
        }
    }

    void reconnectionTriggered()
    {
        Aki::BaseWindow *window = mainView->currentWindow();
        if (window) {
            switch (window->windowType()) {
            case Aki::BaseWindow::ServerWindow: {
                window->socket()->connectToHost();
                akiDisconnection->setEnabled(true);
                akiReconnection->setEnabled(false);
                break;
            }
            default: {
                break;
            }
            }
        }
    }

    void disconnectionTriggered()
    {
        Aki::BaseWindow *window = mainView->currentWindow();
        if (window) {
            switch (window->windowType()) {
            case Aki::BaseWindow::ServerWindow: {
                window->socket()->disconnectFromHost();
                akiDisconnection->setEnabled(false);
                akiReconnection->setEnabled(true);
                break;
            }
            default: {
                break;
            }
            }
        }
    }

    void quitTriggered()
    {
        foreach (Aki::BaseWindow *window, mainView->serverList()) {
            mainView->removeServer(window->name());
        }
        Aki::PluginManager::self()->unloadPlugins();
        kapp->quit();
    }

    void charSelectorTriggered()
    {
        Aki::CharSelectorDialog::self()->show();
    }

    void identityTriggered()
    {
        Aki::IdentityDialog dlg;
        dlg.exec();
    }

    void urlWatcherTriggered()
    {
    }

    void clearAllWindowsTriggered()
    {
        foreach (Aki::BaseWindow *window, mainView->serverList()) {
            if (window && window->windowType() == Aki::BaseWindow::ServerWindow) {
                Aki::ServerWindow *serverWindow = qobject_cast<Aki::ServerWindow*>(window);
                Q_UNUSED(serverWindow);
                //foreach (Aki::BaseWindow *channels, serverWindow->
            }
        }
    }

    void messageLogTriggered()
    {
        messageLog->show();
    }

    void preferencesTriggered()
    {
        settingsDialog->exec();
    }

    void configureNotificationsTriggered()
    {
        KNotifyConfigWidget::configure(q);
    }

    void createCustomConnection(const QString &address, quint16 port,
                                const QString &nickname, const QString &password,
                                bool ssl)
    {
        Aki::IdentityConfig *config = new Aki::IdentityConfig(q);
        config->setCurrentGroup("Custom Connection");
        config->setAwayNickname(QString("%1|away").arg(nickname));
        QStringList nicks;
        nicks << nickname << (nickname + '_') << (nickname + "__");
        config->setNicknameList(nicks);
        config->setRealName("Aki");

        Aki::Irc::Socket *socket = new Aki::Irc::Socket(address, q);
        socket->setAddressList(QStringList() << address + '/' + QString::number(port));
        socket->setAutoIdentify(false);
        socket->setAutoReconnect(true);
        socket->setIdentName("Aki");
        socket->setNickList(config->nicknameList());
        socket->setRealName(config->realName());
        socket->setRetryAttemptCount(10);
        socket->setRetryInterval(10);
        socket->setSsl(ssl);
        socket->setServicePassword(password);

        Aki::Irc::Ctcp *ctcp = new Aki::Irc::Ctcp(q);
        ctcp->setVersionString(Aki::Settings::ctcpVersionMessage());
        ctcp->blockClientInfo(Aki::Settings::ignoreClientInfo());
        ctcp->blockDcc(Aki::Settings::ignoreDcc());
        ctcp->blockPing(Aki::Settings::ignorePing());
        ctcp->blockSource(Aki::Settings::ignoreSource());
        ctcp->blockTime(Aki::Settings::ignoreTime());
        ctcp->blockUserInfo(Aki::Settings::ignoreUserInfo());
        ctcp->blockVersion(Aki::Settings::ignoreVersion());
        socket->setCtcp(ctcp);

        mainView->addServer(config, socket);
        socket->connectToHost();
    }

    void charSelected(const QChar &ch)
    {
        if (mainView->currentWindow()->windowType() != Aki::BaseWindow::ServerWindow) {
            return;
        }

        Aki::ServerWindow *server = qobject_cast<Aki::ServerWindow*>(mainView->currentWindow());
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

    Aki::MainWindow *q;
    Aki::ServerView *mainView;
    Aki::SettingsDialog *settingsDialog;
    Aki::MessageLog *messageLog;
    KAction *akiNewServerTab;
    KAction *akiQuickConnection;
    KAction *akiReconnection;
    KAction *akiDisconnection;
    KAction *akiQuit;
    KAction *windowCharSelector;
    KAction *windowIdentity;
    KAction *windowUrlWatcher;
    KAction *windowClearAllWindows;
    KAction *windowMessageLog;
}; // End of class MainWindowPrivate.
} // End of namespace Aki.

MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
{
    d.reset(new Aki::MainWindowPrivate(this));
    restoreAkiDefaultWindowTitle();
    d->messageLog = new Aki::MessageLog(this);
    d->createMenus();

    d->mainView = new ServerView(this, this);
    d->mainView->addMessageLog(d->messageLog);
    setCentralWidget(d->mainView);
}

MainWindow::~MainWindow()
{
}

void
MainWindow::addSettingsPage(Aki::SettingsPageInterface *page)
{
    d->settingsDialog->addPage(page);
}

void
MainWindow::removeSettingsPage(Aki::SettingsPageInterface *page)
{
    d->settingsDialog->removePage(page);
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
MainWindow::restoreAkiDefaultWindowTitle()
{
    setWindowTitle(i18n("Aki IRC Client %1", AKI_VERSION_STR));
}

void
MainWindow::setAkiWindowTitle(const QString &serverName)
{
    setWindowTitle(i18n("Aki IRC Client %1 - %2", AKI_VERSION_STR, serverName));
}

void
MainWindow::setAkiWindowTitle(const QString &serverName,
                              const QString &channelName)
{
    setWindowTitle(i18n("Aki IRC Client %1 - %2 (%3)", AKI_VERSION_STR,
                        channelName, serverName));
}

Aki::ServerView*
MainWindow::mainView()
{
    return d->mainView;
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
        d->quitTriggered();
        event->accept();
    }
}

void
MainWindow::loadPlugins()
{
    Aki::PluginManager::self()->init(this);
    d->settingsDialog = new Aki::SettingsDialog(this);
    Aki::PluginManager::self()->loadPlugins();
}

#include "mainwindow.moc"
