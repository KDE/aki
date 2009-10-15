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

#include "serverpage.h"
#include "config/identityconfig.h"
#include "config/serverconfig.h"
#include "networkexporter.h"
#include "networkimporter.h"
#include "settings.h"
#include <KFileDialog>
#include <KInputDialog>
#include <KMessageBox>
#include <QTextCodec>

ServerPage::ServerPage(QWidget *parent)
    : Aki::SettingsPageInterface(i18n("Servers"), Aki::Settings::self(),
                                 "preferences-system-network", i18n("Configure Server Settings"),
                                 parent),
    m_hasChanged(false)
{
    setupUi(this);

    // Setup the identity and server configuration files.
    m_identityConfig = new Aki::IdentityConfig(this);
    m_serverConfig = new Aki::ServerConfig(this);

    // Setup icons for the buttons.
    addNetworkButton->setIcon(KIcon("list-add"));
    deleteNetworkButton->setIcon(KIcon("list-remove"));
    editNetworkButton->setIcon(KIcon("edit-rename"));

    QStringListIterator strIter(m_identityConfig->groupList());
    while (strIter.hasNext()) {
        identitySelector->addItem(strIter.next());
    }

    identitySelector->model()->sort(0);
    identitySelector->setCurrentIndex(0);
    slotIdentityActivated(identitySelector->currentText());

    // Get all available codecs.
    foreach (int mib, QTextCodec::availableMibs()) {
        defaultEncoding->addItem(QTextCodec::codecForMib(mib)->name());
    }

    defaultEncoding->model()->sort(0);

    connect(addNetworkButton, SIGNAL(clicked(bool)), SLOT(slotAddNetworkClicked()));
    connect(deleteNetworkButton, SIGNAL(clicked(bool)), SLOT(slotDeleteNetworkClicked()));
    connect(editNetworkButton, SIGNAL(clicked(bool)), SLOT(slotEditNetworkClicked()));
    connect(identitySelector, SIGNAL(activated(QString)), SLOT(slotIdentityActivated(QString)));
    connect(autoJoinChannels, SIGNAL(clicked(bool)), SLOT(slotAutoJoinChannelsClicked(bool)));
    connect(channelList, SIGNAL(changed()), SLOT(slotChannelListChanged()));
    connect(connectOnStartup, SIGNAL(clicked(bool)), SLOT(slotConnectOnStartupClicked(bool)));
    connect(connectToRandomServer, SIGNAL(clicked(bool)), SLOT(slotConnectToRandomServerClicked(bool)));
    connect(defaultEncoding, SIGNAL(currentIndexChanged(QString)), SLOT(slotDefaultEncodingIndexChanged(QString)));
    connect(networkList, SIGNAL(currentTextChanged(QString)), SLOT(slotLoadNetworkSettings(QString)));
    connect(retryAttempts, SIGNAL(valueChanged(int)), SLOT(slotRetryAttemptsValueChanged(int)));
    connect(retryInterval, SIGNAL(valueChanged(int)), SLOT(slotRetryIntervalValueChanged(int)));
    connect(serverList, SIGNAL(changed()), SLOT(slotServerListChanged()));
    connect(serviceName, SIGNAL(textEdited(QString)), SLOT(slotServiceNameTextEdited(QString)));
    connect(servicePassword, SIGNAL(textEdited(QString)), SLOT(slotServicePasswordTextEdited(QString)));
    connect(useAutoIdentify, SIGNAL(clicked(bool)), SLOT(slotUseAutoIdentifyClicked(bool)));
    connect(useDefaultEncoding, SIGNAL(clicked(bool)), SLOT(slotUseDefaultEncodingClicked(bool)));
    connect(useReconnection, SIGNAL(clicked(bool)), SLOT(slotUseReconnectionClicked(bool)));
    connect(useSSL, SIGNAL(clicked(bool)), SLOT(slotUseSSLClicked(bool)));
    connect(exportNetworks, SIGNAL(clicked(bool)), SLOT(slotExportNetworksClicked()));
    connect(importNetworks, SIGNAL(clicked(bool)), SLOT(slotImportNetworksClicked()));

    loadSettings();
}

ServerPage::~ServerPage()
{
}

bool
ServerPage::hasChanged() const
{
    return m_hasChanged;
}

