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
#include <Aki/Irc/Socket>
#include <Aki/Irc/User>
#include <KDebug>
#include <KIcon>
#include <KLocale>
#include <KPushButton>
#include <QDragEnterEvent>
#include <QMenu>
#include <QQueue>
#include <QTimer>
using namespace Aki;

namespace Aki
{
class ChannelViewPrivate
{
public:
    ChannelViewPrivate(ChannelView *qq)
        : q(qq),
        identity(0),
        socket(0),
        isSplit(false)
    {
        whoQueue.clear();
    }

    void _tabCloseRequested(int index)
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
            break;
        }
        }

        if (window->logFile()) {
            window->logFile()->close();
        }
        q->removeChannel(index);

        if (q->count() == 0) {
            q->hide();
        }
    }

    void _currentChanged(int index)
    {
        if (q->count() == 0 || index == -1 || tabList.count() == 0) {
            return;
        }

        for (int i = 0; i < tabList.count(); ++i) {
            if (i == index) {
                tabList[i]->setCurrent(true);
                const QString name = tabList[i]->name();
                const QString serverName = tabList[i]->socket()->name();
            } else {
                tabList[i]->setCurrent(false);
            }
        }
    }

    void _tabMoved(int oldIndex, int newIndex)
    {
        qSwap(tabList[oldIndex], tabList[newIndex]);
    }

    Aki::BaseWindow* findChannel(const QString &name)
    {
        if (name.isEmpty() || name.isNull()) {
            return 0;
        }

        foreach (Aki::BaseWindow *window, tabList) {
            if (window->name().toLower() == name.toLower()) {
                return window;
            }
        }

        return 0;
    }

    bool containsChannel(const QString &name)
    {
        if (name.isEmpty() || name.isNull()) {
            return false;
        }

        foreach (Aki::BaseWindow *window, tabList) {
            if (window->name().toLower() == name.toLower()) {
                return true;
            }
        }

        return false;
    }

    void _customContextMenuRequested(const QPoint &pos)
    {
        Q_UNUSED(pos);

        QMenu *menu = new QMenu(q);

        QAction *splitView = new QAction(menu);
        splitView->setIcon(KIcon("tab-duplicate"));
        splitView->setText(i18n("Split View"));
        splitView->setEnabled((q->count() > 1 && !isSplit) ? true : false);
        QObject::connect(splitView, SIGNAL(triggered(bool)),
                         q, SLOT(_splitView()));

        QAction *closeTab = new QAction(menu);
        closeTab->setIcon(KIcon("tab-close"));
        closeTab->setShortcut(Qt::CTRL + Qt::Key_W);
        closeTab->setText(i18n("Close Tab"));
        closeTab->setEnabled((q->count() > 1) ? true : false);
        QObject::connect(closeTab, SIGNAL(triggered(bool)),
                         q, SLOT(_closeTab()));

        menu->addAction(splitView);
        menu->addSeparator();
        menu->addAction(closeTab);
        menu->popup(QCursor::pos());
    }

    void _splitView()
    {
        Aki::BaseWindow *currentWindow = tabList.value(q->currentIndex());
        q->removeChannel(q->currentIndex());

        QSplitter *splitter = qobject_cast<QSplitter*>(q->parentWidget());
        qobject_cast<Aki::ServerWindow*>(splitter->parentWidget())->createNewView(currentWindow);

        tabList.value(q->currentIndex())->setCurrent(true);
        isSplit = true;
    }

    void _closeTab()
    {
        Aki::BaseWindow *window = qobject_cast<Aki::BaseWindow*>(q->widget(q->currentIndex()));

        switch (window->windowType()) {
        case Aki::BaseWindow::ChannelWindow: {
            window->socket()->rfcPart(window->name(), identity->partMessage());
            break;
        }
        default: {
            break;
        }
        }

        if (window->logFile()) {
            window->logFile()->close();
        }

        q->removeChannel(window->name());

        if (q->count() == 0) {
            q->hide();
        }
    }

    void _textSubmitted(Aki::BaseWindow *window, const QString &text)
    {
        parser->setWindow(window);
        parser->parse(text);
    }

    void whoAdded(const QString &channel)
    {
        whoQueue.append(channel.toLower());
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(channel.toLower()));
        if (window) {
            window->setIsWhoRunning(true);
        }
    }

    void whoRemoved(const QString &channel)
    {
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(channel.toLower()));
        if (window) {
            window->setIsWhoRunning(false);
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


    ChannelView *q;
    Aki::IdentityConfig *identity;
    Aki::Irc::Socket *socket;
    Aki::ChatParser *parser;
    Aki::Notifications *notifications;
    ChannelView::WindowList tabList;
    QQueue<QString> whoQueue;
    QTimer *whoQueueTimer;
    bool isSplit;
}; // End of class ChannelViewPrivate;
} // End of namespace Aki.

