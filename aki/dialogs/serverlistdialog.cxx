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

#include "serverlistdialog.hpp"
#include "sql/address.hpp"
#include "sql/database.hpp"
#include "sql/identity.hpp"
#include "sql/server.hpp"
#include "ui/addresswidget.hpp"
#include "ui/channelwidget.hpp"
#include "ui/encodingcombobox.hpp"
#include "ui/identitycombobox.hpp"
#include "ui/serverwidget.hpp"
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <KDE/KIntSpinBox>
#include <KDE/KMessageBox>
#include <KDE/KLineEdit>
#include <KDE/KLocale>
#include <KDE/KTabWidget>
using namespace Aki;

ServerListDialog::ServerListDialog(QWidget* parent)
    : KDialog(parent)
{
    _database = new Aki::Sql::Database("QSQLITE");
    _database->setDatabaseName(Aki::databaseFile());
    if (!_database->open()) {
        KMessageBox::error(this, i18n("Unable to open database file.\nSettings will not be saved."),
                           i18n("Unable to open database."));
    }

    QWidget* mainWidget = new QWidget;
    setMainWidget(mainWidget);

    QFormLayout* mainLayout = new QFormLayout;
    mainWidget->setLayout(mainLayout);

    QLabel* identityLabel = new QLabel;
    mainLayout->setWidget(0, QFormLayout::LabelRole, identityLabel);
    identityLabel->setText(i18n("Identity:"));

    _identityComboBox = new Aki::IdentityComboBox;
    mainLayout->setWidget(0, QFormLayout::FieldRole, _identityComboBox);
    _identityComboBox->setDatabase(_database);

    QGroupBox* serversGroupBox = new QGroupBox;
    mainLayout->setWidget(1, QFormLayout::SpanningRole, serversGroupBox);
    serversGroupBox->setTitle(i18n("Servers"));

    QGridLayout* serversGroupBoxLayout = new QGridLayout;
    serversGroupBox->setLayout(serversGroupBoxLayout);

    _serverWidget = new Aki::ServerWidget;
    serversGroupBoxLayout->addWidget(_serverWidget, 0, 0, 1, 1);
    _serverWidget->setDatabase(_database);

    KTabWidget* serversSettingsTabWidget = new KTabWidget;
    mainLayout->setWidget(2, QFormLayout::SpanningRole, serversSettingsTabWidget);

    QWidget* connectionPage = new QWidget;
    serversSettingsTabWidget->addTab(connectionPage, i18n("Connection"));

    QGridLayout* connectionPageLayout = new QGridLayout;
    connectionPage->setLayout(connectionPageLayout);

    _autoReconnectGroupBox = new QGroupBox;
    connectionPageLayout->addWidget(_autoReconnectGroupBox, 0, 0, 1, 1);
    _autoReconnectGroupBox->setCheckable(true);
    _autoReconnectGroupBox->setChecked(false);
    _autoReconnectGroupBox->setTitle(i18n("Auto Reconnect"));


    QFormLayout* autoReconnectGroupBoxLayout = new QFormLayout;
    _autoReconnectGroupBox->setLayout(autoReconnectGroupBoxLayout);

    QLabel* retryIntervalLabel = new QLabel;
    autoReconnectGroupBoxLayout->setWidget(0, QFormLayout::LabelRole, retryIntervalLabel);
    retryIntervalLabel->setText(i18n("Retry interval (seconds):"));

    _retryInterval = new KIntSpinBox;
    autoReconnectGroupBoxLayout->setWidget(0, QFormLayout::FieldRole, _retryInterval);
    retryIntervalLabel->setBuddy(_retryInterval);
    _retryInterval->setMaximum(60);
    _retryInterval->setMinimum(5);
    _retryInterval->setSingleStep(5);
    _retryInterval->setValue(_retryInterval->minimum());
    _retryInterval->setAlignment(Qt::AlignRight);
    _retryInterval->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);

    QLabel* retryAttemptsLabel = new QLabel;
    autoReconnectGroupBoxLayout->setWidget(1, QFormLayout::LabelRole, retryAttemptsLabel);
    retryAttemptsLabel->setText(i18n("Retry attempts:"));

    _retryAttempts = new KIntSpinBox;
    autoReconnectGroupBoxLayout->setWidget(1, QFormLayout::FieldRole, _retryAttempts);
    retryAttemptsLabel->setBuddy(_retryAttempts);
    _retryAttempts->setMaximum(60);
    _retryAttempts->setMinimum(0);
    _retryAttempts->setSingleStep(1);
    _retryAttempts->setValue(0);
    _retryAttempts->setAlignment(Qt::AlignRight);
    _retryAttempts->setSpecialValueText(i18n("Unlimited"));
    _retryAttempts->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);

    QGroupBox* connectionOptionsGroupBox = new QGroupBox;
    connectionPageLayout->addWidget(connectionOptionsGroupBox, 1, 0, 1, 1);
    connectionOptionsGroupBox->setTitle(i18n("Connection Options"));

    QGridLayout* connectionOptionsGroupBoxLayout = new QGridLayout;
    connectionOptionsGroupBox->setLayout(connectionOptionsGroupBoxLayout);

    _connectOnStartup = new QCheckBox;
    connectionOptionsGroupBoxLayout->addWidget(_connectOnStartup, 0, 0, 1, 1);
    _connectOnStartup->setText(i18n("Connect on startup"));
    connectionPageLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
                                  2, 0, 1, 1);

    QWidget* serversPage = new QWidget;
    serversSettingsTabWidget->addTab(serversPage, i18n("Servers"));

    QGridLayout* serversPageLayout = new QGridLayout;
    serversPage->setLayout(serversPageLayout);

    _addressWidget = new Aki::AddressWidget;
    serversPageLayout->addWidget(_addressWidget, 0, 0, 1, 1);
    _addressWidget->setDatabase(_database);

    _connectToRandomServers = new QCheckBox;
    serversPageLayout->addWidget(_connectToRandomServers, 1, 0, 1, 1);
    _connectToRandomServers->setText(i18n("Connect to random servers"));

    QWidget* channelsPage = new QWidget;
    serversSettingsTabWidget->addTab(channelsPage, i18n("Channels"));

    QGridLayout* channelPageLayout = new QGridLayout;
    channelsPage->setLayout(channelPageLayout);

    _channelWidget = new Aki::ChannelWidget;
    channelPageLayout->addWidget(_channelWidget, 0, 0, 1, 1);
    _channelWidget->setDatabase(_database);

    _autoJoinChannels = new QCheckBox;
    channelPageLayout->addWidget(_autoJoinChannels, 1, 0, 1, 1);
    _autoJoinChannels->setText(i18n("Auto join channels"));

    QWidget* encodingPage = new QWidget;
    serversSettingsTabWidget->addTab(encodingPage, i18n("Encoding"));

    QGridLayout* encodingPageLayout = new QGridLayout;
    encodingPage->setLayout(encodingPageLayout);

    _customEncodingGroupBox = new QGroupBox;
    encodingPageLayout->addWidget(_customEncodingGroupBox, 0, 0, 1, 1);
    _customEncodingGroupBox->setCheckable(true);
    _customEncodingGroupBox->setChecked(false);
    _customEncodingGroupBox->setTitle(i18n("Custom Encoding"));

    QFormLayout* customEncodingGroupBoxLayout = new QFormLayout;
    _customEncodingGroupBox->setLayout(customEncodingGroupBoxLayout);

    QLabel* encodingLabel = new QLabel;
    customEncodingGroupBoxLayout->setWidget(0, QFormLayout::LabelRole, encodingLabel);
    encodingLabel->setText(i18n("Encoding:"));

    _defaultEncoding = new Aki::EncodingComboBox;
    customEncodingGroupBoxLayout->setWidget(0, QFormLayout::FieldRole, _defaultEncoding);
    encodingLabel->setBuddy(_defaultEncoding);
    encodingPageLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
                                1, 0, 1, 1);

    QWidget* authenticationPage = new QWidget;
    serversSettingsTabWidget->addTab(authenticationPage, i18n("Authentication"));

    QGridLayout* authenticationPageLayout = new QGridLayout;
    authenticationPage->setLayout(authenticationPageLayout);

    _autoIdentifyGroupBox = new QGroupBox;
    authenticationPageLayout->addWidget(_autoIdentifyGroupBox, 0, 0, 1, 1);
    _autoIdentifyGroupBox->setCheckable(true);
    _autoIdentifyGroupBox->setChecked(false);
    _autoIdentifyGroupBox->setTitle(i18n("Auto Identify"));

    QFormLayout* autoIdentityGroupBoxLayout = new QFormLayout;
    _autoIdentifyGroupBox->setLayout(autoIdentityGroupBoxLayout);

    QLabel* serviceNameLabel = new QLabel;
    autoIdentityGroupBoxLayout->setWidget(0, QFormLayout::LabelRole, serviceNameLabel);
    serviceNameLabel->setText(i18n("Service Name:"));

    _serviceName = new KLineEdit;
    autoIdentityGroupBoxLayout->setWidget(0, QFormLayout::FieldRole, _serviceName);
    serviceNameLabel->setBuddy(_serviceName);
    _serviceName->setClearButtonShown(true);

    QLabel* servicePasswordLabel = new QLabel;
    autoIdentityGroupBoxLayout->setWidget(1, QFormLayout::LabelRole, servicePasswordLabel);
    servicePasswordLabel->setText(i18n("Service Password:"));

    _servicePassword = new KLineEdit;
    autoIdentityGroupBoxLayout->setWidget(1, QFormLayout::FieldRole, _servicePassword);
    servicePasswordLabel->setBuddy(_servicePassword);
    _servicePassword->setClearButtonShown(true);
    _servicePassword->setEchoMode(QLineEdit::Password);

    authenticationPageLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
                                      1, 0, 1, 1);

    setupActions();

    _identityComboBox->repopulateIdentities();
}