void
ServerPage::updateSettings()
{
    QMapIterator<QString, Aki::ServerList> serverMapIter(m_serverList);
    while (serverMapIter.hasNext()) {
        serverMapIter.next();
        // Get current identity name.
        const QString identityName = serverMapIter.key();

        QListIterator<Aki::ServerPointer> serverListIter(serverMapIter.value());
        while (serverListIter.hasNext()) {
            Aki::ServerPointer server = serverListIter.next();

            m_serverConfig->setCurrentGroup(identityName + QChar('-') + server->name());
            m_serverConfig->setAddressList(server->addressList());
            m_serverConfig->setAutoIdentify(server->isAutoIdentifyEnabled());
            m_serverConfig->setAutoJoinChannels(server->isAutoJoinChannelsEnabled());
            m_serverConfig->setAutoReconnection(server->isAutoReconnectionEnabled());
            m_serverConfig->setChannelList(server->channelList());
            m_serverConfig->setConnectOnStartup(server->isConnectOnStartupEnabled());
            m_serverConfig->setConnectToRandomServer(server->isConnectToRandomServerEnabled());
            m_serverConfig->setDefaultEncoding(server->isDefaultEncodingEnabled());
            m_serverConfig->setEncoding(server->encoding());
            m_serverConfig->setRetryAttempts(server->retryAttemptCount());
            m_serverConfig->setRetryInterval(server->retryInterval());
            m_serverConfig->setServiceName(server->serviceName());
            m_serverConfig->setServicePassword(server->servicePassword());
            m_serverConfig->setSsl(server->isSslEnabled());
        }
    }

    m_serverConfig->save();
    setChanged(false);
}

void
ServerPage::slotAddNetworkClicked()
{
    bool ok = false;
    QString network = KInputDialog::getText(i18n("Add new network"), i18n("Enter a new network name."),
                                            QString(), &ok, this);

    // Check to see that the network is not empty and that we found a match.
    if (ok && !network.isEmpty() &&
        !networkList->findItems(network, Qt::MatchExactly).isEmpty()) {
        // This network already exists so we do nothing except make the
        // user re-enter a different one.
        KMessageBox::error(this, i18n("Unable to add network. Network already exists."));
        slotAddNetworkClicked();
    } else if (ok && !network.isEmpty() && // Network isn't empty and there isn't one that exists.
               networkList->findItems(network, Qt::MatchExactly).isEmpty()) {
        newNetworkSettings(network);
        setChanged(true);
    } else if (ok && network.isEmpty()) { // Empty network name.
        KMessageBox::error(this, i18n("Network name is empty."));
        slotAddNetworkClicked();
    }
}

void
ServerPage::slotDeleteNetworkClicked()
{
    // Get the current item.
    QListWidgetItem *item = networkList->currentItem();

    // Get the identityName.
    const QString identityName = identitySelector->currentText();

    // Make sure it is valid (one is selected).
    if (item) {
        // Append the correct name that is in the configuration file.
        // And delete the group
        m_serverConfig->deleteGroup(identityName + QChar('-') + item->text());

        // Search for network and remove it from the list.
        foreach (Aki::ServerPointer server, m_serverList[identityName]) {
            // Does this server match the selected server.
            if (server->name() == item->text()) {
                const int index = m_serverList[identityName].indexOf(server);
                m_serverList[identityName].removeAt(index);
                break;
            }
        }

        // Delete the network widget from the list.
        delete networkList->takeItem(networkList->row(item));
        setChanged(true);
    }
    
    // Disable the delete button when there is only 1 there.
    if (networkList->count() == 1) {
        deleteNetworkButton->setDisabled(true);
    }
}

void
ServerPage::slotEditNetworkClicked()
{
    bool ok = false;

    // Get the current item.
    QListWidgetItem *item = networkList->currentItem();
    // Store the old name.
    const QString oldName = item->text();
    // Get the current identity name.
    const QString identityName = identitySelector->currentText();

    // Get the new network name.
    QString newName = KInputDialog::getText(i18n("Edit current network."),
                                            i18n("Enter a new network name."), oldName, &ok, this);

    // Check to see if the new network name is not empty and the old name
    // doesn't match the new name.
    if (ok && !newName.isEmpty() && oldName != newName) {
        // Since it not empty and they don't match
        // Make sure the new one isn't already taken.
        if (networkList->findItems(newName, Qt::MatchExactly).isEmpty()) {
            // Append the correct name that is in the configuration file.
            // And delete the group
            m_serverConfig->deleteGroup(identityName + QChar('-') + oldName);

            // Append the correct name that will be in the configuration file.
            // And create the group.
            m_serverConfig->createGroup(identityName + QChar('-') + newName);

            // Search for the network that matches the old name
            // And update it.
            foreach (Aki::ServerPointer server, m_serverList[identityName]) {
                // Does the server name match the old name?
                if (server->name() == oldName) {
                    const int index = m_serverList[identityName].indexOf(server);
                    // Update the old name to the new name now.
                    m_serverList[identityName].value(index)->setName(newName);
                    // Update it in the list widget now.
                    item->setText(newName);
                    setChanged(true);
                }
            }
        } else { // The new network name already exist.
            KMessageBox::error(this, i18n("Unable to edit network. Network already exists."));
            slotEditNetworkClicked();
        }
    } else if (ok && newName.isEmpty()) { // The new network name is empty
        KMessageBox::error(this, i18n("Network name cannot be empty."));
        slotEditNetworkClicked();
    } else if (ok && !newName.isEmpty() && newName == oldName) { // The new network name is the same as the old one.
        KMessageBox::error(this, i18n("New network name cannot be the same as the old one."));
        slotEditNetworkClicked();
    }
}

