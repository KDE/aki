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

#ifndef SERVERVIEW_H
#define SERVERVIEW_H

#include "libaki_export.h"
#include <KTabWidget>
#include <QScopedPointer>

class KMainWindow;
namespace Aki
{
namespace Irc
{
class Socket;
} // End of namespace Irc.
class MessageLog;
class BaseWindow;
class IdentityConfig;
class ServerViewPrivate;
/**
 * Server view that shows the server and the channels for each server view.
 */
class LIBAKI_EXPORT ServerView : public KTabWidget
{
    Q_OBJECT
public:
    typedef QList<Aki::BaseWindow*> List;
    /**
     * Creates a new ServerView.
     * @param parent Parent of the object.
     */
    ServerView(KMainWindow *window, QWidget *parent = 0);
    /**
     * Destroys the ServerView object.
     */
    ~ServerView();
    /**
     * Adds a new server tab.
     * @param identityConfig Identify configuration.
     * @param socket IRC socket.
     */
    void addServer(Aki::IdentityConfig *identityConfig, Aki::Irc::Socket *socket,
                   const QStringList &channelList);
    /**
     * Clears all the window text for all the windows and the channels they occupy.
     */
    void clearAllWindows();
    /**
     * Removes a server by name @p name.
     * @param name Window name,
     */
    void removeServer(const QString &name);
    /**
     * Removes a server by index @p index.
     * @param index Index of the window.
     */
    void removeServer(int index);
    /**
     * Get the window by name @p name.
     * @param name If window is found it returns the pointer to the window; if
     * window isn't valid returns a 0.
     */
    Aki::BaseWindow* findWindow(const QString &name);
    /**
     * Adds a custom window @p window.
     * @param window Custom window to be displayed.
     */
    void addCustomWindow(Aki::BaseWindow *window);
    /**
     * Gets the current window.
     * @return Current window.
     */
    Aki::BaseWindow* currentWindow();
    /**
     * Gets the current window.
     * @return Current window.
     */
    const Aki::BaseWindow* currentWindow() const;

    QList<Aki::BaseWindow*> serverList();
    QList<Aki::BaseWindow*> serverList() const;

    void addMessageLog(Aki::MessageLog *messageLog);
Q_SIGNALS:
    void serverAdded(Aki::Irc::Socket *socket);
    void serverRemoved(Aki::Irc::Socket *socket);
    void customCommand(const QString &command, const QString &message);
private:
    friend class ServerViewPrivate;
    QScopedPointer<ServerViewPrivate> d;

    Q_PRIVATE_SLOT(d, void _closeTabTriggered());
    Q_PRIVATE_SLOT(d, void _currentChanged(int))
    Q_PRIVATE_SLOT(d, void _tabCloseRequested(int))
    Q_PRIVATE_SLOT(d, void _tabMoved(int, int))
}; // End of class ServerView.
} // End of namespace Aki.

#endif // SERVERVIEW_H
