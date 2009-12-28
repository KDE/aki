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

#ifndef CHANNELVIEW_H
#define CHANNELVIEW_H

#include <KTabWidget>
#include <QScopedPointer>

namespace Aki
{
namespace Irc
{
class Socket;
class User;
} // End of namespace Irc.
class Notifications;
class BaseWindow;
class StatusWindow;
class IdentityConfig;
class ChatParser;
class ChannelViewPrivate;
/**
 * Displays the IRC Channels in a TabWidget view. ChannelView
 * supports dragging and dropping of channels from one to another,
 * tab moving by click on a tab and moving it to another position.
 */
class ChannelView : public KTabWidget
{
    Q_OBJECT
public:
    typedef QList<Aki::BaseWindow*> WindowList;
    /**
     * Creates a new ChannelView that doesn't support sockets. Each ChannelWindow
     * will be used for socket.
     * @param identityConfig Identity configuration file.
     * @param parent Parent of the object.
     */
    ChannelView(Aki::IdentityConfig *identityConfig, QWidget *parent = 0);
    /**
     * Creates a new ChannelView support sockets.
     * @param identityConfig Identity configuration file.
     * @param socket IRC socket.
     * @param parent Parent of the object.
     */
    ChannelView(Aki::IdentityConfig *identityConfig, Aki::Irc::Socket *socket,
                Aki::ChatParser *parser, Aki::Notifications *notification, QWidget *parent = 0);
    /**
     * Destroys the object.
     */
    ~ChannelView();
    /**
     * Get the status window for the ChannelView.
     * @return Gets the status window of the ChannelView. If the ChannelView
     * doesn't contain one it will return 0.
     */
    Aki::StatusWindow* statusWindow();
    /**
     * Gets the window by a name @p name.
     * @param name Channel name.
     * @return Channel window or 0 if Channel is not found.
     */
    Aki::BaseWindow* findChannel(const QString &name);
    /**
     * Gets the current channel.
     * @return Current channel or 0 if there is no current channel.
     */
    Aki::BaseWindow* currentChannel();

    void setChatParser(Aki::ChatParser *parser);

    QList<Aki::BaseWindow*> windows();

    bool isSplitEnabled() const;
public Q_SLOTS:
    /**
     * Removes a channel by index @p index.
     * @param index Index of channel.
     */
    void removeChannel(int index);
    /**
     * Removes a channel by name @p name.
     * @param name Name of the channel.
     */
    void removeChannel(const QString &name);
    /**
     * Adds a new channel with name @p name. If channel is already added. This
     * function will do nothing.
     * @param name Name of channel to be created.
     */
    void addChannel(const QString &name);
    /**
     * Adds a new channel. This doesn't check for an existing channel.
     * @param channel Channel window.
     */
    void addChannel(Aki::BaseWindow *channel);
    /**
     * Checks if the window @p window being dropped is valid. This function
     * is used internally.
     * @internal
     * @param window Window being dropped.
     */
    void checkChannelDrop(Aki::BaseWindow *window);
    /**
     * Adds the server status window by name @p name.
     * @param name Name of the status window.
     */
    void addStatus(const QString &name);

    void addQuery(Aki::Irc::User *self, Aki::Irc::User *other, const QString &message, bool toSelf = false);

    void setSplitEnabled(bool enabled);
Q_SIGNALS:
    /**
     * Emitted when the window @p window is dropped to this
     * view is successful.
     * @param window Window that was dropped on to this window.
     */
    void dropSuccessful(Aki::BaseWindow *window);

    void splitStatusChanged(bool enabled);
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
private:
    Q_PRIVATE_SLOT(d, void _tabCloseRequested(int index))
    Q_PRIVATE_SLOT(d, void _currentChanged(int index))
    Q_PRIVATE_SLOT(d, void _tabMoved(int oldIndex, int newIndex))
    Q_PRIVATE_SLOT(d, void _customContextMenuRequested(const QPoint &pos))
    Q_PRIVATE_SLOT(d, void _splitView())
    Q_PRIVATE_SLOT(d, void _closeTab())
    Q_PRIVATE_SLOT(d, void _textSubmitted(Aki::BaseWindow *window, const QString &text))
private:
    friend class ChannelViewPrivate;
    QScopedPointer<ChannelViewPrivate> d;
}; // End of class ChannelView.
} // End of namespace Aki.

#endif // CHANNELVIEW_H