ChannelView::ChannelView(Aki::IdentityConfig *identityConfig, QWidget *parent)
    : KTabWidget(parent)
{
    d.reset(new Aki::ChannelViewPrivate(this));

    Aki::TabBar *bar = new Aki::TabBar(this);
    d->identity = identityConfig;

    setTabBar(bar);
    tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);

    switch (Aki::Settings::channelTabPosition()) {
    case 0: {
        setTabPosition(North);
        break;
    }
    case 1: {
        setTabPosition(South);
        break;
    }
    case 2: {
        setTabPosition(West);
        break;
    }
    case 3: {
        setTabPosition(East);
        break;
    }
    }

    setTabsClosable(true);
    setTabCloseActivatePrevious(true);
    setMovable(true);
    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);

    connect(this, SIGNAL(tabCloseRequested(int)),
            SLOT(_tabCloseRequested(int)));
    connect(this, SIGNAL(currentChanged(int)),
            SLOT(_currentChanged(int)));
    connect(tabBar(), SIGNAL(tabMoved(int,int)),
            SLOT(_tabMoved(int,int)));
    connect(tabBar(), SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(_customContextMenuRequested(QPoint)));

    d->whoQueueTimer = new QTimer(this);
    d->whoQueueTimer->start(25000);
    connect(d->whoQueueTimer, SIGNAL(timeout()),
            SLOT(whoQueueTimeout()));
    connect(this, SIGNAL(whoRemoved(QString)),
            SLOT(whoRemoved(QString)));
}

ChannelView::ChannelView(Aki::IdentityConfig *identityConfig, Aki::Irc::Socket *socket,
                         Aki::ChatParser *parser, Aki::Notifications *notification, QWidget *parent)
    : KTabWidget(parent)
{
    d.reset(new Aki::ChannelViewPrivate(this));

    Aki::TabBar *bar = new Aki::TabBar(this);
    d->identity = identityConfig;
    d->socket = socket;
    d->parser = parser;
    d->notifications = notification;

    setTabBar(bar);
    tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);

    switch (Aki::Settings::channelTabPosition()) {
    case 0: {
        setTabPosition(North);
        break;
    }
    case 1: {
        setTabPosition(South);
        break;
    }
    case 2: {
        setTabPosition(West);
        break;
    }
    case 3: {
        setTabPosition(East);
        break;
    }
    }

    setTabsClosable(true);
    setTabCloseActivatePrevious(true);
    setMovable(true);
    setAcceptDrops(true);

    connect(this, SIGNAL(tabCloseRequested(int)),
            SLOT(_tabCloseRequested(int)));
    connect(this, SIGNAL(currentChanged(int)),
            SLOT(_currentChanged(int)));
    connect(tabBar(), SIGNAL(tabMoved(int,int)),
            SLOT(_tabMoved(int,int)));
    connect(tabBar(), SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(_customContextMenuRequested(QPoint)));

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

        if (count() == 0) {
            setSplitEnabled(true);
        }

        addChannel(window);
    } else {
        event->ignore();
    }

    KTabWidget::dragEnterEvent(event);
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

    Aki::BaseWindow *window = d->findChannel(name);
    if (!window) {
        return;
    }

    const int index = d->tabList.indexOf(window);

    removeTab(index);
    d->tabList.removeOne(window);

    if (count() == 0) {
        setSplitEnabled(false);
    }
}

void
ChannelView::addChannel(const QString &name)
{
    if (name.isEmpty() || name.isNull()) {
        return;
    }

    if (!d->findChannel(name)) {
        Aki::ChannelWindow *window = new Aki::ChannelWindow(name, d->identity, d->socket, this);
        window->setChannelView(this);
        window->setNotifications(d->notifications);
        window->setNickList(d->socket->nickList());
        window->setCurrent(true);

        addTab(window, name);

        connect(window, SIGNAL(textSubmitted(Aki::BaseWindow*,QString)),
                SLOT(_textSubmitted(Aki::BaseWindow*,QString)));
        connect(window, SIGNAL(whoAdded(QString)),
                SLOT(whoAdded(QString)));

        d->tabList << window;
        setCurrentIndex(d->tabList.indexOf(window));
    } else {
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(findChannel(name));
        window->populateUserList();
        window->resetWho(true);
    }
}

