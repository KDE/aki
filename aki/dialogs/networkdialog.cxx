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

#include "networkdialog.hpp"
#include "aki.hpp"
#include "dialogs/addressdialog.hpp"
#include "dialogs/channeldialog.hpp"
#include "ui/networklist.hpp"
#include "utils/database.hpp"
#include "utils/sqladdress.hpp"
#include "utils/sqlchannel.hpp"
#include "utils/sqlidentity.hpp"
#include "utils/sqlnetwork.hpp"
#include <KDE/KInputDialog>
#include <KDE/KMessageBox>
#include <QtCore/QTextCodec>
using namespace Aki;

NetworkDialog::NetworkDialog(QWidget* parent)
    : KDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setupDialog();
    load();
}

NetworkDialog::~NetworkDialog()
{
}

void
NetworkDialog::createNewNetwork(const QString& name)
{
    // Get the current Identity name.
    const QString identityName = identitySelector->currentText();
    // Get the current identity for the name.
    QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(identityName));
    // Get the new server.
    Aki::SqlNetwork* server = Aki::SqlNetwork::newNetwork(name, identity.data());
    // Append it to the network list.
    networkList->addNetwork(server);
}

void
NetworkDialog::load()
{
    if (!identitySelector->count()) {
        return;
    }

    networkList->repopulateNetwork(identitySelector->currentIdentity());
    Aki::SqlNetwork* network = networkList->currentNetwork();
    if (network) {
        loadNetwork(network);
    }
}

void
NetworkDialog::loadNetwork(Aki::SqlNetwork* network)
{
    Q_ASSERT(network);

    autoReconnectGroupBox->setChecked(network->isAutoReconnectEnabled());
    retryAttemptsSpinBox->setValue(network->retryAttemptCount());
    retryIntervalSpinBox->setValue(network->retryInterval());
    connectOnStartupCheckBox->setChecked(network->isConnectOnStartupEnabled());
    connectToRandomServerCheckBox->setChecked(network->isConnectToRandomServerEnabled());
    autoJoinChannelsCheckBox->setChecked(network->isAutoJoinChannelsEnabled());
    customEncodingGroupBox->setChecked(!network->isDefaultEncodingEnabled());
    encodingComboBox->setCurrentItem(network->encoding());
    autoIdentifyGroupBox->setChecked(network->isAutoIdentifyEnabled());
    serviceNameLineEdit->setText(network->serviceName());
    servicePasswordLineEdit->setText(network->servicePassword());
}

void
NetworkDialog::setupActions()
{
    connect(identitySelector, SIGNAL(identityActivated(Aki::SqlIdentity*)),
            networkList, SLOT(repopulateNetwork(Aki::SqlIdentity*)));
    connect(networkList, SIGNAL(networkCurrentRowChanged(int)),
            SLOT(slotNetworkCurrentRowChanged(int)));
    connect(addNetworkButton, SIGNAL(clicked(bool)),
            SLOT(slotAddNetworkClicked()));
    connect(removeNetworkButton, SIGNAL(clicked(bool)),
            SLOT(slotRemoveNetworkClicked()));
    connect(editNetworkButton, SIGNAL(clicked(bool)),
            SLOT(slotEditNetworkClicked()));
    connect(addAddressButton, SIGNAL(clicked(bool)),
            SLOT(slotAddAddressClicked()));
    connect(editAddressButton, SIGNAL(clicked(bool)),
            SLOT(slotEditAddressClicked()));
    connect(removeAddressButton, SIGNAL(clicked(bool)),
            SLOT(slotRemoveAddressClicked()));
    connect(addChannelButton, SIGNAL(clicked(bool)),
            SLOT(slotAddChannelClicked()));
    connect(editChannelButton, SIGNAL(clicked(bool)),
            SLOT(slotEditChannelClicked()));
    connect(removeChannelButton, SIGNAL(clicked(bool)),
            SLOT(slotRemoveChannelClicked()));
    connect(autoReconnectGroupBox, SIGNAL(clicked(bool)),
            SLOT(slotAutoReconnectClicked(bool)));
    connect(retryIntervalSpinBox, SIGNAL(valueChanged(int)),
            SLOT(slotRetryIntervalValueChanged(int)));
    connect(retryAttemptsSpinBox, SIGNAL(valueChanged(int)),
            SLOT(slotRetryAttemptsValueChanged(int)));
    connect(connectOnStartupCheckBox, SIGNAL(clicked(bool)),
            SLOT(slotConnectOnStartupClicked(bool)));
    connect(connectToRandomServerCheckBox, SIGNAL(clicked(bool)),
            SLOT(slotConnectToRandomServersClicked(bool)));
    connect(autoJoinChannelsCheckBox, SIGNAL(clicked(bool)),
            SLOT(slotAutoJoinChannelsClicked(bool)));
    connect(customEncodingGroupBox, SIGNAL(clicked(bool)),
            SLOT(slotCustomEncodingClicked(bool)));
    connect(encodingComboBox, SIGNAL(activated(QString)),
            SLOT(slotEncodingActivated(QString)));
    connect(serviceNameLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotServiceNameTextEdited(QString)));
    connect(servicePasswordLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotServicePasswordTextEdited(QString)));
    connect(autoIdentifyGroupBox, SIGNAL(clicked(bool)),
            SLOT(slotAutoIdentifiedClicked(bool)));
}

