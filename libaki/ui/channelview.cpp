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

#include "channelview.h"
#include "chatparser.h"
#include "config/identityconfig.h"
#include "logfile.h"
#include "notifications.h"
#include "settings.h"
#include "ui/basewindow.h"
#include "ui/channelwindow.h"
#include "ui/querywindow.h"
#include "ui/serverwindow.h"
#include "ui/statuswindow.h"
#include "ui/tabbar.h"
#include <aki/irc/nickinfo.h>
#include <Aki/Irc/Socket>
#include <Aki/Irc/User>
#include <KDebug>
#include <KIcon>
#include <KLocale>
#include <KPushButton>
#include <QtGui>
using namespace Aki;

namespace Aki
{
class ChannelViewPrivate
{
public:
    ChannelViewPrivate(Aki::ChannelView *qq)
        : q(qq),
        identity(0),
        socket(0),
        parser(0),
        notifications(0),
        whoQueueTimer(0),
        contextMenu(0),
        splitViewAction(0),
        closeTabAction(0),
        isSplit(false)
    {
        tabList.clear();
        whoQueue.clear();
    }

    void tabCloseRequested(int index)
    {
        Aki::BaseWindow *window = qobject_cast<Aki::BaseWindow*>(q->widget(index));

        switch (window->windowType()) {
        case Aki::BaseWindow::ChannelWindow: {
            Aki::ChannelWindow *channel = qobject_cast<Aki::ChannelWindow*>(window);
            channel->socket()->rfcPart(channel->name(), channel->identity()->partMessage());
            break;
        }
        case Aki::BaseWindow::StatusWindow: {
            Aki::StatusWindow *status = qobject_cast<Aki::StatusWindow*>(window);
            status->socket()->rfcQuit(status->identity()->quitMessage());
            break;
        }
        default: {
            return;
            break;
        }
        }

        if (window->logFile()) {
            window->logFile()->close();
        }

        q->takeChannel(window);
        delete window;

        if (q->count() == 0) {
            q->hide();
        }
    }

    void currentChanged(int index)
    {
        if (q->count() == 0 || tabList.count() == 0) {
            return;
        }

        for (int i = 0; i < tabList.count(); ++i) {
            if (i == index) {
                Aki::BaseWindow *window = tabList[i];
                window->setCurrent(true);
                const QString name = window->name();
                const QString serverName = window->socket()->name();
            } else {
                tabList[i]->setCurrent(false);
            }
        }
    }

    void tabMoved(int oldIndex, int newIndex)
    {
        qSwap(tabList[oldIndex], tabList[newIndex]);
    }

    void buildContextMenu()
    {
        contextMenu = new QMenu(q);
        splitViewAction = new QAction(KIcon("tab-duplicate"), i18n("Split View"), contextMenu);
        splitViewAction->setEnabled((q->count() > 1) && !isSplit);
        q->connect(splitViewAction, SIGNAL(triggered(bool)),
                   q, SLOT(splitView()));

        closeTabAction = new QAction(KIcon("tab-close"), i18n("Close Tab"), contextMenu);
        closeTabAction->setEnabled((q->count() > 1) && !isSplit);
        q->connect(closeTabAction, SIGNAL(triggered(bool)),
                   q, SLOT(closeTab()));

        contextMenu->addAction(splitViewAction);
        contextMenu->addSeparator();
        contextMenu->addAction(closeTabAction);
    }

    void customContextMenuRequested()
    {
        bool enabled = ((q->count() > 1) && !isSplit);
        splitViewAction->setEnabled(enabled);
        closeTabAction->setEnabled(enabled);
        contextMenu->popup(QCursor::pos());
    }

    void splitView()
    {
        const int currentIndex = q->currentIndex();
        Aki::BaseWindow *current = tabList.value(currentIndex);
        q->takeChannel(current);

        QSplitter *splitter = qobject_cast<QSplitter*>(q->parentWidget());
        Aki::ServerWindow *serverWindow = qobject_cast<Aki::ServerWindow*>(splitter->parentWidget());
        serverWindow->createNewView(current);

        isSplit = true;
    }

    void closeTab()
    {
        tabCloseRequested(q->currentIndex());
    }

    void textSubmitted(Aki::BaseWindow *window, const QString &text)
    {
        parser->setWindow(window);
        parser->parse(text);
    }

    void whoAdded(const QString &channel)
    {
        whoQueue.append(channel.toLower());
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(q->findChannel(channel));

        if (window) {
            window->setIsWhoRunning(true);
        }
    }

    void whoQueueTimeout()
    {
        if (!whoQueue.isEmpty()) {
            QString channel = whoQueue.dequeue();
            socket->rfcWho(channel);
            emit q->whoRemoved(channel);
        }
    }

    void whoRemoved(const QString &channel)
    {
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(q->findChannel(channel));
        if (window) {
            window->setIsWhoRunning(false);
        }
    }

