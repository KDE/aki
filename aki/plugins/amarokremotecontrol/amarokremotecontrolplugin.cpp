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

#include "amarokremotecontrolplugin.h"
#include "akiversion.h"
#include "amarokconfig.h"
#include "amarokremote.h"
#include "interfaces/maininterface.h"
#include "ui/channelwindow.h"
#include "ui/querywindow.h"
#include "ui/serverwindow.h"
#include "ui/serverview.h"
#include <KLocale>
#include <KGenericFactory>
#include <KMessageBox>
#include <KPluginFactory>
#include <ui/chatview.h>
#include <Aki/Irc/Socket>
#include <ui/statuswindow.h>

K_PLUGIN_FACTORY(AmarokRemoteControlPluginFactory, registerPlugin<AmarokRemoteControlPlugin>();)
K_EXPORT_PLUGIN(AmarokRemoteControlPluginFactory("aki_amarokremotecontrolplugin"));

AmarokRemoteControlPlugin::AmarokRemoteControlPlugin(QObject *parent, const QVariantList &args)
    : Aki::Plugin(parent)
{
    Q_UNUSED(args);
    setComponentData(AmarokRemoteControlPluginFactory::componentData());
    m_remote = new AmarokRemote(this);
    m_config = new AmarokConfig;
}

AmarokRemoteControlPlugin::~AmarokRemoteControlPlugin()
{
}

void
AmarokRemoteControlPlugin::load()
{
    connect(mainInterface()->mainView(), SIGNAL(customCommand(QString,QString)),
            SLOT(slotCustomCommand(QString,QString)));
    mainInterface()->addSettingsPage(m_config);
}

void
AmarokRemoteControlPlugin::unload()
{
    mainInterface()->removeSettingsPage(m_config);
}

bool
AmarokRemoteControlPlugin::checkVersion(const QString &version)
{
    return version == AKI_VERSION_STR;
}

void
AmarokRemoteControlPlugin::slotCustomCommand(const QString &command, const QString &message)
{
    if (command == "amarok") {
        if (message == "exit") {
            m_remote->exit();
        } else if (message == "next") {
            m_remote->nextTrack();
        } else if (message == "pause") {
            m_remote->pause();
        } else if (message == "play") {
            m_remote->play();
        } else if (message == "last") {
            m_remote->previousTrack();
        } else if (message == "repeat on") {
            m_remote->setRepeat(true);
        } else if (message == "repeat off") {
            m_remote->setRepeat(false);
        } else if (message == "status") {
            //m_remote->status();
        } else if (message == "stop") {
            m_remote->stop();
        } else if (message == "display") {
            Aki::ServerWindow *server = qobject_cast<Aki::ServerWindow*>(mainInterface()->mainView()->currentWindow());
            if (server) {
                const QString metaData = m_remote->metaData();
                if (metaData.isEmpty() || metaData.isNull()) {
                    return;
                }

                Aki::BaseWindow *window = server->currentFocusedChannel();

                switch (window->windowType()) {
                case Aki::BaseWindow::ChannelWindow: {
                    Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
                    channel->view()->addCtcpAction(window->socket()->currentNick(), metaData);
                    channel->socket()->rfcCtcpAction(channel->name(), metaData);
                    break;
                }
                case Aki::BaseWindow::QueryWindow: {
                    Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(window);
                    query->view()->addCtcpAction(window->socket()->currentNick(), metaData);
                    query->socket()->rfcCtcpAction(query->name(), metaData);
                    break;
                }
                default: {
                    break;
                }
                }
            }
        } else if (message == "open") {
            m_remote->open();
        }
    }
}