void
NetworkDialog::setupDialog()
{
    QWidget* widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);
    setButtons(User1 | Close);
    setButtonText(User1, i18n("Connect"));
    setButtonIcon(User1, KIcon("network-connect"));
    resize(widget->width(), widget->height());
    enableButton(User1, false);

    // Find all the avilable text codecs.
    foreach (int mib, QTextCodec::availableMibs()) {
        // Get the name as that is all we care about.
        encodingComboBox->addItem(QTextCodec::codecForMib(mib)->name());
    }

    // Sort the list.
    encodingComboBox->model()->sort(0);

    // Setup the signals/slots.
    setupActions();

    // Load button icons.
    setupIcons();
}

void
NetworkDialog::setupIcons()
{
    addNetworkButton->setIcon(KIcon("list-add"));
    removeNetworkButton->setIcon(KIcon("list-remove"));
    editNetworkButton->setIcon(KIcon("edit-rename"));

    addAddressButton->setIcon(KIcon("list-add"));
    removeAddressButton->setIcon(KIcon("list-remove"));
    editAddressButton->setIcon(KIcon("edit-rename"));

    addChannelButton->setIcon(KIcon("list-add"));
    removeChannelButton->setIcon(KIcon("list-remove"));
    editChannelButton->setIcon(KIcon("edit-rename"));
}

void
NetworkDialog::slotAddAddressClicked()
{
    if (!networkList->currentNetwork()) {
        return;
    }

    Aki::AddressDialog* dialog = new Aki::AddressDialog;
    const int result = dialog->exec();

    switch (result) {
    case QDialog::Accepted: {
        Aki::SqlAddress* address =
            Aki::SqlAddress::newAddress(dialog->address(), networkList->currentNetwork());
        if (!address) {
            delete address;
            return;
        }
        address->setPort(dialog->port());
        address->setPassword(dialog->password());
        address->setSsl(dialog->isSslEnabled());
        if (address->save()) {
            networkAddressList->addNetworkAddress(address);
        } else {
            delete address;
        }
        return;
    }
    default: {
        break;
    }
    }
}

void
NetworkDialog::slotAddChannelClicked()
{
    if (!networkList->currentNetwork()) {
        return;
    }

    Aki::ChannelDialog* dialog = new Aki::ChannelDialog;
    const int result = dialog->exec();

    switch (result) {
    case QDialog::Accepted: {
        Aki::SqlChannel* channel =
            Aki::SqlChannel::newChannel(dialog->channelName(), networkList->currentNetwork());
        if (!channel) {
            delete channel;
        }
        channel->setPassword(channel->password());
        if (channel->save()) {
            networkChannelList->addNetworkChannel(channel);
        } else {
            delete channel;
        }
        return;
    }
    default: {
        break;
    }
    }
}

