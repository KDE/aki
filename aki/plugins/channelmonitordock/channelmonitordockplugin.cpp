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

#include "channelmonitordockplugin.h"
#include "akiversion.h"
#include "interfaces/maininterface.h"
#include "channelmonitor.h"
#include "channelmonitordock.h"
#include "ui/channelwindow.h"
#include "ui/serverview.h"
#include "ui/serverwindow.h"
#include <Aki/Irc/Color>
#include <aki/irc/nickinfo.h>
#include <Aki/Irc/Socket>
#include <Aki/Irc/User>
#include <KActionCollection>
#include <KGenericFactory>
#include <KIcon>
#include <KLocale>
#include <KPluginFactory>
#include <QAction>

K_PLUGIN_FACTORY(ChannelMonitorDockPluginFactory, registerPlugin<ChannelMonitorDockPlugin>();)
K_EXPORT_PLUGIN(ChannelMonitorDockPluginFactory("aki_channelmonitordockplugin"));

ChannelMonitorDockPlugin::ChannelMonitorDockPlugin(QObject *parent, const QVariantList &args)
    : Aki::Plugin(parent)
{
    Q_UNUSED(args);
    setComponentData(ChannelMonitorDockPluginFactory::componentData());
}

ChannelMonitorDockPlugin::~ChannelMonitorDockPlugin()
{
}

bool
ChannelMonitorDockPlugin::checkVersion(const QString& version)
{
    return version == AKI_VERSION_STR;
}

void
ChannelMonitorDockPlugin::unload()
{
    mainInterface()->removeDock(m_channelMonitorDock);

    delete m_channelMonitorDock;

    disconnect(m_serverView, SIGNAL(serverAdded(Aki::Irc::Socket*)),
               this, SLOT(slotServerAdded(Aki::Irc::Socket*)));
    disconnect(m_serverView, SIGNAL(serverRemoved(Aki::Irc::Socket*)),
               this, SLOT(slotServerRemoved(Aki::Irc::Socket*)));
}

void
ChannelMonitorDockPlugin::load()
{
    m_channelMonitorDock = new ChannelMonitorDock;

    QAction *channelMonitorAction = m_channelMonitorDock->toggleViewAction();
    actionCollection()->addAction("channelMonitor", channelMonitorAction);
    setXMLFile("aki_channelmonitordockpluginui.rc");
    
    m_serverView = mainInterface()->mainView();

    mainInterface()->addDock(m_channelMonitorDock, Qt::BottomDockWidgetArea, Qt::Horizontal);

    connect(m_serverView, SIGNAL(serverAdded(Aki::Irc::Socket*)),
            SLOT(slotServerAdded(Aki::Irc::Socket*)));
    connect(m_serverView, SIGNAL(serverRemoved(Aki::Irc::Socket*)),
            SLOT(slotServerRemoved(Aki::Irc::Socket*)));
}

void
ChannelMonitorDockPlugin::slotServerAdded(Aki::Irc::Socket *socket)
{
    connect(socket, SIGNAL(onPrivmsg(QString,Aki::Irc::NickInfo,Aki::Irc::NickInfo,QString)),
            SLOT(slotOnPrivmsg(QString,Aki::Irc::NickInfo,Aki::Irc::NickInfo,QString)));
    connect(socket, SIGNAL(onCtcpAction(QString,QString,QString)),
            SLOT(slotOnCtcpAction(QString,QString,QString)));
}

void
ChannelMonitorDockPlugin::slotServerRemoved(Aki::Irc::Socket *socket)
{
    disconnect(socket, SIGNAL(onPrivmsg(QString,Aki::Irc::NickInfo,Aki::Irc::NickInfo,QString)),
               this, SLOT(slotOnPrivmsg(QString,Aki::Irc::NickInfo,Aki::Irc::NickInfo,QString)));
    disconnect(socket, SIGNAL(onCtcpAction(QString,QString,QString)),
               this, SLOT(slotOnCtcpAction(QString,QString,QString)));
}

void
ChannelMonitorDockPlugin::slotOnPrivmsg(const QString &channel, const Aki::Irc::NickInfo &from,
                                        const Aki::Irc::NickInfo &to, const QString &message)
{
    Q_UNUSED(to);

    Aki::Irc::Socket *socket = qobject_cast<Aki::Irc::Socket*>(sender());
    if (!socket) {
        return;
    }

    Aki::ServerWindow *window = qobject_cast<Aki::ServerWindow*>(m_serverView->findWindow(socket->name()));
    if (!window) {
        return;
    }

    Aki::ChannelWindow *channelWin = qobject_cast<Aki::ChannelWindow*>(window->findChannel(channel));
    if (!channelWin) {
        return;
    }

    foreach (Aki::Irc::User *user, channelWin->users()) {
        if (user->nick() == from.nick()) {
            QString matchString = "\\s*" + QRegExp::escape(socket->currentNick()) + ":*\\s*";
            if (message.contains(QRegExp(matchString))) {
                QString colour = QString("<font color='%1'>%2</font>")
                                    .arg(user->color().name(), user->nick());
                m_channelMonitorDock->addMessageHighlight(channel, colour, Aki::Irc::Color::toHtml(message));
            } else {
                QString colour = QString("<font color='%1'>%2</font>")
                                    .arg(user->color().name(), user->nick());
                m_channelMonitorDock->addMessage(channel, colour, Aki::Irc::Color::toHtml(message));
            }
        }
    }
}

void
ChannelMonitorDockPlugin::slotOnCtcpAction(const QString &from, const QString &to,
                                           const QString &message)
{
    Aki::Irc::Socket *socket = qobject_cast<Aki::Irc::Socket*>(sender());
    if (!socket) {
        return;
    }

    Aki::ServerWindow *window = qobject_cast<Aki::ServerWindow*>(m_serverView->findWindow(socket->name()));
    if (!window) {
        return;
    }

    Aki::ChannelWindow *channelWin = qobject_cast<Aki::ChannelWindow*>(window->findChannel(to));
    if (!channelWin) {
        return;
    }

    m_channelMonitorDock->addCtcpAction(channelWin->name(), from, message);
}
