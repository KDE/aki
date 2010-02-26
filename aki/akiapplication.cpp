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

#include "akiapplication.h"
#include "config/identityconfig.h"
#include "config/serverconfig.h"
#include "libaki/dialogs/welcomedialog.h"
#include "ui/mainwindow.h"
#include "settings.h"
#include "ui/serverview.h"
#include "ui/systemtray.h"
#include <Aki/Irc/Socket>
#include <KCmdLineArgs>
#include <KMessageBox>
#include <KStandardDirs>
#include <KWindowSystem>
#include <QTimer>

AkiApplication::AkiApplication()
    : KUniqueApplication(),
    m_mainWindow(0)
{
}

AkiApplication::~AkiApplication()
{
}

int
AkiApplication::newInstance()
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    KUrl url;

    if (args->count() > 0) {
        url = args->arg(0);
    }

    if (!m_mainWindow) {
        KStandardDirs dirs;
        if (dirs.findResource("config", "akiidentityrc").isEmpty()) {
            Aki::WelcomeDialog dlg;
            dlg.exec();
        }

        m_mainWindow = new Aki::MainWindow;
        m_mainWindow->hide();

        m_tray = new Aki::SystemTray(m_mainWindow);
        Aki::Settings::enableSystemTray() ? m_tray->show() : m_tray->hide();

        if (Aki::Settings::enableSystemTray() && m_tray->isVisible()) {
            if (!Aki::Settings::minimizeToTrayOnStartup()) {
                m_mainWindow->show();
            }
        }

        m_mainWindow->loadPlugins();
        loadConfigurations(m_mainWindow);
    }

    if (!url.isEmpty()) {
        loadUrlArgument(url, m_mainWindow);
    }

    KWindowSystem::forceActiveWindow(m_mainWindow->winId());
    args->clear();

    return 0;
}

void
AkiApplication::loadConfigurations(Aki::MainWindow *window)
{
    Aki::IdentityConfig *identityConfig = new Aki::IdentityConfig(window);
    Aki::ServerConfig *serverConfig = new Aki::ServerConfig(window);

    QStringListIterator identityIter(identityConfig->groupList());
    while (identityIter.hasNext()) {
        const QString identityName = identityIter.next();
        Aki::IdentityConfig *ident = new Aki::IdentityConfig(window);
        ident->setCurrentGroup(identityName);

        QStringListIterator networkIter(serverConfig->groupList());
        while (networkIter.hasNext()) {
            QString serverName = networkIter.next();
            if (serverName.contains(identityName + QChar('-'))) {
                serverConfig->setCurrentGroup(serverName);
                serverName.remove(identityName + QChar('-'));

                Aki::Irc::Socket *socket = new Aki::Irc::Socket(serverName, window->mainView());
                socket->setAddressList(serverConfig->addressList());
                socket->setAutoIdentify(serverConfig->isAutoIdentifyEnabled());
                socket->setAutoReconnect(serverConfig->isAutoReconnectionEnabled());

                if (serverConfig->isDefaultEncodingEnabled()) {
                    socket->setEncoding("UTF-8");
                } else {
                    socket->setEncoding(serverConfig->encoding().toLatin1());
                }

                socket->setIdentName("aki");
                socket->setNickList(ident->nicknameList());
                socket->setRealName(ident->realName());
                socket->setRetryAttemptCount(serverConfig->retryAttemptCount());
                socket->setRetryInterval(serverConfig->retryInterval());
                socket->setServiceName(serverConfig->serviceName());
                socket->setServicePassword(serverConfig->servicePassword());
                socket->setSsl(serverConfig->isSslEnabled());

                if (serverConfig->isConnectOnStartupEnabled()) {
                    ident->setName(identityName);
                    window->mainView()->addServer(ident, socket, serverConfig->channelList());
                    socket->connectToHost();
                }
            }
        }
    }
}

void
AkiApplication::loadUrlArgument(const KUrl &url, Aki::MainWindow *window)
{
    if (url.protocol().toLower() == "irc" ||
        url.protocol().toLower() == "ircs" ||
        url.protocol().toLower() == "irc6")
    {
        Aki::IdentityConfig *identityConfig = new Aki::IdentityConfig(this);
        Aki::ServerConfig *serverConfig = new Aki::ServerConfig(this);
        identityConfig->setCurrentGroup("Custom Connection");
        serverConfig->setCurrentGroup("Custom Server");

        Aki::Irc::Socket *socket = new Aki::Irc::Socket(url.host(), window->mainView());
        socket->setAddressList(QStringList() << (url.host()));
        socket->setAutoIdentify(serverConfig->isAutoIdentifyEnabled());
        socket->setAutoReconnect(serverConfig->isAutoReconnectionEnabled());
        socket->setEncoding(serverConfig->encoding().toLatin1());
        socket->setIdentName("aki");
        socket->setNickList(identityConfig->nicknameList());
        socket->setRealName(identityConfig->realName());
        socket->setRetryAttemptCount(serverConfig->retryAttemptCount());
        socket->setRetryInterval(serverConfig->retryInterval());
        socket->setServiceName(serverConfig->serviceName());
        socket->setServicePassword(serverConfig->servicePassword());

        if (url.protocol().toLower() == "ircs") {
            socket->setSsl(true);
        } else {
            socket->setSsl(false);
        }

        QStringList channels;

        window->mainView()->addServer(identityConfig, socket, channels);
        socket->connectToHost();
    }
}
