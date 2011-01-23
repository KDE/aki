/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "serverwelcomepage.hpp"
#include "aki.hpp"
#include "sql/address.hpp"
#include "sql/channel.hpp"
#include "sql/database.hpp"
#include "sql/server.hpp"
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <KDE/KEditListWidget>
#include <KDE/KLineEdit>
#include <KDE/KTabWidget>
using namespace Aki;

ServerWelcomePage::ServerWelcomePage(QWidget* parent)
    : QWidget(parent),
    _database(0)
{
    QFormLayout* mainLayout = new QFormLayout;
    setLayout(mainLayout);

    QLabel* label = new QLabel;
    label->setText(i18n("Network Name:"));
    mainLayout->setWidget(0, QFormLayout::LabelRole, label);

    _networkName = new KLineEdit;
    label->setBuddy(_networkName);
    mainLayout->setWidget(0, QFormLayout::FieldRole, _networkName);

    // Create the TabWidget for the dialogue.
    KTabWidget* serverTabWidget = new KTabWidget;
    mainLayout->setWidget(1, QFormLayout::SpanningRole, serverTabWidget);

    // Create the first page for it called the Servers Page.
    QWidget* serversPage = new QWidget;
    serverTabWidget->addTab(serversPage, i18n("Servers"));

    QGridLayout* serversPageLayout = new QGridLayout;
    serversPage->setLayout(serversPageLayout);

    _serversListWidget = new KEditListWidget;
    serversPageLayout->addWidget(_serversListWidget, 0, 0, 1, 1);
    connect(_serversListWidget, SIGNAL(changed()),
            SLOT(slotServersListWidgetChanged()));
    // End of the first page (Servers Page).

    // Create the second page for it called the Channels Page.
    QWidget* channelsPage = new QWidget;
    serverTabWidget->addTab(channelsPage, i18n("Channels"));
    QGridLayout* channelsPageLayout = new QGridLayout;
    channelsPage->setLayout(channelsPageLayout);

    _channelsListWidget = new KEditListWidget;
    channelsPageLayout->addWidget(_channelsListWidget, 0, 0, 1, 1);
    connect(_channelsListWidget, SIGNAL(changed()),
            SLOT(slotChannelsListWidgetChanged()));
    // End of the second page (Channels Page).

    // Create the last page for it called the Authentication Page.
    QWidget* authenticationPage = new QWidget;
    serverTabWidget->addTab(authenticationPage, i18n("Authentication"));
    QGridLayout* authenticationPageLayout = new QGridLayout;
    authenticationPage->setLayout(authenticationPageLayout);

    QGroupBox* authenticationBox = new QGroupBox;
    authenticationPageLayout->addWidget(authenticationBox, 0, 0, 1, 1);
    authenticationBox->setTitle(i18n("Auto Identify"));
    authenticationBox->setCheckable(true);
    authenticationBox->setChecked(false);
    connect(authenticationBox, SIGNAL(clicked(bool)),
            SLOT(slotAuthenticationBoxClicked(bool)));

    QFormLayout* authenticationBoxLayout = new QFormLayout;
    authenticationBox->setLayout(authenticationBoxLayout);

    QLabel* label2 = new QLabel;
    authenticationBoxLayout->setWidget(0, QFormLayout::LabelRole, label2);
    label2->setText(i18n("Service Name"));

    _serviceName = new KLineEdit;
    label2->setBuddy(_serviceName);
    authenticationBoxLayout->setWidget(0, QFormLayout::FieldRole, _serviceName);
    connect(_serviceName, SIGNAL(textEdited(QString)),
            SLOT(slotServiceNameTextEdited(QString)));

    QLabel* label3 = new QLabel;
    authenticationBoxLayout->setWidget(1, QFormLayout::LabelRole, label3);
    label3->setText(i18n("Service Password"));

    _servicePassword = new KLineEdit;
    label3->setBuddy(_servicePassword);
    authenticationBoxLayout->setWidget(1, QFormLayout::FieldRole, _servicePassword);
    connect(_servicePassword, SIGNAL(textEdited(QString)),
            SLOT(slotServicePasswordTextEdited(QString)));

    QSpacerItem* authenticationBoxSpacer = new QSpacerItem(20, 158, QSizePolicy::Minimum);
    authenticationPageLayout->addItem(authenticationBoxSpacer, 1, 0, 1, 1);
    // End of the last page (Authentication page).

    _server = new Aki::Sql::Server(this);

    loadNewServer();
}

ServerWelcomePage::~ServerWelcomePage()
{
}

void
ServerWelcomePage::loadNewServer()
{
}

void
ServerWelcomePage::save()
{
    _server->setAutoJoinChannels(true);
    _server->setAutoReconnect(true);
    _server->setConnectOnStartup(true);
    _server->setConnectToRandomServer(false);
    _server->setDefaultEncoding("UTF-8");
    _server->setEncoding("UTF-8");
    _server->setRetryAttemptCount(10);
    _server->setRetryInterval(5);

    if (!_database->add(_server)) {
        return;
    }

    foreach (const QString& address, _addresses) {
        QScopedPointer<Aki::Sql::Address> tmp(new Aki::Sql::Address);
        if (address.contains("/")) {
            const QStringList split = address.split("/", QString::SkipEmptyParts);
            tmp->setAddress(split.at(0));
            bool ok = false;
            quint16 port = split.at(1).toUShort(&ok);
            tmp->setPort(ok ? port : 6667);
        } else {
            tmp->setAddress(address);
            tmp->setPort(6667);
        }

        tmp->setAddressServer(_server->id());
    }

    foreach (const QString& channel, _channels) {
        QScopedPointer<Aki::Sql::Channel> tmp(new Aki::Sql::Channel);
        tmp->setName(channel);
        tmp->setChannelServer(_server->id());
        _database->add(tmp.data());
    }
}

void
ServerWelcomePage::slotAuthenticationBoxClicked(bool clicked)
{
    _server->setAutoIdentify(clicked);
}

void
ServerWelcomePage::slotChannelsListWidgetChanged()
{
    _channels = _channelsListWidget->items();
}

void
ServerWelcomePage::slotServersListWidgetChanged()
{
    _addresses = _serversListWidget->items();
}

void
ServerWelcomePage::slotServiceNameTextEdited(const QString& text)
{
    _server->setServiceName(text);
}

void
ServerWelcomePage::slotServicePasswordTextEdited(const QString& text)
{
    _server->setServicePassword(text);
}