void
ServerPage::loadSettings()
{
    // Check and see if the server configuration is empty.
    if (!m_serverConfig->groupList().isEmpty()) {
        QStringListIterator identIter(m_identityConfig->groupList());
        // Load identities.
        while (identIter.hasNext()) {
            const QString identityName = identIter.next();
            Aki::ServerList list;

            QStringListIterator serverIter(m_serverConfig->groupList());
            while (serverIter.hasNext()) {
                QString network = serverIter.next();
                // Check and make sure we only continue if the identity has servers.
                // If it doesn't then we can skip them.
                if (network.contains(identityName + QChar('-'))) {

                    // Set the current network.
                    m_serverConfig->setCurrentGroup(network);

                    // Remove the configuration prefix on the group.
                    network.remove(identityName + QChar('-'));

                    // If the current identity is the identity being loading
                    // We load it to the network list else we ignore it.
                    if (identitySelector->currentText() == identityName) {
                        networkList->addItem(network);
                    }

                    networkList->sortItems();

                    Aki::ServerPointer server(new Aki::Server(network));
                    server->setAddressList(m_serverConfig->addressList());
                    server->setAutoIdentify(m_serverConfig->isAutoIdentifyEnabled());
                    server->setAutoJoinChannels(m_serverConfig->isAutoJoinChannelsEnabled());
                    server->setAutoReconnection(m_serverConfig->isAutoReconnectionEnabled());
                    server->setChannelList(m_serverConfig->channelList());
                    server->setConnectOnStartup(m_serverConfig->isConnectOnStartupEnabled());
                    server->setConnectToRandomServer(m_serverConfig->isConnectToRandomServerEnabled());
                    server->setDefaultEncoding(m_serverConfig->isDefaultEncodingEnabled());
                    server->setEncoding(m_serverConfig->encoding());
                    server->setRetryAttempts(m_serverConfig->retryAttemptCount());
                    server->setRetryInterval(m_serverConfig->retryInterval());
                    server->setServiceName(m_serverConfig->serviceName());
                    server->setServicePassword(m_serverConfig->servicePassword());
                    server->setSsl(m_serverConfig->isSslEnabled());

                    list << server;
                }
                if (!list.isEmpty()) {
                    m_serverList.insert(identityName, list);
                }
            }

            if (!m_serverList[identitySelector->currentText()].isEmpty()) {
                networkList->setCurrentRow(0);
                networkList->item(0)->setSelected(true);
                slotLoadNetworkSettings(networkList->item(0)->text());

                if (networkList->count() == 1) {
                    deleteNetworkButton->setDisabled(true);
                    serverTabs->setEnabled(true);
                } else {
                    deleteNetworkButton->setEnabled(true);
                    serverTabs->setEnabled(true);
                }

                exportNetworks->setEnabled(true);
            }
        }
    }
}

