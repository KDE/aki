/*
    This file is part of Aki IRC.
    Copyright 2009 - 2010 Keith Rusler <xzekex@live.co.uk>

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

#include "mediacontrol.h"
#include "akiversion.h"
#include "interfaces/maininterface.h"
#include "mediaconfig.h"
#include "mediaremote.h"
#include "ui/channelwindow.h"
#include "ui/querywindow.h"
#include "ui/serverview.h"
#include "ui/serverwindow.h"
#include <Aki/Irc/Socket>
#include <KPluginFactory>

K_PLUGIN_FACTORY(MediaControlFactory, registerPlugin<MediaControl>();)
K_EXPORT_PLUGIN(MediaControlFactory("akimediacontrolplugin"))

class MediaControlPrivate
{
public:
    MediaControlPrivate(MediaControl *qq)
        : q(qq),
        visualDisplay(false),
        config(0),
        remote(0)
    {
    }

    ~MediaControlPrivate()
    {
    }

    void customCommand(const QString &command, const QString &message)
    {
        if (command.isEmpty() || command.isNull()) {
            return;
        }

        const QString cmd = command.toLower();
        const QString msg = message.toLower();

        if (cmd == "media") {
            if (msg.isNull() || msg.isEmpty()) {
                Aki::ServerWindow *server =
                    qobject_cast<Aki::ServerWindow*>(q->mainInterface()->mainView()->currentWindow());
                if (server) {
                    const QString metaData = remote->metaData();
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
            } else if (message == "exit") {
                remote->exit();
            } else if (message == "next") {
                remote->nextTrack();
            } else if (message == "pause") {
                remote->pause();
            } else if (message == "play") {
                remote->play();
            } else if (message == "last") {
                remote->previousTrack();
            } else if (message == "repeat on") {
                remote->setRepeat(true);
            } else if (message == "repeat off") {
                remote->setRepeat(false);
            } else if (message == "stop") {
                remote->stop();
            } else if (message == "open") {
                remote->open();
            } else if (message == "vdisplay on") {
                visualDisplay = true;
            } else if (message == "vdisplay off") {
                visualDisplay = false;
            } else if (message.contains(QRegExp("^volume ([0-9][0-9]?|100)$"))) {
                remote->setVolume(message.split(' ')[1].toInt());
            } else if (message == "amarok") {
                remote->setPlayer("Amarok");
                config->setPlayerName("Amarok");
            } else if (message == "vlc") {
                remote->setPlayer("VLC");
                config->setPlayerName("VLC");
            }
        }
    }

    void playerChanged(const QString &name)
    {
        remote->setPlayer(name);
        config->setPlayerName(name);
    }

    MediaControl *q;
    bool visualDisplay;
    MediaConfig *config;
    MediaRemote *remote;
}; // End of class MediaControlPrivate.

MediaControl::MediaControl(QObject *parent, const QVariantList &args)
    : Aki::Plugin(parent),
    d(new MediaControlPrivate(this))
{
    Q_UNUSED(args);
    setComponentData(MediaControlFactory::componentData());
    d->config = new MediaConfig;
    d->remote = new MediaRemote;
}

MediaControl::~MediaControl()
{
    delete d;
}

bool
MediaControl::checkVersion(const QString& version)
{
    return version == AKI_VERSION_STR;
}

void
MediaControl::unload()
{
    disconnect(mainInterface()->mainView(), SIGNAL(customCommand(QString,QString)),
               this, SLOT(customCommand(QString,QString)));
    disconnect(d->config, SIGNAL(playerChanged(QString)),
               this, SLOT(playerChanged(QString)));
    mainInterface()->removeSettingsPage(d->config);
}

void
MediaControl::load()
{
    connect(mainInterface()->mainView(), SIGNAL(customCommand(QString,QString)),
            SLOT(customCommand(QString,QString)));
    connect(d->config, SIGNAL(playerChanged(QString)),
            SLOT(playerChanged(QString)));
    mainInterface()->addSettingsPage(d->config);
}

#include "mediacontrol.moc"
