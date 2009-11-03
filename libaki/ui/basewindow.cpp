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

#include "basewindow.h"
#include "logfile.h"
#include "notifications.h"
#include "settings.h"
#include "ui/channelview.h"
#include "ui/chatview.h"
#include <Aki/Irc/Socket>
using namespace Aki;

namespace Aki
{
class BaseWindowPrivate
{
public:
    BaseWindowPrivate()
        : type(Aki::BaseWindow::OtherWindow),
        socket(0),
        view(0),
        notifications(0),
        channelView(0),
        logFile(0),
        name(QString()),
        isCurrent(false)
    {
    }

    Aki::BaseWindow::WindowType type;
    Aki::Irc::Socket *socket;
    Aki::ChatView *view;
    Aki::Notifications *notifications;
    Aki::ChannelView *channelView;
    Aki::LogFile *logFile;
    QString name;
    bool isCurrent;
}; // End of class BaseWindowPrivate.
} // End of namespace Aki.

BaseWindow::BaseWindow(const QString &name, const Aki::BaseWindow::WindowType &type, QWidget *parent)
    : QWidget(parent)
{
    d.reset(new Aki::BaseWindowPrivate);
    d->name = name;
    d->type = type;
    setAttribute(Qt::WA_DeleteOnClose, true);
}

BaseWindow::~BaseWindow()
{
}

bool
BaseWindow::isCurrent() const
{
    return d->isCurrent;
}

QString
BaseWindow::name() const
{
    return d->name;
}

void
BaseWindow::setCurrent(bool current)
{
    d->isCurrent = current;
}

void
BaseWindow::setName(const QString &name)
{
    d->name = name;
}

void
BaseWindow::setWindowType(const Aki::BaseWindow::WindowType &type)
{
    d->type = type;
}

Aki::BaseWindow::WindowType
BaseWindow::windowType() const
{
    return d->type;
}

void
BaseWindow::setSocket(Aki::Irc::Socket *socket)
{
    d->socket = socket;
}

Aki::Irc::Socket*
BaseWindow::socket()
{
    return d->socket;
}

void
BaseWindow::setView(Aki::ChatView *view)
{
    d->view = view;
}

Aki::ChatView*
BaseWindow::view()
{
    return d->view;
}

void
BaseWindow::setNotifications(Aki::Notifications *notifications)
{
    d->notifications = notifications;
}

Aki::Notifications*
BaseWindow::notifications()
{
    return d->notifications;
}

void
BaseWindow::setChannelView(Aki::ChannelView *view)
{
    d->channelView = view;
}

void
BaseWindow::setTabText(const QString &name)
{
    const int index = d->channelView->indexOf(this);
    d->channelView->setTabText(index, name);
}

void
BaseWindow::setLogFile(Aki::LogFile *logFile)
{
    d->logFile = logFile;
}

Aki::LogFile*
BaseWindow::logFile()
{
    return d->logFile;
}

void
BaseWindow::setTabColor(const Aki::BaseWindow::TabColor &color)
{
    const int currentIndex = d->channelView->indexOf(this);
    const QColor currentColor = d->channelView->tabTextColor(currentIndex);
    const QColor defaultColor = Aki::Settings::defaultTabColor();
    const QColor highlightColor = Aki::Settings::highlightTabColor();
    const QColor newDataColor = Aki::Settings::newDataTabColor();
    const QColor newMessageColor = Aki::Settings::newMessageTabColor();

    if (color == BaseWindow::Default) {
        d->channelView->setTabTextColor(currentIndex, defaultColor);
    } else if (color == BaseWindow::Highlight) {
        d->channelView->setTabTextColor(currentIndex, highlightColor);
    } else if (color == BaseWindow::NewData) {
        if (currentColor == defaultColor) {
            d->channelView->setTabTextColor(currentIndex, newDataColor);
        }
    } else if (color == BaseWindow::NewMessage) {
        if (currentColor != highlightColor) {
            d->channelView->setTabTextColor(currentIndex, newMessageColor);
        }
    }
}
