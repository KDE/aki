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

#include "eventmonitordockplugin.h"
#include "akiversion.h"
#include "eventitem.h"
#include "eventmonitordock.h"
#include "interfaces/maininterface.h"
#include "ui/channelwindow.h"
#include "ui/serverwindow.h"
#include "ui/serverview.h"
#include <aki/irc/nickinfo.h>
#include <Aki/Irc/Socket>
#include <KActionCollection>
#include <KPluginFactory>

class EventMonitorDockPluginPrivate
{
public:
    EventMonitorDockPluginPrivate(EventMonitorDockPlugin *qq)
        : q(qq),
        serverView(0),
        dock(0)
    {
    }

    void serverAdded(Aki::Irc::Socket *socket)
    {
        q->connect(socket, SIGNAL(onPrivmsg(QString,Aki::Irc::NickInfo,Aki::Irc::NickInfo,QString)),
                   SLOT(onPrivmsg(QString,Aki::Irc::NickInfo,Aki::Irc::NickInfo,QString)));
        q->connect(socket, SIGNAL(onKick(Aki::Irc::NickInfo,QString,QString,QString)),
                   SLOT(onKick(Aki::Irc::NickInfo,QString,QString,QString)));
        q->connect(socket, SIGNAL(onNotice(Aki::Irc::NickInfo,QString)),
                   SLOT(onNotice(Aki::Irc::NickInfo,QString)));
        q->connect(socket, SIGNAL(onCtcpRequest(Aki::Irc::NickInfo,QString)),
                   SLOT(onCtcpRequest(Aki::Irc::NickInfo,QString)));
    }

    void serverRemoved(Aki::Irc::Socket *socket)
    {
        q->disconnect(socket, SIGNAL(onPrivmsg(QString,Aki::Irc::NickInfo,Aki::Irc::NickInfo,QString)),
                      q, SLOT(onPrivmsg(QString,Aki::Irc::NickInfo,Aki::Irc::NickInfo,QString)));
        q->disconnect(socket, SIGNAL(onKick(Aki::Irc::NickInfo,QString,QString,QString)),
                      q, SLOT(onKick(Aki::Irc::NickInfo,QString,QString,QString)));
        q->disconnect(socket, SIGNAL(onNotice(Aki::Irc::NickInfo,QString)),
                      q, SLOT(onNotice(Aki::Irc::NickInfo,QString)));
        q->disconnect(socket, SIGNAL(onCtcpRequest(Aki::Irc::NickInfo,QString)),
                      q, SLOT(onCtcpRequest(Aki::Irc::NickInfo,QString)));
    }

    void onCtcpRequest(const Aki::Irc::NickInfo &from, const QString &type)
    {
        Aki::Irc::Socket *socket = qobject_cast<Aki::Irc::Socket*>(q->sender());
        if (!socket) {
            return;
        }

        EventItem event;
        event.setChannel("");
        event.setFrom(from);
        event.setServer(socket->name());
        
        const QString tmp = type.toLower();

        if (tmp == "clientinfo") {
            event.setMessage(i18n("%1 wants to know your client capabilities", from.nick()));
            event.setEvent(EventItem::CtcpClientInfoEvent);
        } else if (tmp == "ping") {
            event.setMessage(i18n("%1 pinged you", from.nick()));
            event.setEvent(EventItem::CtcpPingEvent);
        } else if (tmp == "source") {
            event.setMessage(i18n("%1 wanted your clients source", from.nick()));
            event.setEvent(EventItem::CtcpSourceEvent);
        } else if (tmp == "time") {
            event.setMessage(i18n("%1 wanted your timezone info", from.nick()));
            event.setEvent(EventItem::CtcpTimeEvent);
        } else if (tmp == "userinfo") {
            event.setMessage(i18n("%1 wanted your custom info", from.nick()));
            event.setEvent(EventItem::CtcpUserInfoEvent);
        } else if (tmp == "version") {
            event.setMessage(i18n("%1 wanted your clients version", from.nick()));
            event.setEvent(EventItem::CtcpVersionEvent);
        } else {
            event.setMessage(i18n("%1 wanted an unknown CTCP", from.nick()));
            event.setEvent(EventItem::UnknownEvent);
        }

        if (dock) {
            dock->addItem(event);
        }
    }

