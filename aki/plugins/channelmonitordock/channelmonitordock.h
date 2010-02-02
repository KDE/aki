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

#ifndef CHANNELMONITORDOCK_H
#define CHANNELMONITORDOCK_H

#include <QDockWidget>

class ChannelMonitorDockPrivate;
class ChannelMonitorDock : public QDockWidget
{
    Q_OBJECT
public:
    ChannelMonitorDock(QWidget *parent = 0);
    ~ChannelMonitorDock();
    void addMessage(const QString &channel, const QString &from, const QString &message);
    void addMessageHighlight(const QString &channel, const QString &from,
                             const QString &message);
    void addCtcpAction(const QString &channel, const QString &from, const QString &message);
private:
    friend class ChannelMonitorDockPrivate;
    ChannelMonitorDockPrivate* const d;
};

#endif // CHANNELMONITORDOCK_H
