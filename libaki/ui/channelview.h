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

#include "libaki_export.h"
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
class LIBAKI_EXPORT ChannelView : public KTabWidget
{
    Q_OBJECT
public:
    typedef QList<Aki::BaseWindow*> WindowList;
    explicit ChannelView(Aki::IdentityConfig *identityConfig, QWidget *parent = 0);
    explicit ChannelView(Aki::IdentityConfig *identityConfig, Aki::Irc::Socket *socket,
                         Aki::ChatParser *parser, Aki::Notifications *notifications,
                         QWidget *parent = 0);
    ~ChannelView();
    Aki::BaseWindow* findChannel(const QString &name);
    Aki::BaseWindow* currentChannel();
    void setChatParser(Aki::ChatParser *parser);
    Aki::ChannelView::WindowList windows();
    bool isSplitEnabled() const;
    void removeChannel(int index);
    void removeChannel(const QString &name);
    void takeChannel(Aki::BaseWindow *window);
    void addChannel(const QString &name);
    bool addChannel(Aki::BaseWindow *channel);
    void addStatus(const QString &name);
    void addQuery(Aki::Irc::User *self, Aki::Irc::User *other, const QString &message, bool toSelf = false);
    void setSplitEnabled(bool enabled);
    void parserText(const QString &text);
public Q_SLOTS:
    void checkChannelDrop(Aki::BaseWindow *window);
Q_SIGNALS:
    void dropSuccessful(Aki::BaseWindow *window);
    void splitStatusChanged(bool enabled);
    void whoRemoved(const QString &channel);
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
private:
    Q_PRIVATE_SLOT(d, void tabCloseRequested(int index))
    Q_PRIVATE_SLOT(d, void currentChanged(int index))
    Q_PRIVATE_SLOT(d, void tabMoved(int oldIndex, int newIndex))
    Q_PRIVATE_SLOT(d, void customContextMenuRequested())
    Q_PRIVATE_SLOT(d, void splitView())
    Q_PRIVATE_SLOT(d, void closeTab())
    Q_PRIVATE_SLOT(d, void textSubmitted(Aki::BaseWindow *window, const QString &text))
    Q_PRIVATE_SLOT(d, void whoAdded(const QString &channel))
    Q_PRIVATE_SLOT(d, void whoQueueTimeout())
    Q_PRIVATE_SLOT(d, void whoRemoved(const QString &channel))
private:
    friend class ChannelViewPrivate;
    QScopedPointer<ChannelViewPrivate> d;
}; // End of class ChannelView.
} // End of namespace Aki.

#endif // CHANNELVIEW_H