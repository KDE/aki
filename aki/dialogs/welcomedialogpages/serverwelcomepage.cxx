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
#include "sql/identity.hpp"
#include "sql/server.hpp"
#include "ui/addresswidget.hpp"
#include "ui/channelwidget.hpp"
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <KDE/KEditListWidget>
#include <KDE/KLineEdit>
#include <KDE/KTabWidget>
using namespace Aki;

ServerWelcomePage::ServerWelcomePage(Aki::Sql::Database* database, QWidget* parent)
    : QWidget(parent),
    _channels(QStringList()),
    _addresses(QStringList()),
    _database(database),
    _server(0),
    _identity(0)
{
    Q_ASSERT(database);

    _database->create<Aki::Sql::Server>();
    _database->create<Aki::Sql::Address>();
    _database->create<Aki::Sql::Channel>();

    QGridLayout* mainLayout = new QGridLayout;
    setLayout(mainLayout);

    QLabel* label = new QLabel;
    label->setText(i18n("Network Name:"));
    mainLayout->addWidget(label, 0, 0, 1, 1);

    _networkName = new KLineEdit;
    label->setBuddy(_networkName);
    _networkName->setText("Freenode");
    mainLayout->addWidget(_networkName, 0, 1, 1, 1);

    // Create the TabWidget for the dialogue.
    KTabWidget* serverTabWidget = new KTabWidget;
    mainLayout->addWidget(serverTabWidget, 1, 0, 1, 2);

    // Create the first page for it called the Servers Page.
    QWidget* serversPage = new QWidget;
    serverTabWidget->addTab(serversPage, i18n("Servers"));

    QGridLayout* serversPageLayout = new QGridLayout;
    serversPage->setLayout(serversPageLayout);

    Aki::Sql::Address* address = new Aki::Sql::Address;
    address->setAddress("chat.freenode.net");
    address->setPort(7000);
    address->setSsl(true);

    _addressListWidget = new Aki::AddressWidget;
    serversPageLayout->addWidget(_addressListWidget, 0, 0, 1, 1);
    _addressListWidget->setDatabase(_database);
    _addressListWidget->addAddress(address);
    // End of the first page (Servers Page).

    // Create the second page for it called the Channels Page.
    QWidget* channelsPage = new QWidget;
    serverTabWidget->addTab(channelsPage, i18n("Channels"));

    QGridLayout* channelsPageLayout = new QGridLayout;
    channelsPage->setLayout(channelsPageLayout);

    _channelListWidget = new Aki::ChannelWidget;
    channelsPageLayout->addWidget(_channelListWidget, 0, 0, 1, 1);
    _channelListWidget->setDatabase(database);
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
    _serviceName->setClearButtonShown(true);
    _serviceName->setText("nickserv");
    connect(_serviceName, SIGNAL(textEdited(QString)),
            SLOT(slotServiceNameTextEdited(QString)));

    QLabel* label3 = new QLabel;
    authenticationBoxLayout->setWidget(1, QFormLayout::LabelRole, label3);
    label3->setText(i18n("Service Password"));

    _servicePassword = new KLineEdit;
    label3->setBuddy(_servicePassword);
    authenticationBoxLayout->setWidget(1, QFormLayout::FieldRole, _servicePassword);
    _servicePassword->setClearButtonShown(true);
    _servicePassword->setEchoMode(QLineEdit::Password);
    connect(_servicePassword, SIGNAL(textEdited(QString)),
            SLOT(slotServicePasswordTextEdited(QString)));

    QSpacerItem* authenticationBoxSpacer = new QSpacerItem(20, 158, QSizePolicy::Minimum);
    authenticationPageLayout->addItem(authenticationBoxSpacer, 1, 0, 1, 1);
    // End of the last page (Authentication page).

    loadNewServer();
}

ServerWelcomePage::~ServerWelcomePage()
{
}

void
ServerWelcomePage::loadNewServer()
{
    _server = new Aki::Sql::Server(this);
}

void
ServerWelcomePage::save()
{
    Q_ASSERT(_identity);

    _server->setName(_networkName->text());
    _server->setServerIdentity(_identity->id());
    if (_database->transaction()) {
        if (!_database->add(_server)) {
            if (!_database->rollback()) {
                qDebug() << "Unable to rollback Server data";
            }
        } else {
            if (!_database->commit()) {
                qDebug() << "Unable to commit Server data";
            }
        }
    } else {
        qDebug() << "Unable to start Server transaction";
        return;
    }

    for (int i = 0, c = _addressListWidget->count(); i < c; ++i) {
        Aki::Sql::Address* tmp = _addressListWidget->address(i);
        tmp->setAddressServer(_server->id());

        if (_database->transaction()) {
            if (!_database->add(tmp)) {
                if (!_database->rollback()) {
                    qDebug() << "Unable to rollback Address data";
                }
            } else {
                if (!_database->commit()) {
                    qDebug() << "Unable to commit Address data";
                }
            }
        } else {
            qDebug() << "Unable to start Address transaction";
        }
    }

    for (int i = 0, c = _channelListWidget->count(); i < c; ++i) {
        Aki::Sql::Channel* channel = _channelListWidget->channel(i);
        if (channel) {
            channel->setChannelServer(_server->id());

            if (_database->transaction()) {
                if (!_database->add(channel)) {
                    if (!_database->rollback()) {
                        qDebug() << "Unable to rollback Channel data";
                    }
                } else {
                    if (!_database->commit()) {
                        qDebug() << "Unable to commit Channel data";
                    }
                }
            } else {
                qDebug() << "Unable to start Channel transaction";
            }
        }
    }
}

void
ServerWelcomePage::setIdentity(Aki::Sql::Identity* identity)
{
    _identity = identity;
}

void
ServerWelcomePage::slotAuthenticationBoxClicked(bool clicked)
{
    _server->setAutoIdentify(clicked);
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
