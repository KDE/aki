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

#ifndef EVENTMONITORDOCKPLUGIN_H
#define EVENTMONITORDOCKPLUGIN_H

#include "plugin/plugin.h"
#include <QVariant>

class EventMonitorDockPluginPrivate;
class EventMonitorDockPlugin : public Aki::Plugin
{
    Q_OBJECT
public:
    EventMonitorDockPlugin(QObject *parent, const QVariantList &args);
    ~EventMonitorDockPlugin();
    virtual bool checkVersion(const QString& version);
    virtual void unload();
    virtual void load();
private:
    Q_PRIVATE_SLOT(d, void serverAdded(Aki::Irc::Socket *socket))
    Q_PRIVATE_SLOT(d, void serverRemoved(Aki::Irc::Socket *socket))
    Q_PRIVATE_SLOT(d, void onPrivmsg(const QString &channel, const Aki::Irc::NickInfo &from,
                                     const Aki::Irc::NickInfo &to, const QString &message))
    Q_PRIVATE_SLOT(d, void onKick(const Aki::Irc::NickInfo &from, const QString &channel,
                                  const QString &nick, const QString &message))
    Q_PRIVATE_SLOT(d, void onNotice(const Aki::Irc::NickInfo &from, const QString &message))
    Q_PRIVATE_SLOT(d, void onCtcpRequest(const Aki::Irc::NickInfo &from, const QString &type))
private:
    friend class EventMonitorDockPluginPrivate;
    EventMonitorDockPluginPrivate* const d;
}; // End of class EventMonitorDockPlugin.

#endif // EVENTMONITORDOCKPLUGIN_H