void
ServerPage::newNetworkSettings(const QString &name)
{
    // Get current identity name.
    const QString identityName = identitySelector->currentText();
    // Get the server list for the current identity.
    Aki::ServerList tmpList = m_serverList.value(identityName);
    // Create the new network.
    Aki::ServerPointer server(new Aki::Server(name));
    // Add the new network.
    networkList->addItem(name);

    // Append the prefix that is used in the configuration file.
    m_serverConfig->createGroup(identityName + QChar('-') + name);

    server->setAddressList(m_serverConfig->addressList());
    server->setAutoIdentify(m_serverConfig->isAutoIdentifyEnabled());
    server->setAutoJoinChannels(m_serverConfig->isAutoJoinChannelsEnabled());
    server->setAutoReconnection(m_serverConfig->isAutoReconnectionEnabled());
    server->setChannelList(m_serverConfig->channelList());
    server->setConnectOnStartup(m_serverConfig->isConnectOnStartupEnabled());
    server->setConnectToRandomServer(m_serverConfig->isConnectToRandomServerEnabled());
    server->setDefaultEncoding(m_serverConfig->isDefaultEncodingEnabled());
    server->setEncoding(m_serverConfig->encoding());
    server->setRetryAttempts(m_serverConfig->retryAttemptCount());
    server->setRetryInterval(m_serverConfig->retryInterval());
    server->setServiceName(m_serverConfig->serviceName());
    server->setServicePassword(m_serverConfig->servicePassword());
    server->setSsl(m_serverConfig->isSslEnabled());
    // Append the new server to the server list.
    tmpList.append(server);

    // Remove the old list.
    m_serverList.remove(identityName);
    // Append the new list.
    m_serverList.insert(identityName, tmpList);

    if (networkList->count() == 1) {
        networkList->setCurrentRow(0);
        networkList->item(0)->setSelected(true);
    } else {
        const int index = networkList->row(networkList->findItems(name, Qt::MatchExactly)[0]);
        networkList->setCurrentRow(index);
        networkList->item(index)->setSelected(true);
        slotLoadNetworkSettings(networkList->item(index)->text());
        deleteNetworkButton->setEnabled(true);
        /*
        const int index = networkList->count() - 1;
        networkList->setCurrentRow(index + 1);
        networkList->item(index + 1)->setSelected(true);
        slotLoadNetworkSettings(networkList->item(index + 1)->text());
        deleteNetworkButton->setEnabled(true);
        */
    }

    editNetworkButton->setEnabled(true);
    exportNetworks->setEnabled(true);
    serverTabs->setEnabled(true);

    setChanged(true);
}

Aki::ServerPointer
ServerPage::serverNetworkForCurrentIdentity()
{
    // Get the current identity name
    const QString identityName = identitySelector->currentText();
    // Get the current network name
    const QString networkName = networkList->currentItem()->text();

    foreach (Aki::ServerPointer server, m_serverList[identityName]) {
        if (server->name() == networkName) {
            return server;
        }
    }

    return Aki::ServerPointer();
}

void
ServerPage::setChanged(bool enable)
{
    m_hasChanged = enable;

    if (m_hasChanged) {
        emit widgetsModified();
    }
}

void
ServerPage::slotIdentityActivated(const QString &name)
{
    if (m_serverList.contains(name) && !m_serverList[name].isEmpty()) {
        networkList->clear();

        QMapIterator<QString, Aki::ServerList> serverListIter(m_serverList);
        while (serverListIter.hasNext()) {
            serverListIter.next();

            if (name == serverListIter.key()) {
                QListIterator<Aki::ServerPointer> networkListIter(serverListIter.value());
                while (networkListIter.hasNext()) {
                    Aki::ServerPointer server = networkListIter.next();
                    QString tmp = server->name().remove(name + QChar('-'));
                    networkList->addItem(tmp);
                }

                networkList->sortItems();
                networkList->setCurrentRow(0);
                networkList->item(0)->setSelected(true);
                slotLoadNetworkSettings(networkList->item(0)->text());

                if (networkList->count() > 1) {
                    deleteNetworkButton->setEnabled(true);
                }
            }
        }

        importNetworks->setEnabled(true);
        exportNetworks->setEnabled(true);
        editNetworkButton->setEnabled(true);
        serverTabs->setEnabled(true);
    } else {
        networkList->clear();
        exportNetworks->setDisabled(true);
        editNetworkButton->setDisabled(true);
        deleteNetworkButton->setDisabled(true);
        serverTabs->setDisabled(true);
    }
    
    if (networkList->count() == 1) {
        deleteNetworkButton->setDisabled(true);
    }
}

void
ServerPage::slotLoadNetworkSettings(const QString &name)
{
    // Get the current identity name.
    const QString identityName = identitySelector->currentText();
    // Get the server list for the current identity.
    Aki::ServerList tmpList = m_serverList.value(identityName);

    QListIterator<Aki::ServerPointer> serverListIter(tmpList);
    while (serverListIter.hasNext()) {
        Aki::ServerPointer server = serverListIter.next();
        if (server->name() == name) {
            useReconnection->setChecked(server->isAutoReconnectionEnabled());
            retryInterval->setValue(server->retryInterval());
            retryAttempts->setValue(server->retryAttemptCount());
            connectOnStartup->setChecked(server->isConnectOnStartupEnabled());
            useSSL->setChecked(server->isSslEnabled());
            serverList->setItems(server->addressList());
            connectToRandomServer->setChecked(server->isConnectToRandomServerEnabled());
            channelList->setItems(server->channelList());
            autoJoinChannels->setChecked(server->isAutoJoinChannelsEnabled());
            useDefaultEncoding->setChecked(server->isDefaultEncodingEnabled());
            defaultEncoding->setCurrentIndex(defaultEncoding->findText(server->encoding()));
            useAutoIdentify->setChecked(server->isAutoIdentifyEnabled());
            serviceName->setText(server->serviceName());
            servicePassword->setText(server->servicePassword());
            return;
        }
    }
}