    void onPrivmsg(const QString &channel, const Aki::Irc::NickInfo &from, const Aki::Irc::NickInfo &to,
                   const QString &message)
    {
        Aki::Irc::Socket *socket = qobject_cast<Aki::Irc::Socket*>(q->sender());
        if (!socket) {
            return;
        }

        if (to.nick().toLower() == socket->currentNick().toLower()) {
            EventItem event;
            event.setChannel("");
            event.setEvent(EventItem::PrivmsgEvent);
            event.setFrom(from);
            event.setMessage(message);
            event.setServer(socket->name());

            if (dock) {
                dock->addItem(event);
            }
            return;
        } else {
            QString matchString = "\\s*" + QRegExp::escape(socket->currentNick()) + ":*\\s*";
            if (message.contains(QRegExp(matchString))) {
                EventItem event;
                event.setChannel(channel);
                event.setEvent(EventItem::HighlightEvent);
                event.setFrom(from);
                event.setMessage(message);
                event.setServer(socket->name());

                if (dock) {
                    dock->addItem(event);
                }
            }
        }
    }

    void onKick(const Aki::Irc::NickInfo &from, const QString &channel, const QString &nick,
                const QString &message)
    {
        Q_UNUSED(nick);
        Aki::Irc::Socket *socket = qobject_cast<Aki::Irc::Socket*>(q->sender());
        if (!socket) {
            return;
        }

        EventItem event;
        event.setChannel(channel);
        event.setEvent(EventItem::KickedEvent);
        event.setFrom(from);
        event.setMessage(message);
        event.setServer(socket->name());

        if (dock) {
            dock->addItem(event);
        }
    }

    void onNotice(const Aki::Irc::NickInfo &from, const QString &message)
    {
        Aki::Irc::Socket *socket = qobject_cast<Aki::Irc::Socket*>(q->sender());
        if (!socket) {
            return;
        }

        EventItem event;
        event.setChannel("");
        event.setEvent(EventItem::NoticeEvent);
        event.setFrom(from);
        event.setMessage(message);
        event.setServer(socket->name());

        if (dock) {
            dock->addItem(event);
        }
    }

    EventMonitorDockPlugin *q;
    Aki::ServerView *serverView;
    EventMonitorDock *dock;
}; // End of class EventMonitorDockPluginPrivate.

K_PLUGIN_FACTORY(EventMonitorDockPluginFactory, registerPlugin<EventMonitorDockPlugin>();)
K_EXPORT_PLUGIN(EventMonitorDockPluginFactory("aki_eventmonitordockplugin"))

EventMonitorDockPlugin::EventMonitorDockPlugin(QObject* parent, const QVariantList& args)
    : Aki::Plugin(parent),
    d(new EventMonitorDockPluginPrivate(this))
{
    Q_UNUSED(args);
    setComponentData(EventMonitorDockPluginFactory::componentData());
}

EventMonitorDockPlugin::~EventMonitorDockPlugin()
{
    delete d;
}

bool
EventMonitorDockPlugin::checkVersion(const QString& version)
{
    return version == AKI_VERSION_STR;
}

void
EventMonitorDockPlugin::unload()
{
    mainInterface()->removeDock(d->dock);

    delete d->dock;

    disconnect(d->serverView, SIGNAL(serverAdded(Aki::Irc::Socket*)),
               this, SLOT(slotServerAdded(Aki::Irc::Socket*)));
    disconnect(d->serverView, SIGNAL(serverRemoved(Aki::Irc::Socket*)),
               this, SLOT(slotServerRemoved(Aki::Irc::Socket*)));
}

void
EventMonitorDockPlugin::load()
{
    d->dock = new EventMonitorDock;

    QAction *eventMonitorAction = d->dock->toggleViewAction();
    actionCollection()->addAction("event", eventMonitorAction);
    setXMLFile("aki_eventmonitordockpluginui.rc");

    d->serverView = mainInterface()->mainView();

    mainInterface()->addDock(d->dock, Qt::BottomDockWidgetArea, Qt::Vertical);

    connect(d->serverView, SIGNAL(serverAdded(Aki::Irc::Socket*)),
            SLOT(serverAdded(Aki::Irc::Socket*)));
    connect(d->serverView, SIGNAL(serverRemoved(Aki::Irc::Socket*)),
            SLOT(serverRemoved(Aki::Irc::Socket*)));
}

#include "eventmonitordockplugin.moc"
