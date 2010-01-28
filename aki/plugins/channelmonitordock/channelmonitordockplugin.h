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

#ifndef CHANNELMONITORDOCKPLUGIN_H
#define CHANNELMONITORDOCKPLUGIN_H

#include "plugin/plugin.h"
#include <QVariant>

namespace Aki
{
class ServerView;
namespace Irc
{
class NickInfo;
class Socket;
} // End of namespace Irc.
} // End of namespace Aki.

class ChannelMonitorDock;
class ChannelMonitorDockPlugin : public Aki::Plugin
{
    Q_OBJECT
public:
    ChannelMonitorDockPlugin(QObject *parent, const QVariantList &args);
    ~ChannelMonitorDockPlugin();
    virtual bool checkVersion(const QString& version);
    virtual void unload();
    virtual void load();
private Q_SLOTS:
    void slotServerAdded(Aki::Irc::Socket *socket);
    void slotServerRemoved(Aki::Irc::Socket *socket);
    void slotOnPrivmsg(const QString &channel, const Aki::Irc::NickInfo &from,
                       const Aki::Irc::NickInfo &to, const QString &message);
    void slotOnCtcpAction(const QString &from, const QString &to,
                          const QString &message);
private:
    ChannelMonitorDock *m_channelMonitorDock;
    Aki::ServerView *m_serverView;
}; // End of class ChannelMonitorDockPlugin.

#endif // CHANNELMONITORDOCKPLUGIN_H