ServerListDialog::~ServerListDialog()
{
}

void
ServerListDialog::setupActions()
{
    connect(_identityComboBox, SIGNAL(currentIndexChanged(Aki::Sql::Identity*)),
            _serverWidget, SLOT(repopulateServers(Aki::Sql::Identity*)));
    connect(_serverWidget, SIGNAL(serverChanged(Aki::Sql::Server*)),
            _addressWidget, SLOT(repopulateAddresses(Aki::Sql::Server*)));
    connect(_serverWidget, SIGNAL(serverChanged(Aki::Sql::Server*)),
            _channelWidget, SLOT(repopulateChannels(Aki::Sql::Server*)));
    connect(_autoReconnectGroupBox, SIGNAL(clicked(bool)),
            SLOT(slotAutoReconnectClicked(bool)));
    connect(_retryInterval, SIGNAL(valueChanged(int)),
            SLOT(slotRetryIntervalValueChanged(int)));
    connect(_retryAttempts, SIGNAL(valueChanged(int)),
            SLOT(slotRetryAttemptsValueChanged(int)));
    connect(_connectOnStartup, SIGNAL(clicked(bool)),
            SLOT(slotConnectOnStartupClicked(bool)));
    connect(_connectToRandomServers, SIGNAL(clicked(bool)),
            SLOT(slotConnectToRandomServersClicked(bool)));
    connect(_autoJoinChannels, SIGNAL(clicked(bool)),
            SLOT(slotAutoJoinChannelsClicked(bool)));
    connect(_customEncodingGroupBox, SIGNAL(clicked(bool)),
            SLOT(slotCustomEncodingClicked(bool)));
    connect(_defaultEncoding, SIGNAL(encodingChanged(QString)),
            SLOT(slotDefaultEncodingEncodingChanged(QString)));
    connect(_autoIdentifyGroupBox, SIGNAL(clicked(bool)),
            SLOT(slotAutoIdentifyClicked(bool)));
    connect(_serviceName, SIGNAL(textEdited(QString)),
            SLOT(slotServiceNameTextEdited(QString)));
    connect(_servicePassword, SIGNAL(textEdited(QString)),
            SLOT(slotServicePasswordTextEdited(QString)));
}