    void setTabPosition(int type)
    {
        switch (type) {
        case 0: {
            q->setTabPosition(QTabWidget::North);
            break;
        }
        case 1: {
            q->setTabPosition(QTabWidget::South);
            break;
        }
        case 2: {
            q->setTabPosition(QTabWidget::West);
            break;
        }
        case 3: {
            q->setTabPosition(QTabWidget::East);
            break;
        }
        }
    }

    Aki::ChannelView *q;
    Aki::IdentityConfig *identity;
    Aki::Irc::Socket *socket;
    Aki::ChatParser *parser;
    Aki::Notifications *notifications;
    Aki::ChannelView::WindowList tabList;
    QQueue<QString> whoQueue;
    QTimer *whoQueueTimer;
    QMenu *contextMenu;
    QAction *splitViewAction;
    QAction *closeTabAction;
    bool isSplit;
}; // End of class ChannelViewPrivate.
} // End of namespace Aki.

ChannelView::ChannelView(Aki::IdentityConfig *identityConfig, QWidget *parent)
    : KTabWidget(parent)
{
    d.reset(new Aki::ChannelViewPrivate(this));
    d->identity = identityConfig;

    Aki::TabBar *bar = new Aki::TabBar(this);
    setTabBar(bar);

    tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);

    d->buildContextMenu();
    d->setTabPosition(Aki::Settings::channelTabPosition());

    setTabsClosable(true);
    setTabCloseActivatePrevious(true);
    setMovable(true);
    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);

    connect(this, SIGNAL(tabCloseRequested(int)),
            SLOT(tabCloseRequested(int)));
    connect(this, SIGNAL(currentChanged(int)),
            SLOT(currentChanged(int)));
    connect(tabBar(), SIGNAL(tabMoved(int,int)),
            SLOT(tabMoved(int,int)));
    connect(tabBar(), SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customContextMenuRequested()));

    d->whoQueueTimer = new QTimer(this);
    d->whoQueueTimer->start(25000);
    connect(d->whoQueueTimer, SIGNAL(timeout()),
            SLOT(whoQueueTimeout()));
    connect(this, SIGNAL(whoRemoved(QString)),
            SLOT(whoRemoved(QString)));
}

ChannelView::ChannelView(Aki::IdentityConfig *identityConfig, Aki::Irc::Socket *socket, Aki::ChatParser *parser,
                         Aki::Notifications *notifications, QWidget *parent)
    : KTabWidget(parent)
{
    d.reset(new Aki::ChannelViewPrivate(this));
    d->identity = identityConfig;
    d->socket = socket;
    d->parser = parser;
    d->notifications = notifications;

    Aki::TabBar *bar = new Aki::TabBar(this);
    setTabBar(bar);

    tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);

    d->buildContextMenu();
    d->setTabPosition(Aki::Settings::channelTabPosition());

    setTabsClosable(true);
    setTabCloseActivatePrevious(true);
    setMovable(true);
    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);

    connect(this, SIGNAL(tabCloseRequested(int)),
            SLOT(tabCloseRequested(int)));
    connect(this, SIGNAL(currentChanged(int)),
            SLOT(currentChanged(int)));
    connect(tabBar(), SIGNAL(tabMoved(int,int)),
            SLOT(tabMoved(int,int)));
    connect(tabBar(), SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customContextMenuRequested()));

    d->whoQueueTimer = new QTimer(this);
    d->whoQueueTimer->start(25000);
    connect(d->whoQueueTimer, SIGNAL(timeout()),
            SLOT(whoQueueTimeout()));
    connect(this, SIGNAL(whoRemoved(QString)),
            SLOT(whoRemoved(QString)));

    addStatus(socket->name());
}

ChannelView::~ChannelView()
{
    
}

void
ChannelView::addChannel(const QString &name)
{
    if (name.isEmpty() || name.isNull()) {
        return;
    }

    if (!findChannel(name)) {
        Aki::ChannelWindow *window = new Aki::ChannelWindow(name, d->identity, d->socket);
        window->setChannelView(this);
        window->setNotifications(d->notifications);
        window->setNickList(d->socket->nickList());
        window->setCurrent(true);

        addTab(window, name);

        connect(window, SIGNAL(textSubmitted(Aki::BaseWindow*,QString)),
                SLOT(textSubmitted(Aki::BaseWindow*,QString)));
        connect(window, SIGNAL(whoAdded(QString)),
                SLOT(whoAdded(QString)));

        d->tabList.append(window);
        setCurrentIndex(d->tabList.indexOf(window));
    } else {
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(name));
        window->populateUserList();
        window->resetWho(true);
    }
}

bool
ChannelView::addChannel(Aki::BaseWindow *channel)
{
    Q_ASSERT(channel);

    if (findChannel(channel->name())) {
        return false;
    }

    switch (channel->windowType()) {
    case Aki::BaseWindow::ChannelWindow: {
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(channel);
        d->tabList.append(window);
        addTab(window, window->name());
        break;
    }
    case Aki::BaseWindow::StatusWindow: {
        Aki::StatusWindow *window = qobject_cast<Aki::StatusWindow*>(channel);
        d->tabList.append(window);
        addTab(window, window->name());
        break;
    }
    case Aki::BaseWindow::QueryWindow: {
        Aki::QueryWindow *window = qobject_cast<Aki::QueryWindow*>(channel);
        d->tabList.append(window);
        addTab(window, window->name());
        break;
    }
    default: {
        kError() << "Window type not supported yet!";
        return false;
        break;
    }
    }

    channel->setChannelView(this);
    setCurrentIndex(d->tabList.indexOf(channel));
    return true;
}

