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

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <KStatusBar>

class QLabel;
/**
 * Status bar for Aki IRC. Adds information about the current
 * server, channel, and anything else.
 */
class AkiStatusBar : public KStatusBar
{
    Q_OBJECT
public:
    /**
     * Creates a status bar.
     * @param parent Parent of the object.
     */
    AkiStatusBar(QWidget *parent = 0);
    /**
     * Changes the channel name @p name.
     * @param name Channel name.
     */
    void updateChannelName(const QString &name);
    /**
     * Changes the lag meter count @p count.
     * @param count Lag count in ms.
     */
    void updateLagMeter(quint16 count);
    /**
     * Changes the Server name @p name.
     * @param name Server name.
     */
    void updateServerName(const QString &name);
    /**
     * Changes the channel user count.
     * @param ops Operator count.
     * @param halfOps Half operator count.
     * @param voices Voices count.
     * @param total Total user count.
     */
    void updateUserCount(int ops, int halfOps, int voices, int total);
    /**
     * Hides the channel information.
     */
    void hideChannelName();
    /**
     * Hides the lag meter.
     */
    void hideLagMeter();
    /**
     * Hides the server name,
     */
    void hideServerName();
    /**
     * Hides user count.
     */
    void hideUserCount();
    /**
     * Shows the channel name.
     */
    void showChannelName();
    /**
     * Shows the lag meter.
     */
    void showLagMeter();
    /**
     * Shows the server name.
     */
    void showServerName();
    /**
     * Shows the user count.
     */
    void showUserCount();
public Q_SLOTS:
    /**
     * Shows a message @p message in the status bar.
     * @param message Message to show.
     */
    void showMessage(const QString &message);
private:
    QLabel *m_serverName;
    QLabel *m_channelName;
    QLabel *m_userCount;
    QLabel *m_lagMeter;
}; // End of class AkiStatusBar.

#endif // AKISTATUSBAR_H
