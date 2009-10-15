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

#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include "libaki_export.h"
#include <QWidget>

namespace Aki
{
namespace Irc
{
class Socket;
} // End of namespace Irc.
class Notifications;
class LogFile;
class ChannelView;
class ChatView;
/**
 * Base window class for all the tabs in ServerView and ChannelView.
 * Every class has to inherit this class for it to appear in Aki. The
 * window is deleted own it's own. Should not call delete on it.
 */
class LIBAKI_EXPORT BaseWindow : public QWidget
{
    Q_OBJECT
public:
    /**
     * Determines the type of window this will be.
     */
    enum WindowType {
        /**
         * Server status window. This is the window that is named as
         * the network.
         */
        StatusWindow = 0,
        /**
         * Channel window. This is the window that every channel that
         * you can have conventional with a group of people in.
         */
        ChannelWindow,
        /**
         * Query window. This is the private channel between two users
         * for private conversations.
         */
        QueryWindow,
        /**
         * DCC Chat Window.
         */
        DccChatWindow,
        /**
         * Server window. This is the server tab that hosts the server
         * and the channels for the server.
         */
        ServerWindow,
        /**
         * Other window. This is the window that plugins and scripts
         * use to implement custom windows.
         */
        OtherWindow
    }; // End of enum WindowType.

    enum TabColor {
        Default = 0,
        Highlight,
        NewData,
        NewMessage
    }; // End of enum TabColor.
public:
    /**
     * Creates a BaseWindow as a @p type default OtherWindow.
     * @param name Name of the tab.
     * @param type Window Type (Defaults to OtherWindow).
     * @param parent Parent of the object.
     */
    explicit BaseWindow(const QString &name, const Aki::BaseWindow::WindowType &type = Aki::BaseWindow::OtherWindow, QWidget *parent = 0);
    /**
     * Destroys the BaseWindow.
     */
    virtual ~BaseWindow();
    /**
     * Gets the state of if the channel is the current one.
     */
    virtual bool isCurrent() const;
    /**
     * Gets the name of the window.
     */
    QString name() const;
    /**
     * Sets the window as the @p current current one.
     * @param current true if this window is current; false otherwise.
     */
    virtual void setCurrent(bool current);
    /**
     * Sets the window name @p name.
     * @param name Name of the window.
     */
    void setName(const QString &name);
    /**
     * Sets the type @p type of window this is.
     * @param type Window type.
     */
    void setWindowType(const Aki::BaseWindow::WindowType &type);
    /**
     * Gets the type of window this is.
     * @return Window type.
     */
    Aki::BaseWindow::WindowType windowType() const;
    /**
     * Sets the window socket @p socket.
     * @param socket Windows irc socket.
     */
    void setSocket(Aki::Irc::Socket *socket);
    /**
     * Gets the window socket.
     * @return Window socket.
     */
    Aki::Irc::Socket* socket();
    /**
     * Sets the window chat view @p view. Not every
     * window has one.
     * @param view Windows chatview.
     */
    void setView(Aki::ChatView *view);
    /**
     * Gets the window chat view.
     * @return Window's chatview or 0 if there isn't one.
     */
    Aki::ChatView* view();

    void setNotifications(Aki::Notifications *notification);
    Aki::Notifications* notifications();
    void setChannelView(Aki::ChannelView *view);
    void setTabText(const QString &name);
    virtual bool hasInputFocus() const = 0;
    Aki::LogFile* logFile();
    void setTabColor(const Aki::BaseWindow::TabColor &color);
protected:
    virtual void setLogFile(Aki::LogFile *logFile);
private:
    Aki::BaseWindow::WindowType m_type;
    Aki::Irc::Socket *m_socket;
    Aki::ChatView *m_view;
    Aki::Notifications *m_notifications;
    Aki::ChannelView *m_channelView;
    Aki::LogFile *m_logFile;
    QString m_name;
    bool m_isCurrent;
}; // End of class BaseWindow.
} // End of namespace Aki.

#endif // BASEWINDOW_H
