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

#include "serverview.h"
#include "akiversion.h"
#include "config/identityconfig.h"
#include "config/serverconfig.h"
#include "dialogs/messagelog.h"
#include "interfaces/maininterface.h"
#include "notifications.h"
#include "ui/basewindow.h"
#include "ui/tabbar.h"
#include "ui/serverwindow.h"
#include <Aki/Irc/Socket>
#include <KDebug>
#include <KIcon>
#include <KLocale>
#include <KMainWindow>
#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QTabBar>
using namespace Aki;

namespace Aki
{
class ServerViewPrivate
{
public:
    ServerViewPrivate(ServerView *qq)
        : q(qq),
        notifications(0),
        window(0)
    {
    }

    void _tabCloseRequested(int index)
    {
        Aki::BaseWindow *window = qobject_cast<Aki::BaseWindow*>(q->widget(index));

        // Only do this if the window is a Server window.
        if (window->windowType() == Aki::BaseWindow::ServerWindow) {
            Aki::ServerWindow *server = qobject_cast<Aki::ServerWindow*>(window);
            server->socket()->rfcQuit(server->identity()->quitMessage());
            server->socket()->disconnectFromHost();
        }

        q->removeServer(index);
    }

    void _closeTabTriggered()
    {
        Aki::BaseWindow *window = qobject_cast<Aki::BaseWindow*>(q->widget(q->currentIndex()));

        if (window->windowType() == Aki::BaseWindow::ServerWindow) {
            Aki::ServerWindow *server = qobject_cast<Aki::ServerWindow*>(window);
            server->socket()->rfcQuit(server->identity()->quitMessage());
            server->socket()->disconnectFromHost();
        }
        q->removeServer(window->name());
    }

    void _currentChanged(int index)
    {
        if (q->count() == 0 || index == -1 || tabList.count() == 0) {
            return;
        }

        for (int i = 0; i < tabList.count(); ++i) {
            if (i == index) {
                tabList[i]->setCurrent(true);

                if (tabList[i]->windowType() == Aki::BaseWindow::ServerWindow) {
                    const QString name = tabList[i]->name();
                    const QString channelName = qobject_cast<Aki::ServerWindow*>(tabList[i])->currentFocusedChannel()->name();
                    window->setWindowTitle(i18n("Aki IRC Client %1 - %3 (%2)", AKI_VERSION_STR, name, channelName));
                }
            } else {
                tabList[i]->setCurrent(false);
            }
        }
    }

    void _tabMoved(int oldIndex, int newIndex)
    {
        qSwap(tabList[oldIndex], tabList[newIndex]);
    }

    void makeCurrent(Aki::BaseWindow *current)
    {
        foreach (Aki::BaseWindow *window, tabList) {
            if (window == current) {
                window->setCurrent(true);
            } else {
                window->setCurrent(false);
            }
        }
    }

    Aki::ServerView *q;
    QList<Aki::BaseWindow*> tabList;
    Aki::Notifications *notifications;
    KMainWindow *window;
    Aki::MessageLog *messageLog;
}; // End of class ServerViewPrivate.
} // End of namespace Aki.

ServerView::ServerView(KMainWindow *window, QWidget *parent)
    : KTabWidget(parent),
    d(new Aki::ServerViewPrivate(this))
{
    Aki::TabBar *bar = new Aki::TabBar(this);
    bar->setDragMode(false);

    d->notifications = new Aki::Notifications(this);
    d->notifications->setWindow(window);
    d->window = window;

    setTabBar(bar);

    connect(this, SIGNAL(tabCloseRequested(int)),
            SLOT(_tabCloseRequested(int)));
    connect(this, SIGNAL(currentChanged(int)),
            SLOT(_currentChanged(int)));
    connect(bar, SIGNAL(tabMoved(int,int)),
            SLOT(_tabMoved(int,int)));

    setTabPosition(North);
    setTabsClosable(true);
    setTabCloseActivatePrevious(true);
    setMovable(true);
}

ServerView::~ServerView()
{
    delete d;
}

void
ServerView::addServer(IdentityConfig *identityConfig, Aki::Irc::Socket *socket)
{
    if (!socket || !identityConfig) {
        return;
    }

    Aki::ServerWindow *window = new Aki::ServerWindow(identityConfig, socket, d->notifications,
                                                      d->messageLog, this);
    window->setCurrent(true);
    connect(window, SIGNAL(customCommand(QString,QString)),
            SIGNAL(customCommand(QString,QString)));
    addTab(window, socket->name());
    d->tabList << window;
    setCurrentIndex(d->tabList.indexOf(window));

    emit serverAdded(socket);
}

void
ServerView::clearAllWindows()
{
}

void
ServerView::removeServer(const QString &name)
{
    if (name.isEmpty() || name.isNull()) {
        return;
    }

    Aki::BaseWindow *window = findWindow(name);
    if (!window) {
        return;
    }

    switch (window->windowType()) {
    case Aki::BaseWindow::ServerWindow: {
        emit serverRemoved(window->socket());
        Aki::ServerWindow *server = qobject_cast<Aki::ServerWindow*>(window);
        Aki::IdentityConfig *config = server->identity();
        window->socket()->rfcQuit(config->quitMessage());
        break;
    }
    default: {
        break;
    }
    }

    const int index = d->tabList.indexOf(window);

    removeTab(index);
    d->tabList.removeOne(window);
}

void
ServerView::removeServer(int index)
{
    removeServer(d->tabList.at(index)->name());
}

Aki::BaseWindow*
ServerView::findWindow(const QString &name)
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

void
ServerView::addCustomWindow(Aki::BaseWindow *window)
{
    if (!window) {
        return;
    }

    addTab(window, window->name());
    d->tabList << window;
}

Aki::BaseWindow*
ServerView::currentWindow()
{
    foreach (Aki::BaseWindow *window, d->tabList) {
        if (window->isCurrent()) {
            return window;
        }
    }

    return 0;
}

const Aki::BaseWindow*
ServerView::currentWindow() const
{
    foreach (const Aki::BaseWindow* const window, d->tabList) {
        if (window->isCurrent()) {
            return window;
        }
    }

    return 0;
}

QList<Aki::BaseWindow*>
ServerView::serverList()
{
    return d->tabList;
}

QList<Aki::BaseWindow*>
ServerView::serverList() const
{
    return d->tabList;
}

void
ServerView::addMessageLog(Aki::MessageLog *messageLog)
{
    d->messageLog = messageLog;
}

#include "serverview.moc"
