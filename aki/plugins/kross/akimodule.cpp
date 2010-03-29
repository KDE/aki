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

#include "akimodule.h"
#include "interfaces/maininterface.h"
#include "interfaces/settingspageinterface.h"
#include "ui/basewindow.h"
#include "ui/channelview.h"
#include "ui/channelwindow.h"
#include "ui/chatview.h"
#include "ui/querywindow.h"
#include "ui/serverwindow.h"
#include "ui/serverview.h"
#include <QDockWidget>
#include <Aki/Irc/Socket>

class AkiModulePrivate
{
public:
    AkiModulePrivate(AkiModule *qq)
        : q(qq)
    {
    }

    AkiModule *q;
    Aki::MainInterface *interface;
}; // End of class AkiModulePrivate.

AkiModule::AkiModule(QObject *parent, Aki::MainInterface *interface)
    : QObject(parent)
{
    setObjectName("Aki");
    d.reset(new AkiModulePrivate(this));
    d->interface = interface;
}

AkiModule::~AkiModule()
{
}

void
AkiModule::addDock(QDockWidget *widget)
{
    d->interface->addDock(widget, Qt::BottomDockWidgetArea, Qt::Horizontal);
}

void
AkiModule::removeDock(QDockWidget *widget)
{
    d->interface->removeDock(widget);
}

void
AkiModule::addSettingsPage(Aki::SettingsPageInterface *page)
{
    d->interface->addSettingsPage(page);
}

void
AkiModule::removeSettingsPage(Aki::SettingsPageInterface *page)
{
    d->interface->removeSettingsPage(page);
}

QObject*
AkiModule::mainView()
{
    return d->interface->mainView();
}

void
AkiModule::executeCommand(const QString &command)
{
    Aki::ServerView *mainView = d->interface->mainView();
    Aki::ServerWindow *serverWindow = qobject_cast<Aki::ServerWindow*>(mainView->currentWindow());

    foreach (Aki::BaseWindow *window, serverWindow->splitView()->windows()) {
        if (window->isCurrent() && window->hasInputFocus()) {
            serverWindow->splitView()->parserText(command);
            return;
        }
    }

    serverWindow->mainView()->parserText(command);
}

void
AkiModule::printText(const QString &str)
{
    Aki::ServerView *mainView = d->interface->mainView();
    Aki::ServerWindow *serverWindow = qobject_cast<Aki::ServerWindow*>(mainView->currentWindow());

    if (serverWindow) {
        Aki::BaseWindow *base = serverWindow->currentFocusedChannel();
        switch (base->windowType()) {
        case Aki::BaseWindow::ChannelWindow: {
            qobject_cast<Aki::ChannelWindow*>(base)->view()->addMessage(str);
            break;
        }
        case Aki::BaseWindow::QueryWindow: {
            qobject_cast<Aki::QueryWindow*>(base)->view()->addMessage(str);
            break;
        }
        default: {
            return;
        }
        }
    }
}

QObject*
AkiModule::socket()
{
    Aki::ServerView *mainView = d->interface->mainView();
    return qobject_cast<Aki::ServerWindow*>(mainView->currentWindow())->socket();
}

#include "akimodule.moc"