void
NetworkDialog::slotAddNetworkClicked()
{
    bool ok;
    const QString network = KInputDialog::getText(i18n("Add New Network"), i18n("Enter a new network name."),
                                                  QString(), &ok, this);
    // Check to see if the network already exists.
    const bool emptyNetwork = networkList->findNetworks(network, Qt::MatchExactly).isEmpty();
    // Check that the user clicked okay.
    if (ok) {
        if (!emptyNetwork) {
            KMessageBox::error(this, i18n("Unable to add network. Network already exists."));
            // Re show thie dialog.
            slotAddNetworkClicked();
        } else {
            createNewNetwork(network);
        }
    }
}

void
NetworkDialog::slotAutoIdentifiedClicked(bool checked)
{
    Aki::SqlNetwork* server = networkList->currentNetwork();
    server->setAutoIdentity(checked);
    server->save();
}

void
NetworkDialog::slotAutoJoinChannelsClicked(bool checked)
{
    Aki::SqlNetwork* server = networkList->currentNetwork();
    server->setAutoJoinChannels(checked);
    server->save();
}

void
NetworkDialog::slotAutoReconnectClicked(bool checked)
{
    Aki::SqlNetwork* server = networkList->currentNetwork();
    server->setAutoReconnect(checked);
    server->save();
}

void
NetworkDialog::slotConnectOnStartupClicked(bool checked)
{
    Aki::SqlNetwork* server = networkList->currentNetwork();
    server->setConnectOnStartup(checked);
    server->save();
}

void
NetworkDialog::slotConnectToRandomServersClicked(bool checked)
{
    Aki::SqlNetwork* server = networkList->currentNetwork();
    server->setConnectToRandomServer(checked);
    server->save();
}

void
NetworkDialog::slotCustomEncodingClicked(bool checked)
{
    Aki::SqlNetwork* server = networkList->currentNetwork();
    server->setDefaultEncoding(!checked);
    server->save();
}

void NetworkDialog::slotEditAddressClicked()
{
    // Get the current address.
    Aki::SqlAddress* currentAddress = networkAddressList->currentNetworkAddress();

    // Can we safely remove this check?
    if (!currentAddress) {
        return;
    }

    Aki::AddressDialog* dialog = new Aki::AddressDialog;
    dialog->setAddress(currentAddress->address());
    dialog->setPassword(currentAddress->password());
    dialog->setPort(currentAddress->port());
    dialog->setSsl(currentAddress->isSslEnabled());

    switch (dialog->exec()) {
    case QDialog::Accepted: {
        currentAddress->setAddress(dialog->address());
        currentAddress->setPassword(dialog->password());
        currentAddress->setPort(dialog->port());
        currentAddress->setSsl(dialog->isSslEnabled());
        currentAddress->save();
        return;
    }
    default: {
        break;
    }
    }
}

void NetworkDialog::slotEditChannelClicked()
{
    // Get the current channel.
    Aki::SqlChannel* currentChannel = networkChannelList->currentNetworkChannel();

    // Can we safely remove this check?
    if (!currentChannel) {
        return;
    }

    Aki::ChannelDialog* dialog = new Aki::ChannelDialog;
    dialog->setChannelName(currentChannel->channel());
    dialog->setChannelPassword(currentChannel->password());

    switch (dialog->exec()) {
    case QDialog::Accepted: {
        currentChannel->setChannel(dialog->channelName());
        currentChannel->setPassword(dialog->channelPassword());
        currentChannel->save();
        break;
    }
    default: {
        break;
    }
    }
}