void
ChannelView::addChannel(Aki::BaseWindow *channel)
{
    Q_ASSERT(channel);

    if (d->findChannel(channel->name())) {
        return;
    }

    switch (channel->windowType()) {
    case Aki::BaseWindow::ChannelWindow: {
        Aki::ChannelWindow *window = qobject_cast<Aki::ChannelWindow*>(channel);
        addTab(window, window->name());
        break;
    }
    case Aki::BaseWindow::StatusWindow: {
        Aki::StatusWindow *status = qobject_cast<Aki::StatusWindow*>(channel);
        addTab(status, status->name());
        break;
    }
    case Aki::BaseWindow::QueryWindow: {
        Aki::QueryWindow *query = qobject_cast<Aki::QueryWindow*>(channel);
        addTab(query, query->name());
        break;
    }
    default: {
        kError() << "Window type not supported yet!";
        return; 
        break;
    }
    }

    channel->setChannelView(this);
    d->tabList << channel;
    setCurrentIndex(d->tabList.indexOf(channel));
}

void
ChannelView::checkChannelDrop(Aki::BaseWindow *window)
{
    if (d->tabList.contains(window)) {
        removeChannel(d->tabList.indexOf(window));

        if (d->tabList.count() == 0) {
            emit splitStatusChanged(false);
            hide();
        }
    }
}

void
ChannelView::addStatus(const QString &name)
{
    if (name.isEmpty() || name.isNull()) {
        return;
    }

    if (d->findChannel(name)) {
        return;
    }

    Aki::StatusWindow *window = new Aki::StatusWindow(name, d->identity, d->socket, this);
    window->setCurrent(true);
    window->setChannelView(this);
    window->setNotifications(d->notifications);
    window->setNickList(d->socket->nickList());
    addTab(window, name);

    connect(window, SIGNAL(textSubmitted(Aki::BaseWindow*,QString)),
            SLOT(_textSubmitted(Aki::BaseWindow*,QString)));

    d->tabList << window;
    setCurrentIndex(d->tabList.indexOf(window));
}

void
ChannelView::addQuery(Aki::Irc::User *self, Aki::Irc::User *other, const QString &message,
                      bool toSelf)
{
    if (!self || !other) {
        return;
    }

    if (d->findChannel(other->nick())) {
        return;
    }

    Aki::QueryWindow *window = new Aki::QueryWindow(other->nick(), d->socket, this);
    window->setChannelView(this);
    window->setCurrent(true);
    window->setNotifications(d->notifications);
    window->setSelfUser(self);
    window->setOtherUser(other);

    if (!message.isEmpty() && !message.isNull()) {
        window->addMessage(toSelf ? self->nick() : other->nick(), message);
        if (toSelf) {
            window->socket()->rfcPrivmsg(other->nick(), message);
        }
    }

    addTab(window, other->nick());

    connect(window, SIGNAL(textSubmitted(Aki::BaseWindow*,QString)),
            SLOT(_textSubmitted(Aki::BaseWindow*,QString)));

    d->tabList << window;
    setCurrentIndex(d->tabList.indexOf(window));
}

Aki::StatusWindow*
ChannelView::statusWindow()
{
    foreach (Aki::BaseWindow *window, d->tabList) {
        if (window->windowType() == Aki::BaseWindow::StatusWindow) {
            return qobject_cast<Aki::StatusWindow*>(window);
        }
    }
    return 0;
}

Aki::BaseWindow*
ChannelView::findChannel(const QString &name)
{
    return d->findChannel(name);
}

Aki::BaseWindow*
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
ChannelView::setChatParser(Aki::ChatParser *parser)
{
    d->parser = parser;
}

QList<Aki::BaseWindow*>
ChannelView::windows()
{
    return d->tabList;
}

void
ChannelView::setSplitEnabled(bool enabled)
{
    d->isSplit = enabled;
}

bool
ChannelView::isSplitEnabled() const
{
    return d->isSplit;
}

#include "channelview.moc"
