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

#include "eventmonitordock.h"
#include "eventitem.h"
#include <aki/irc/nickinfo.h>
#include <KAction>
#include <KGlobal>
#include <KLocale>
#include <KMenu>
#include <QTreeWidget>

class EventMonitorDockPrivate
{
public:
    EventMonitorDockPrivate(EventMonitorDock *qq)
        : q(qq),
        eventTree(0)
    {
    }

    void customContextMenuRequested(const QPoint &pos)
    {
        Q_UNUSED(pos);
        KMenu *menu = new KMenu;

        KAction *clear = new KAction(i18n("Clear Events"), menu);
        q->connect(clear, SIGNAL(triggered(bool)),
                   SLOT(clearTriggered()));

        menu->addAction(clear);

        if (menu) {
            menu->exec(QCursor::pos());
        }
    }

    void clearTriggered()
    {
        q->clear();
    }

    EventMonitorDock *q;
    QTreeWidget *eventTree;
}; // End of class EventMonitorDockPrivate.

EventMonitorDock::EventMonitorDock(QWidget *parent)
    : QDockWidget(i18n("Event Monitor"), parent)
{
    d.reset(new EventMonitorDockPrivate(this));
    setObjectName("EventMonitorDock");
    d->eventTree = new QTreeWidget(this);
    setWidget(d->eventTree);

    QTreeWidgetItem *header = d->eventTree->headerItem();
    header->setText(0, i18n("Time"));
    header->setText(1, i18n("Event"));
    header->setText(2, i18n("Message"));
    header->setText(3, i18n("From"));
    header->setText(4, i18n("Server"));
    header->setText(5, i18n("Channel"));

    d->eventTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    d->eventTree->setProperty("showDropIndicator", QVariant(false));
    d->eventTree->setAlternatingRowColors(true);
    d->eventTree->setSelectionMode(QAbstractItemView::ContiguousSelection);
    d->eventTree->setIndentation(0);
    d->eventTree->setRootIsDecorated(false);
    d->eventTree->setItemsExpandable(false);
    d->eventTree->setExpandsOnDoubleClick(false);

    d->eventTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(d->eventTree, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customContextMenuRequested(QPoint)));
}

EventMonitorDock::~EventMonitorDock()
{
}

void
EventMonitorDock::clear()
{
    d->eventTree->clear();
}

void
EventMonitorDock::addItem(const EventItem& item)
{
    QTreeWidgetItem *event = new QTreeWidgetItem(EventMonitorDock::CustomType);
    event->setText(0, KGlobal::locale()->formatDateTime(item.time()));

    switch (item.event()) {
    case EventItem::UnknownEvent: {
        event->setText(1, i18n("Unknown"));
        break;
    }
    case EventItem::ConnectEvent: {
        event->setText(1, i18n("Connect"));
        break;
    }
    case EventItem::DisconnectEvent: {
        event->setText(1, i18n("Disconnect"));
        break;
    }
    case EventItem::KickedEvent: {
        event->setText(1, i18n("Kick"));
        break;
    }
    case EventItem::NoticeEvent: {
        event->setText(1, i18n("Notice"));
        break;
    }
    case EventItem::PrivmsgEvent: {
        event->setText(1, i18n("Privmsg"));
        break;
    }
    case EventItem::CtcpPingEvent: {
        event->setText(1, i18n("CTCP-PING"));
        break;
    }
    case EventItem::CtcpVersionEvent: {
        event->setText(1, i18n("CTCP-VERSION"));
        break;
    }
    case EventItem::CtcpClientInfoEvent: {
        event->setText(1, i18n("CTCP-CLIENTINFO"));
        break;
    }
    case EventItem::CtcpUserInfoEvent: {
        event->setText(1, i18n("CTCP-USERINFO"));
        break;
    }
    case EventItem::CtcpDccEvent: {
        event->setText(1, i18n("CTCP-DCC"));
        break;
    }
    case EventItem::CtcpSourceEvent: {
        event->setText(1, i18n("CTCP-SOURCE"));
        break;
    }
    case EventItem::CtcpTimeEvent: {
        event->setText(1, i18n("CTCP-TIME"));
        break;
    }
    case EventItem::HighlightEvent: {
        event->setText(1, i18n("Highlight"));
        break;
    }
    case EventItem::WallOpsEvent: {
        event->setText(1, i18n("WallOps"));
        break;
    }
    }

    event->setText(2, item.message());
    event->setText(3, item.from().nick());
    event->setText(4, item.server());
    event->setText(5, item.channel());
    
    event->setData(0, EventMonitorDock::IrcEventRole, QVariant::fromValue<EventItem>(item));
    d->eventTree->addTopLevelItem(event);
}

void
EventMonitorDock::removeItem(int index)
{
    QTreeWidgetItem *item = d->eventTree->takeTopLevelItem(index);
    delete item;
}

#include "eventmonitordock.moc"