void
NetworkDialog::slotEditNetworkClicked()
{
    bool ok;
    // Get the current network.
    Aki::SqlNetwork* currentNetwork = networkList->currentNetwork();

    // Can we safely remove this check?
    if (!currentNetwork) {
        return;
    }

    const QString network = KInputDialog::getText(i18n("Edit Network"), i18n("Enter a new network name."),
                                                  currentNetwork->name(), &ok, this);
    // Check to see if the network already exists.
    const bool emptyNetwork = networkList->findNetworks(network, Qt::MatchExactly).isEmpty();
    // Check that the user clicked okay.
    if (ok) {
        if (!emptyNetwork) {
            KMessageBox::error(this, i18n("Unable to edit network. Network already exists."));
            // Re show this dialog.
            slotEditNetworkClicked();
        } else {
            // Change the network name.
            currentNetwork->setName(network);
            // Save it to the database.
            currentNetwork->save();
        }
    }
}

void
NetworkDialog::slotEncodingActivated(const QString& encoding)
{
    Aki::SqlNetwork* server = networkList->currentNetwork();
    server->setEncoding(encoding.toLatin1());
    server->save();
}

void
NetworkDialog::slotIdentityActivated(SqlIdentity* identity)
{
    if (!identity) {
        return;
    }

    networkList->setCurrentRow(0);
}

void
NetworkDialog::slotNetworkCurrentRowChanged(int currentRow)
{
    Aki::SqlNetwork* network = networkList->item(currentRow);
    if (!network) {
        return;
    }

    loadNetwork(network);

    networkAddressList->repopulateNetworkAddresses(networkList->item(currentRow));
    networkAddressList->setCurrentRow(0);
    networkChannelList->repopulateNetworkChannels(networkList->item(currentRow));
    networkChannelList->setCurrentRow(0);
}

void
NetworkDialog::slotRemoveAddressClicked()
{
    // Get the current address.
    Aki::SqlAddress* currentAddress = networkAddressList->currentNetworkAddress();

    // Can we safely remove this check?
    if (!currentAddress) {
        return;
    }

    int result = KMessageBox::warningYesNo(this, i18n("You are about to remove %1. This cannot be undone.",
                                                      currentAddress->address()),
                                           i18n("Confirm Server Address Deletion"));
    switch (result) {
    case KMessageBox::Yes: {
        // Remove the address from the server.
        currentAddress->remove();
        // Take the network from the network list.
        delete networkAddressList->takeNetworkAddress(networkAddressList->row(currentAddress));
        break;
    }
    }
}

void
NetworkDialog::slotRemoveChannelClicked()
{
#warning please finish slotRemoveChannelClicked
}

void
NetworkDialog::slotRemoveNetworkClicked()
{
    // Get the current network.
    Aki::SqlNetwork* currentNetwork = networkList->currentNetwork();

    // Can we safely remove this check?
    if (!currentNetwork) {
        return;
    }

    int result = KMessageBox::warningYesNo(this, i18n("You are about to remove %1. This cannot be undone.",
                                                      currentNetwork->name()),
                                           i18n("Confirm Network Deletion"));
    switch (result) {
    case KMessageBox::Yes: {
        // Remove the network from the server.
        currentNetwork->remove();
        // Take the network from the network list.
        delete networkList->takeNetwork(networkList->row(currentNetwork));
        break;
    }
    }
}

void
NetworkDialog::slotRetryAttemptsValueChanged(int count)
{
    Aki::SqlNetwork* server = networkList->currentNetwork();
    server->setRetryAttempts(count);
    server->save();
}

void
NetworkDialog::slotRetryIntervalValueChanged(int seconds)
{
    Aki::SqlNetwork* server = networkList->currentNetwork();
    server->setRetryInterval(seconds);
    server->save();
}

void
NetworkDialog::slotServiceNameTextEdited(const QString& name)
{
    Aki::SqlNetwork* server = networkList->currentNetwork();
    server->setServiceName(name);
    server->save();
}

void
NetworkDialog::slotServicePasswordTextEdited(const QString& password)
{
    Aki::SqlNetwork* server = networkList->currentNetwork();
    server->setServicePassword(password);
    server->save();
}
