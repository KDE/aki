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
#include <QScopedPointer>

namespace Aki
{
class ServerConfig;
class IdentityConfig;
class ServerPagePrivate;
class ServerPage : public Aki::SettingsPageInterface,
                   public Ui::ServerPage
{
    Q_OBJECT
public:
    ServerPage(QWidget *parent = 0);
    ~ServerPage();
    virtual bool hasChanged() const;
    virtual void updateSettings();
    void setChanged(bool enable);
private:
    Q_PRIVATE_SLOT(d, void addNetworkClicked())
    Q_PRIVATE_SLOT(d, void deleteNetworkClicked())
    Q_PRIVATE_SLOT(d, void editNetworkClicked())
    Q_PRIVATE_SLOT(d, void identityActivated(const QString &name))
    Q_PRIVATE_SLOT(d, void autoJoinChannelsClicked(bool checked))
    Q_PRIVATE_SLOT(d, void channelListChanged())
    Q_PRIVATE_SLOT(d, void connectOnStartupClicked(bool checked))
    Q_PRIVATE_SLOT(d, void connectToRandomServerClicked(bool checked))
    Q_PRIVATE_SLOT(d, void defaultEncodingIndexChanged(const QString &encodingName))
    Q_PRIVATE_SLOT(d, void retryAttemptsValueChanged(int count))
    Q_PRIVATE_SLOT(d, void retryIntervalValueChanged(int seconds))
    Q_PRIVATE_SLOT(d, void serverListChanged())
    Q_PRIVATE_SLOT(d, void serviceNameTextEdited(const QString &name))
    Q_PRIVATE_SLOT(d, void servicePasswordTextEdited(const QString &password))
    Q_PRIVATE_SLOT(d, void useAutoIdentifyClicked(bool checked))
    Q_PRIVATE_SLOT(d, void useDefaultEncodingClicked(bool checked))
    Q_PRIVATE_SLOT(d, void useReconnectionClicked(bool checked))
    Q_PRIVATE_SLOT(d, void useSSLClicked(bool checked))
    Q_PRIVATE_SLOT(d, void loadNetworkSettings(const QString &name))
    Q_PRIVATE_SLOT(d, void exportNetworksClicked())
    Q_PRIVATE_SLOT(d, void importNetworksClicked())
private:
    friend class ServerPagePrivate;
    QScopedPointer<ServerPagePrivate> d;
    typedef QMap<QString, Aki::ServerList> ServerMap;
}; // End of class ServerPage.
} // End of namespace Aki.

#endif // SERVERPAGE_H