void
ServerPage::slotAutoJoinChannelsClicked(bool checked)
{
    serverNetworkForCurrentIdentity()->setAutoJoinChannels(checked);
    setChanged(true);
}

void
ServerPage::slotChannelListChanged()
{
    serverNetworkForCurrentIdentity()->setChannelList(channelList->items());
    setChanged(true);
}

void
ServerPage::slotConnectOnStartupClicked(bool checked)
{
    serverNetworkForCurrentIdentity()->setConnectOnStartup(checked);
    setChanged(true);
}

void
ServerPage::slotConnectToRandomServerClicked(bool checked)
{
    serverNetworkForCurrentIdentity()->setConnectToRandomServer(checked);
    setChanged(true);
}

void
ServerPage::slotDefaultEncodingIndexChanged(const QString &encodingName)
{
    serverNetworkForCurrentIdentity()->setEncoding(encodingName);
    setChanged(true);
}

void
ServerPage::slotRetryAttemptsValueChanged(int count)
{
    serverNetworkForCurrentIdentity()->setRetryAttempts(count);
    setChanged(true);
}

void
ServerPage::slotRetryIntervalValueChanged(int seconds)
{
    serverNetworkForCurrentIdentity()->setRetryInterval(seconds);
    setChanged(true);
}

void
ServerPage::slotServerListChanged()
{
    serverNetworkForCurrentIdentity()->setAddressList(serverList->items());
    setChanged(true);
}

void
ServerPage::slotServiceNameTextEdited(const QString &name)
{
    serverNetworkForCurrentIdentity()->setServiceName(name);
    setChanged(true);
}

void
ServerPage::slotServicePasswordTextEdited(const QString &password)
{
    serverNetworkForCurrentIdentity()->setServicePassword(password);
    setChanged(true);
}

void
ServerPage::slotUseAutoIdentifyClicked(bool checked)
{
    serverNetworkForCurrentIdentity()->setAutoIdentify(checked);
    setChanged(true);
}

void
ServerPage::slotUseDefaultEncodingClicked(bool checked)
{
    serverNetworkForCurrentIdentity()->setDefaultEncoding(checked);
    setChanged(true);
}

void
ServerPage::slotUseReconnectionClicked(bool checked)
{
    serverNetworkForCurrentIdentity()->setAutoReconnection(checked);
    setChanged(true);
}

void
ServerPage::slotUseSSLClicked(bool checked)
{
    serverNetworkForCurrentIdentity()->setSsl(checked);
    setChanged(true);
}

void
ServerPage::slotExportNetworksClicked()
{
    const QString identityName = identitySelector->currentText();
    const QString fileName = KFileDialog::getSaveFileName(KUrl(), "*.xml|" +
        i18n("Network file(*.xml)"), this, i18n("Save Network File"));

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    Aki::NetworkExporter *exporter = new Aki::NetworkExporter;
    exporter->setServerList(m_serverList[identityName]);

    if (!exporter->write(&file)) {
        KMessageBox::error(this, i18n("Unable to save networks to file."));
    }
}

void
ServerPage::slotImportNetworksClicked()
{
    const QString identityName = identitySelector->currentText();
    const QString fileName = KFileDialog::getOpenFileName(KUrl(), "*.xml|" +
        i18n("Network file(*.xml)"), this, i18n("Open Network File"));

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    Aki::NetworkImporter *import = new Aki::NetworkImporter;
    if (!import->read(&file)) {
        KMessageBox::error(this, i18n("Unable to load network file: %1", fileName));
        return;
    }

    Aki::ServerList list = import->serverList();
    if (list.isEmpty()) {
        return;
    }

    networkList->clear();

    foreach (Aki::ServerPointer s, list) {
        networkList->addItem(s->name());
    }

    m_serverList.remove(identityName);
    m_serverList.insert(identityName, list);

    networkList->setCurrentRow(0);
    networkList->item(0)->setSelected(true);

    serverTabs->setEnabled(true);
}
