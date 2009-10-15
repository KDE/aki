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
#include "server.h"
#include "config/serverconfig.h"
using namespace Aki;

namespace Aki
{
class ServerWelcomePagePrivate
{
public:
    ServerWelcomePagePrivate(Aki::ServerWelcomePage *qq)
        : q(qq),
        config(0)
    {
    }

    void loadNewServer()
    {
        config->createGroup(QString("Default Identity") + QChar('-') + QString("Freenode"));
        server = QSharedPointer<Aki::Server>(new Aki::Server("Freenode"));

        server->setAddressList(config->addressList());
        server->setAutoIdentify(config->isAutoIdentifyEnabled());
        server->setAutoJoinChannels(config->isAutoJoinChannelsEnabled());
        server->setAutoReconnection(config->isAutoReconnectionEnabled());
        server->setChannelList(config->channelList());
        server->setConnectOnStartup(config->isConnectOnStartupEnabled());
        server->setConnectToRandomServer(config->isConnectToRandomServerEnabled());
        server->setDefaultEncoding(config->isDefaultEncodingEnabled());
        server->setEncoding(config->encoding());
        server->setRetryAttempts(config->retryAttemptCount());
        server->setRetryInterval(config->retryInterval());
        server->setServiceName(config->serviceName());
        server->setServicePassword(config->servicePassword());
        server->setSsl(config->isSslEnabled());

        q->channelList->setItems(server->channelList());
        q->serverList->setItems(server->addressList());
        q->serviceName->setText(server->serviceName());
        q->servicePassword->setText(server->servicePassword());
        q->useAutoIdentify->setChecked(server->isAutoIdentifyEnabled());
        q->networkName->setText(server->name());
    }

    void channelListChanged()
    {
        server->setChannelList(q->channelList->items());
    }

    void networkNameTextEdited(const QString &name)
    {
        server->setName(name.trimmed());
    }

    void serverListChanged()
    {
        server->setAddressList(q->serverList->items());
    }

    void serviceNameTextEdited(const QString &name)
    {
        server->setServiceName(name);
    }

    void servicePasswordTextEdited(const QString &password)
    {
        server->setServicePassword(password);
    }

    void useAutoIdentifyClicked(bool checked)
    {
        server->setAutoIdentify(checked);
    }

    Aki::ServerWelcomePage *q;
    Aki::ServerPointer server;
    Aki::ServerConfig *config;
}; // End of class ServerWelcomePagePrivate.
} // End of namspace Aki.

ServerWelcomePage::ServerWelcomePage(QWidget *parent)
    : QWidget(parent)
{
    d.reset(new ServerWelcomePagePrivate(this));
    setupUi(this);

    d->config = new Aki::ServerConfig(this);

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

    d->loadNewServer();
}

ServerWelcomePage::~ServerWelcomePage()
{
}

void
ServerWelcomePage::save()
{
    d->config->setCurrentGroup(QString("Default Identity") + QChar('-') + d->server->name());
    d->config->setAddressList(d->server->addressList());
    d->config->setAutoIdentify(d->server->isAutoIdentifyEnabled());
    d->config->setAutoJoinChannels(d->server->isAutoJoinChannelsEnabled());
    d->config->setAutoReconnection(d->server->isAutoReconnectionEnabled());
    d->config->setChannelList(d->server->channelList());
    d->config->setConnectOnStartup(d->server->isConnectOnStartupEnabled());
    d->config->setConnectToRandomServer(d->server->isConnectToRandomServerEnabled());
    d->config->setDefaultEncoding(d->server->isDefaultEncodingEnabled());
    d->config->setEncoding(d->server->encoding());
    d->config->setRetryAttempts(d->server->retryAttemptCount());
    d->config->setRetryInterval(d->server->retryInterval());
    d->config->setServiceName(d->server->serviceName());
    d->config->setServicePassword(d->server->servicePassword());
    d->config->setSsl(d->server->isSslEnabled());
}

#include "serverwelcomepage.moc"
