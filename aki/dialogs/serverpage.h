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

#ifndef SERVERPAGE_H
#define SERVERPAGE_H

#include "interfaces/settingspageinterface.h"
#include "server.h"
#include "ui_serverpage.h"

namespace Aki
{
class ServerConfig;
class IdentityConfig;
} // End of namespace Aki.

class ServerPage : public Aki::SettingsPageInterface,
                   public Ui::ServerPage
{
    Q_OBJECT
public:
    ServerPage(QWidget *parent = 0);
    ~ServerPage();
    virtual bool hasChanged() const;
    virtual void updateSettings();
    void loadSettings();
    void newNetworkSettings(const QString &name);
    void setChanged(bool enable);
private Q_SLOTS:
    void slotAddNetworkClicked();
    void slotDeleteNetworkClicked();
    void slotEditNetworkClicked();
    void slotIdentityActivated(const QString &name);
    void slotAutoJoinChannelsClicked(bool checked);
    void slotChannelListChanged();
    void slotConnectOnStartupClicked(bool checked);
    void slotConnectToRandomServerClicked(bool checked);
    void slotDefaultEncodingIndexChanged(const QString &encodingName);
    void slotRetryAttemptsValueChanged(int count);
    void slotRetryIntervalValueChanged(int seconds);
    void slotServerListChanged();
    void slotServiceNameTextEdited(const QString &name);
    void slotServicePasswordTextEdited(const QString &password);
    void slotUseAutoIdentifyClicked(bool checked);
    void slotUseDefaultEncodingClicked(bool checked);
    void slotUseReconnectionClicked(bool checked);
    void slotUseSSLClicked(bool checked);
    void slotLoadNetworkSettings(const QString &name);
    void slotExportNetworksClicked();
    void slotImportNetworksClicked();
private:
    Aki::ServerPointer serverNetworkForCurrentIdentity();
private:
    typedef QMap<QString, Aki::ServerList> ServerMap;
    ServerMap m_serverList;
    Aki::ServerConfig *m_serverConfig;
    Aki::IdentityConfig *m_identityConfig;
    bool m_hasChanged;
}; // End of class ServerPage.

#endif // SERVERPAGE_H