void
ServerListDialog::slotAutoIdentifyClicked(bool clicked)
{
    Q_UNUSED(clicked)
}

void
ServerListDialog::slotAutoJoinChannelsClicked(bool clicked)
{
    Q_UNUSED(clicked)
}

void
ServerListDialog::slotAutoReconnectClicked(bool clicked)
{
    Q_UNUSED(clicked)
}

void
ServerListDialog::slotConnectOnStartupClicked(bool clicked)
{
    Q_UNUSED(clicked)
}

void
ServerListDialog::slotConnectToRandomServersClicked(bool clicked)
{
    Q_UNUSED(clicked)
}

void
ServerListDialog::slotCustomEncodingClicked(bool clicked)
{
    Q_UNUSED(clicked)
}

void
ServerListDialog::slotDefaultEncodingEncodingChanged(const QString& encoding)
{
    Q_UNUSED(encoding)
}

void
ServerListDialog::slotRetryAttemptsValueChanged(int value)
{
    Q_UNUSED(value)
}

void
ServerListDialog::slotRetryIntervalValueChanged(int value)
{
    Q_UNUSED(value)
}

void ServerListDialog::slotServiceNameTextEdited(const QString& text)
{
    Q_UNUSED(text)
}

void
ServerListDialog::slotServicePasswordTextEdited(const QString& text)
{
    Q_UNUSED(text)
}
