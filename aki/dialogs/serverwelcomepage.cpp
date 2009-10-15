/*
    This file is part of Aki IRC client.
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

#include "serverwelcomepage.h"
#include "config/serverconfig.h"

ServerWelcomePage::ServerWelcomePage(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    m_config = new Aki::ServerConfig(this);

    connect(channelList, SIGNAL(changed()),
            SLOT(channelListChanged()));
    connect(serverList, SIGNAL(changed()),
            SLOT(serverListChanged()));
    connect(serviceName, SIGNAL(textEdited(QString)),
            SLOT(serviceNameTextEdited(QString)));
    connect(servicePassword, SIGNAL(textEdited(QString)),
            SLOT(servicePasswordTextEdited(QString)));
    connect(useAutoIdentify, SIGNAL(clicked(bool)),
            SLOT(useAutoIdentifyClicked(bool)));
    connect(networkName, SIGNAL(textEdited(QString)),
            SLOT(networkNameTextEdited(QString)));

    loadNewServer();
}

ServerWelcomePage::~ServerWelcomePage()
{
}

void
ServerWelcomePage::loadNewServer()
{
    m_config->createGroup(QString("Default Identity") + QChar('-') + QString("Freenode"));
    m_server = QSharedPointer<Aki::Server>(new Aki::Server("Freenode"));

    m_server->setAddressList(m_config->addressList());
    m_server->setAutoIdentify(m_config->isAutoIdentifyEnabled());
    m_server->setAutoJoinChannels(m_config->isAutoJoinChannelsEnabled());
    m_server->setAutoReconnection(m_config->isAutoReconnectionEnabled());
    m_server->setChannelList(m_config->channelList());
    m_server->setConnectOnStartup(m_config->isConnectOnStartupEnabled());
    m_server->setConnectToRandomServer(m_config->isConnectToRandomServerEnabled());
    m_server->setDefaultEncoding(m_config->isDefaultEncodingEnabled());
    m_server->setEncoding(m_config->encoding());
    m_server->setRetryAttempts(m_config->retryAttemptCount());
    m_server->setRetryInterval(m_config->retryInterval());
    m_server->setServiceName(m_config->serviceName());
    m_server->setServicePassword(m_config->servicePassword());
    m_server->setSsl(m_config->isSslEnabled());

    channelList->setItems(m_server->channelList());
    serverList->setItems(m_server->addressList());
    serviceName->setText(m_server->serviceName());
    servicePassword->setText(m_server->servicePassword());
    useAutoIdentify->setChecked(m_server->isAutoIdentifyEnabled());
    networkName->setText(m_server->name());
}

void
ServerWelcomePage::save()
{
    m_config->setCurrentGroup(QString("Default Identity") + QChar('-') + m_server->name());
    m_config->setAddressList(m_server->addressList());
    m_config->setAutoIdentify(m_server->isAutoIdentifyEnabled());
    m_config->setAutoJoinChannels(m_server->isAutoJoinChannelsEnabled());
    m_config->setAutoReconnection(m_server->isAutoReconnectionEnabled());
    m_config->setChannelList(m_server->channelList());
    m_config->setConnectOnStartup(m_server->isConnectOnStartupEnabled());
    m_config->setConnectToRandomServer(m_server->isConnectToRandomServerEnabled());
    m_config->setDefaultEncoding(m_server->isDefaultEncodingEnabled());
    m_config->setEncoding(m_server->encoding());
    m_config->setRetryAttempts(m_server->retryAttemptCount());
    m_config->setRetryInterval(m_server->retryInterval());
    m_config->setServiceName(m_server->serviceName());
    m_config->setServicePassword(m_server->servicePassword());
    m_config->setSsl(m_server->isSslEnabled());
}

void
ServerWelcomePage::channelListChanged()
{
    m_server->setChannelList(channelList->items());
}

void
ServerWelcomePage::networkNameTextEdited(const QString &name)
{
    m_server->setName(name.trimmed());
}

void
ServerWelcomePage::serverListChanged()
{
    m_server->setAddressList(serverList->items());
}

void
ServerWelcomePage::serviceNameTextEdited(const QString &name)
{
    m_server->setServiceName(name);
}

void
ServerWelcomePage::servicePasswordTextEdited(const QString &password)
{
    m_server->setServicePassword(password);
}

void
ServerWelcomePage::useAutoIdentifyClicked(bool checked)
{
    m_server->setAutoIdentify(checked);
}