void
ChannelView::addQuery(Aki::Irc::User *self, Aki::Irc::User *other, const QString &message, bool toSelf)
{
    if (!self || !other) {
        return;
    }

    if (findChannel(other->nick())) {
        return;
    }

    Aki::QueryWindow *window = new Aki::QueryWindow(other->nick(), d->socket);
    window->setChannelView(this);
    window->setCurrent(true);
    window->setNotifications(d->notifications);
    window->setSelfUser(self);
    window->setOtherUser(other);

    if (!message.isEmpty() || !message.isNull()) {
        window->addMessage(toSelf ? self->nick() : other->nick(), message);
        if (toSelf) {
            window->socket()->rfcPrivmsg(other->nick(), message);
        }
    }

    addTab(window, other->nick());

    connect(window, SIGNAL(textSubmitted(Aki::BaseWindow*,QString)),
            SLOT(textSubmitted(Aki::BaseWindow*,QString)));

    d->tabList.append(window);
    setCurrentIndex(d->tabList.indexOf(window));
}

void
ChannelView::addStatus(const QString &name)
{
    if (name.isEmpty() || name.isNull()) {
        return;
    }

    if (findChannel(name)) {
        return;
    }

    Aki::StatusWindow *window = new Aki::StatusWindow(name, d->identity, d->socket);
    window->setCurrent(true);
    window->setChannelView(this);
    window->setNotifications(d->notifications);
    window->setNickList(d->socket->nickList());

    addTab(window, name);

    connect(window, SIGNAL(textSubmitted(Aki::BaseWindow*,QString)),
            SLOT(textSubmitted(Aki::BaseWindow*,QString)));

    d->tabList.append(window);
    setCurrentIndex(d->tabList.indexOf(window));
}

void
ChannelView::checkChannelDrop(BaseWindow *window)
{
    if (d->tabList.contains(window)) {
        takeChannel(window);

        if (d->tabList.count() == 0) {
            emit splitStatusChanged(false);
            hide();
        }
    }
}

BaseWindow*
ChannelView::currentChannel()
{
    foreach (Aki::BaseWindow *window, d->tabList) {
        if (window->isCurrent()) {
            return window;
        }
    }

    return 0;
}

void
ChannelView::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *data = event->mimeData();
    const QStringList formats = data->formats();

    if (formats.contains("application/aki-tab")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();

        QByteArray itemData = event->mimeData()->data("application/aki-tab");
        QDataStream stream(&itemData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_6);

        quint64 data;
        stream >> data;

        Aki::BaseWindow *window = reinterpret_cast<Aki::BaseWindow*>(data);
        emit dropSuccessful(window);
        if (addChannel(window)) {
            kDebug() << QString("%1 successfully added %2").arg(objectName(), window->name());
        } else {
            return;
        }

        if (count() == 0) {
            setSplitEnabled(true);
        }
    } else {
        setSplitEnabled(true);
    }

    KTabWidget::dragEnterEvent(event);
}

BaseWindow*
ChannelView::findChannel(const QString &name)
{
    if (name.isEmpty() || name.isNull()) {
        return 0;
    }

    foreach (Aki::BaseWindow *window, d->tabList) {
        if (window->name().toLower() == name.toLower()) {
            return window;
        }
    }

    return 0;
}

bool
ChannelView::isSplitEnabled() const
{
    return d->isSplit;
}

void
ChannelView::removeChannel(int index)
{
    if (index == -1) {
        return;
    }

    removeChannel(d->tabList.at(index)->name());
}

void
ChannelView::removeChannel(const QString &name)
{
    if (name.isEmpty() || name.isNull()) {
        return;
    }

    Aki::BaseWindow *window = findChannel(name);
    if (!window) {
        return;
    }

    const int index = d->tabList.indexOf(window);

    removeTab(index);
    delete d->tabList.takeAt(index);

    if (count() == 0) {
        setSplitEnabled(false);
    }
}

void
ChannelView::setChatParser(ChatParser *parser)
{
    d->parser = parser;
}

void
ChannelView::setSplitEnabled(bool enabled)
{
    d->isSplit = enabled;
}

void
ChannelView::takeChannel(Aki::BaseWindow *window)
{
    if (!window) {
        return;
    }

    const int index = d->tabList.indexOf(window);
    if (index == -1) {
        return;
    }

    d->tabList.removeAt(index);
    removeTab(index);

    if (count() == 0) {
        setSplitEnabled(false);
    }
}

ChannelView::WindowList
ChannelView::windows()
{
    return d->tabList;
}

#include "channelview.moc"
