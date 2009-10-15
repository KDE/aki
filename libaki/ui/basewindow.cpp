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

BaseWindow::BaseWindow(const QString &name, const Aki::BaseWindow::WindowType &type, QWidget *parent)
    : QWidget(parent),
    m_type(type),
    m_socket(0),
    m_view(0),
    m_notifications(0),
    m_channelView(0),
    m_logFile(0),
    m_name(name)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
}

BaseWindow::~BaseWindow()
{
}

bool
BaseWindow::isCurrent() const
{
    return m_isCurrent;
}

QString
BaseWindow::name() const
{
    return m_name;
}

void
BaseWindow::setCurrent(bool current)
{
    m_isCurrent = current;
}

void
BaseWindow::setName(const QString &name)
{
    m_name = name;
}

void
BaseWindow::setWindowType(const Aki::BaseWindow::WindowType &type)
{
    m_type = type;
}

Aki::BaseWindow::WindowType
BaseWindow::windowType() const
{
    return m_type;
}

void
BaseWindow::setSocket(Aki::Irc::Socket *socket)
{
    m_socket = socket;
}

Aki::Irc::Socket*
BaseWindow::socket()
{
    return m_socket;
}

void
BaseWindow::setView(Aki::ChatView *view)
{
    m_view = view;
}

Aki::ChatView*
BaseWindow::view()
{
    return m_view;
}

void
BaseWindow::setNotifications(Aki::Notifications *notifications)
{
    m_notifications = notifications;
}

Aki::Notifications*
BaseWindow::notifications()
{
    return m_notifications;
}

void
BaseWindow::setChannelView(Aki::ChannelView *view)
{
    m_channelView = view;
}

void
BaseWindow::setTabText(const QString &name)
{
    const int index = m_channelView->indexOf(this);
    m_channelView->setTabText(index, name);
}

void
BaseWindow::setLogFile(Aki::LogFile *logFile)
{
    m_logFile = logFile;
}

Aki::LogFile*
BaseWindow::logFile()
{
    return m_logFile;
}

void
BaseWindow::setTabColor(const Aki::BaseWindow::TabColor &color)
{
    const int currentIndex = m_channelView->indexOf(this);
    const QColor currentColor = m_channelView->tabTextColor(currentIndex);
    const QColor defaultColor = Aki::Settings::defaultTabColor();
    const QColor highlightColor = Aki::Settings::highlightTabColor();
    const QColor newDataColor = Aki::Settings::newDataTabColor();
    const QColor newMessageColor = Aki::Settings::newMessageTabColor();

    if (color == BaseWindow::Default) {
        m_channelView->setTabTextColor(currentIndex, defaultColor);
    } else if (color == BaseWindow::Highlight) {
        m_channelView->setTabTextColor(currentIndex, highlightColor);
    } else if (color == BaseWindow::NewData) {
        if (currentColor == defaultColor) {
            m_channelView->setTabTextColor(currentIndex, newDataColor);
        }
    } else if (color == BaseWindow::NewMessage) {
        if (currentColor != highlightColor) {
            m_channelView->setTabTextColor(currentIndex, newMessageColor);
        }
    }
}
