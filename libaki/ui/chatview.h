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

#ifndef CHATVIEW_H
#define CHATVIEW_H

#include "libaki_export.h"
#include <QScopedPointer>
#include <QWebView>

namespace Aki
{
class LogFile;
class ChatViewPrivate;
class LIBAKI_EXPORT ChatView : public QWebView
{
    Q_OBJECT
public:
    ChatView(QWidget *parent = 0);
    ~ChatView();
    void addAway(const QString &message);
    void addAway(const QString &nick, const QString &message);
    void addBan(const QString &nick, const QString &mask);
    void addBanList(const QString &channel, const QString &mask, const QString &who,
                    const QString &time);
    void addChannelCreated(const QString &time);
    void addChannelModes(const QString &modes);
    void addChannelModes(const QString &from, const QString &modes);
    void addChannelUrl(const QString &url);
    void addCtcp(const QString &from, const QString &message);
    void addCtcpAction(const QString &from, const QString &message);
    void addCtcpRequest(const QString &from, const QString &type);
    void addError(const QString &message);
    void addError(const QString &type, const QString &message);
    void addHelp(const QString &message);
    void addInvite(const QString &nick, const QString &channel);
    void addIsOn(const QStringList &nicks);
    void addLUser(const QString &message);
    void addMessage(const QString &message);
    void addChannelMode(const QString &fromNick, const QChar &mode, const QString &params = QString(),
                        bool self = false);
    void addMode(const QString &fromNick, const QString &toNick, const QChar &mode,
                 bool toYou = false, bool fromYou = false);
    void addMotd(const QString &message);
    void addNick(const QString &oldNick, const QString &newNick);
    void addNotice(const QString &message);
    void addNotice(const QString &from, const QString &message);
    void addNoticeAuth(const QString &message);
    void addPrivmsg(const QString &from, const QString &message);
    void addPrivmsgHighlight(const QString &from, const QString &message);
    void addRemoveBan(const QString &nick, const QString &mask);
    void addRemoveMode(const QString &fromNick, const QChar &mode, bool self = false);
    void addRemoveMode(const QString &fromNick, const QString &toNick, const QChar &mode,
                       bool toYou = false, bool fromYou = false);
    void addSelfUModeSet(const QString &modes);
    void addSupport(const QString &message);
    void addTime(const QString &server, const QString &time);
    void addTopic(const QString &topic);
    void addTopicChanged(const QString &nick, const QString &topic);
    void addTopicSetBy(const QString &nick, const QString &time);
    void addUMode(const QString &modes);
    void addUMode(const QString &nick, const QString &modes);
    void addUserHost(const QStringList &nicks, const QStringList &hosts);
    void addUserJoin(const QString &channel, const QString &nick, const QString &hostMask);
    void addUserPart(const QString &channel, const QString &nick, const QString &hostMask,
                     const QString &message);
    void addUserQuit(const QString &nick, const QString &hostMask, const QString &message);
    void addVersion(const QString &message);
    void addWho(const QString &channel, const QString &message);
    void addWho(const QString &channel, const QString &username, const QString &address,
                const QString &server, const QString &nick, const QString &flags, int hops,
                const QString &realName);
    void addWhoIs(const QString &nick, const QString &info);
    void addWhoIsChannels(const QString &nick, const QString &channels);
    void addWhoIsIdentified(const QString &nick, const QString &info);
    void addWhoIsIdle(const QString &nick, const QString &idleTime, const QString &signOn);
    void addWhoIsServer(const QString &nick, const QString &server, const QString &info);
    void addWhoIsUser(const QString &nick, const QString &username, const QString &address,
                      const QString &info);
    void addWhoWas(const QString &nick, const QString &message);
    void addWhoWasUser(const QString &nick, const QString &username, const QString &address,
                       const QString &info);
    void addWelcome(const QString &message);
    void add901(const QString &id, const QString &identName, const QString &address,
                const QString &message);
    void addLogLine(const QString &line);
    void setLog(Aki::LogFile *logFile);
    void clear();
    void insertMarker();
    void clearMarker();
    void addKick(const QString &from, const QString &channel, const QString &nick,
                 const QString &message, bool toYou = false, bool fromYou = false);
Q_SIGNALS:
    void userUrlClicked(const QString &nick);
    void findTextTriggered();
private:
    Q_PRIVATE_SLOT(d, void contentsSizeChanged(const QSize &size))
    Q_PRIVATE_SLOT(d, void linkClicked(const QUrl &url))
    Q_PRIVATE_SLOT(d, void customContextMenuRequested(const QPoint &pos))
    Q_PRIVATE_SLOT(d, void copyUrlTriggered())
    Q_PRIVATE_SLOT(d, void saveAsTriggered())
    Q_PRIVATE_SLOT(d, void copyTriggered())
    Q_PRIVATE_SLOT(d, void findTextTriggered())
private:
    friend class ChatViewPrivate;
    QScopedPointer<ChatViewPrivate> d;
}; // End of class ChatView.
} // End of namespace Aki.

#endif // CHATVIEW_H
