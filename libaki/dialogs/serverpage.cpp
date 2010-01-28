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
#include "serverlist/networkserializer.h"
#include "settings.h"
#include "server.h"
#include <KFileDialog>
#include <KInputDialog>
#include <KMessageBox>
#include <QTextCodec>
using namespace Aki;

namespace Aki
{
class ServerPagePrivate
{
public:
    ServerPagePrivate(Aki::ServerPage *qq)
        : q(qq),
        serverConfig(0),
        identityConfig(0),
        hasChanged(false)
    {
        serverList.clear();
    }

    void loadSettings()
    {
        // Check and see if the server configuration is empty.
        if (!serverConfig->groupList().isEmpty()) {
            QStringListIterator identIter(identityConfig->groupList());
            // Load identities.
            while (identIter.hasNext()) {
                const QString identityName = identIter.next();
                Aki::ServerList list;

                QStringListIterator serverIter(serverConfig->groupList());
                while (serverIter.hasNext()) {
                    QString network = serverIter.next();
                    // Check and make sure we only continue if the identity has servers.
                    // If it doesn't then we can skip them.
                    if (network.contains(identityName + QChar('-'))) {

                        // Set the current network.
                        serverConfig->setCurrentGroup(network);

                        // Remove the configuration prefix on the group.
                        network.remove(identityName + QChar('-'));

                        // If the current identity is the identity being loading
                        // We load it to the network list else we ignore it.
                        if (q->identitySelector->currentText() == identityName) {
                            q->networkList->addItem(network);
                        }

                        q->networkList->sortItems();

                        Aki::ServerPointer server(new Aki::Server(network));
                        server->setAddressList(serverConfig->addressList());
                        server->setAutoIdentify(serverConfig->isAutoIdentifyEnabled());
                        server->setAutoJoinChannels(serverConfig->isAutoJoinChannelsEnabled());
                        server->setAutoReconnection(serverConfig->isAutoReconnectionEnabled());
                        server->setChannelList(serverConfig->channelList());
                        server->setConnectOnStartup(serverConfig->isConnectOnStartupEnabled());
                        server->setConnectToRandomServer(serverConfig->isConnectToRandomServerEnabled());
                        server->setDefaultEncoding(serverConfig->isDefaultEncodingEnabled());
                        server->setEncoding(serverConfig->encoding());
                        server->setRetryAttempts(serverConfig->retryAttemptCount());
                        server->setRetryInterval(serverConfig->retryInterval());
                        server->setServiceName(serverConfig->serviceName());
                        server->setServicePassword(serverConfig->servicePassword());
                        server->setSsl(serverConfig->isSslEnabled());

                        list << server;
                    }
                    if (!list.isEmpty()) {
                        serverList.insert(identityName, list);
                    }
                }

                if (!serverList[q->identitySelector->currentText()].isEmpty()) {
                    q->networkList->setCurrentRow(0);
                    q->networkList->item(0)->setSelected(true);
                    loadNetworkSettings(q->networkList->item(0)->text());

                    if (q->networkList->count() == 1) {
                        q->deleteNetworkButton->setDisabled(true);
                        q->serverTabs->setEnabled(true);
                    } else {
                        q->deleteNetworkButton->setEnabled(true);
                        q->serverTabs->setEnabled(true);
                    }

                    q->exportNetworks->setEnabled(true);
                }
            }
        }
    }

    void addNetworkClicked()
    {
        bool ok = false;
        QString network = KInputDialog::getText(i18n("Add new network"), i18n("Enter a new network name."),
                                                QString(), &ok, q);

        // Check to see that the network is not empty and that we found a match.
        if (ok && !network.isEmpty() &&
            !q->networkList->findItems(network, Qt::MatchExactly).isEmpty()) {
            // This network already exists so we do nothing except make the
            // user re-enter a different one.
            KMessageBox::error(q, i18n("Unable to add network. Network already exists."));
            addNetworkClicked();
        } else if (ok && !network.isEmpty() && // Network isn't empty and there isn't one that exists.
                q->networkList->findItems(network, Qt::MatchExactly).isEmpty()) {
            newNetworkSettings(network);
            q->setChanged(true);
        } else if (ok && network.isEmpty()) { // Empty network name.
            KMessageBox::error(q, i18n("Network name is empty."));
            addNetworkClicked();
        }
    }

    void deleteNetworkClicked()
    {
        // Get the current item.
        QListWidgetItem *item = q->networkList->currentItem();

        // Get the identityName.
        const QString identityName = q->identitySelector->currentText();

        // Make sure it is valid (one is selected).
        if (item) {
            // Append the correct name that is in the configuration file.
            // And delete the group
            serverConfig->deleteGroup(identityName + QChar('-') + item->text());

            // Search for network and remove it from the list.
            foreach (const Aki::ServerPointer &server, serverList[identityName]) {
                // Does this server match the selected server.
                if (server->name() == item->text()) {
                    const int index = serverList[identityName].indexOf(server);
                    serverList[identityName].removeAt(index);
                    break;
                }
            }

            // Delete the network widget from the list.
            delete q->networkList->takeItem(q->networkList->row(item));
            q->setChanged(true);
        }

        // Disable the delete button when there is only 1 there.
        if (q->networkList->count() == 1) {
            q->deleteNetworkButton->setDisabled(true);
        }
    }

    void editNetworkClicked()
    {
        bool ok = false;

        // Get the current item.
        QListWidgetItem *item = q->networkList->currentItem();
        // Store the old name.
        const QString oldName = item->text();
        // Get the current identity name.
        const QString identityName = q->identitySelector->currentText();

        // Get the new network name.
        QString newName = KInputDialog::getText(i18n("Edit current network."),
                                                i18n("Enter a new network name."), oldName, &ok, q);

        // Check to see if the new network name is not empty and the old name
        // doesn't match the new name.
        if (ok && !newName.isEmpty() && oldName != newName) {
            // Since it not empty and they don't match
            // Make sure the new one isn't already taken.
            if (q->networkList->findItems(newName, Qt::MatchExactly).isEmpty()) {
                // Append the correct name that is in the configuration file.
                // And delete the group
                serverConfig->deleteGroup(identityName + QChar('-') + oldName);

                // Append the correct name that will be in the configuration file.
                // And create the group.
                serverConfig->createGroup(identityName + QChar('-') + newName);

                // Search for the network that matches the old name
                // And update it.
                foreach (const Aki::ServerPointer &server, serverList[identityName]) {
                    // Does the server name match the old name?
                    if (server->name() == oldName) {
                        const int index = serverList[identityName].indexOf(server);
                        // Update the old name to the new name now.
                        serverList[identityName].value(index)->setName(newName);
                        // Update it in the list widget now.
                        item->setText(newName);
                        q->setChanged(true);
                    }
                }
            } else { // The new network name already exist.
                KMessageBox::error(q, i18n("Unable to edit network. Network already exists."));
                editNetworkClicked();
            }
        } else if (ok && newName.isEmpty()) { // The new network name is empty
            KMessageBox::error(q, i18n("Network name cannot be empty."));
            editNetworkClicked();
        } else if (ok && !newName.isEmpty() && newName == oldName) {
            // The new network name is the same as the old one.
            KMessageBox::error(q, i18n("New network name cannot be the same as the old one."));
            editNetworkClicked();
        }
    }

    void newNetworkSettings(const QString &name)
    {
        // Get current identity name.
        const QString identityName = q->identitySelector->currentText();
        // Get the server list for the current identity.
        Aki::ServerList tmpList = serverList.value(identityName);
        // Create the new network.
        Aki::ServerPointer server(new Aki::Server(name));
        // Add the new network.
        q->networkList->addItem(name);

        // Append the prefix that is used in the configuration file.
        serverConfig->createGroup(identityName + QChar('-') + name);

        server->setAddressList(serverConfig->addressList());
        server->setAutoIdentify(serverConfig->isAutoIdentifyEnabled());
        server->setAutoJoinChannels(serverConfig->isAutoJoinChannelsEnabled());
        server->setAutoReconnection(serverConfig->isAutoReconnectionEnabled());
        server->setChannelList(serverConfig->channelList());
        server->setConnectOnStartup(serverConfig->isConnectOnStartupEnabled());
        server->setConnectToRandomServer(serverConfig->isConnectToRandomServerEnabled());
        server->setDefaultEncoding(serverConfig->isDefaultEncodingEnabled());
        server->setEncoding(serverConfig->encoding());
        server->setRetryAttempts(serverConfig->retryAttemptCount());
        server->setRetryInterval(serverConfig->retryInterval());
        server->setServiceName(serverConfig->serviceName());
        server->setServicePassword(serverConfig->servicePassword());
        server->setSsl(serverConfig->isSslEnabled());
        // Append the new server to the server list.
        tmpList.append(server);

        // Remove the old list.
        serverList.remove(identityName);
        // Append the new list.
        serverList.insert(identityName, tmpList);

        if (q->networkList->count() == 1) {
            q->networkList->setCurrentRow(0);
            q->networkList->item(0)->setSelected(true);
        } else {
            const int index = q->networkList->row(q->networkList->findItems(name, Qt::MatchExactly)[0]);
            q->networkList->setCurrentRow(index);
            q->networkList->item(index)->setSelected(true);
            loadNetworkSettings(q->networkList->item(index)->text());
            q->deleteNetworkButton->setEnabled(true);
        }

        q->editNetworkButton->setEnabled(true);
        q->exportNetworks->setEnabled(true);
        q->serverTabs->setEnabled(true);

        q->setChanged(true);
    }

    Aki::ServerPointer serverNetworkForCurrentIdentity()
    {
        // Get the current identity name
        const QString identityName = q->identitySelector->currentText();
        // Get the current network name
        const QString networkName = q->networkList->currentItem()->text();

        foreach (const Aki::ServerPointer &server, serverList[identityName]) {
            if (server->name() == networkName) {
                return server;
            }
        }

        return Aki::ServerPointer();
    }

    void identityActivated(const QString &name)
    {
        if (serverList.contains(name) && !serverList[name].isEmpty()) {
            q->networkList->clear();

            QMapIterator<QString, Aki::ServerList> serverListIter(serverList);
            while (serverListIter.hasNext()) {
                serverListIter.next();

                if (name == serverListIter.key()) {
                    QListIterator<Aki::ServerPointer> networkListIter(serverListIter.value());
                    while (networkListIter.hasNext()) {
                        Aki::ServerPointer server = networkListIter.next();
                        QString tmp = server->name().remove(name + QChar('-'));
                        q->networkList->addItem(tmp);
                    }

                    q->networkList->sortItems();
                    q->networkList->setCurrentRow(0);
                    q->networkList->item(0)->setSelected(true);
                    loadNetworkSettings(q->networkList->item(0)->text());

                    if (q->networkList->count() > 1) {
                        q->deleteNetworkButton->setEnabled(true);
                    }
                }
            }

            q->importNetworks->setEnabled(true);
            q->exportNetworks->setEnabled(true);
            q->editNetworkButton->setEnabled(true);
            q->serverTabs->setEnabled(true);
        } else {
            q->networkList->clear();
            q->exportNetworks->setDisabled(true);
            q->editNetworkButton->setDisabled(true);
            q->deleteNetworkButton->setDisabled(true);
            q->serverTabs->setDisabled(true);
        }

        if (q->networkList->count() == 1) {
            q->deleteNetworkButton->setDisabled(true);
        }
    }

    void loadNetworkSettings(const QString &name)
    {
        // Get the current identity name.
        const QString identityName = q->identitySelector->currentText();
        // Get the server list for the current identity.
        Aki::ServerList tmpList = serverList.value(identityName);

        QListIterator<Aki::ServerPointer> serverListIter(tmpList);
        while (serverListIter.hasNext()) {
            Aki::ServerPointer server = serverListIter.next();
            if (server->name() == name) {
                q->useReconnection->setChecked(server->isAutoReconnectionEnabled());
                q->retryInterval->setValue(server->retryInterval());
                q->retryAttempts->setValue(server->retryAttemptCount());
                q->connectOnStartup->setChecked(server->isConnectOnStartupEnabled());
                q->useSSL->setChecked(server->isSslEnabled());
                q->serverList->setItems(server->addressList());
                q->connectToRandomServer->setChecked(server->isConnectToRandomServerEnabled());
                q->channelList->setItems(server->channelList());
                q->autoJoinChannels->setChecked(server->isAutoJoinChannelsEnabled());
                q->useDefaultEncoding->setChecked(server->isDefaultEncodingEnabled());
                q->defaultEncoding->setCurrentIndex(q->defaultEncoding->findText(server->encoding()));
                q->useAutoIdentify->setChecked(server->isAutoIdentifyEnabled());
                q->serviceName->setText(server->serviceName());
                q->servicePassword->setText(server->servicePassword());
                return;
            }
        }
    }

    void autoJoinChannelsClicked(bool checked)
    {
        serverNetworkForCurrentIdentity()->setAutoJoinChannels(checked);
        q->setChanged(true);
    }

    void channelListChanged()
    {
        serverNetworkForCurrentIdentity()->setChannelList(q->channelList->items());
        q->setChanged(true);
    }

    void connectOnStartupClicked(bool checked)
    {
        serverNetworkForCurrentIdentity()->setConnectOnStartup(checked);
        q->setChanged(true);
    }

    void connectToRandomServerClicked(bool checked)
    {
        serverNetworkForCurrentIdentity()->setConnectToRandomServer(checked);
        q->setChanged(true);
    }

    void defaultEncodingIndexChanged(const QString &encodingName)
    {
        serverNetworkForCurrentIdentity()->setEncoding(encodingName);
        q->setChanged(true);
    }

    void retryAttemptsValueChanged(int count)
    {
        serverNetworkForCurrentIdentity()->setRetryAttempts(count);
        q->setChanged(true);
    }

    void retryIntervalValueChanged(int seconds)
    {
        serverNetworkForCurrentIdentity()->setRetryInterval(seconds);
        q->setChanged(true);
    }

    void serverListChanged()
    {
        serverNetworkForCurrentIdentity()->setAddressList(q->serverList->items());
        q->setChanged(true);
    }

    void serviceNameTextEdited(const QString &name)
    {
        serverNetworkForCurrentIdentity()->setServiceName(name);
        q->setChanged(true);
    }

    void servicePasswordTextEdited(const QString &password)
    {
        serverNetworkForCurrentIdentity()->setServicePassword(password);
        q->setChanged(true);
    }

    void useAutoIdentifyClicked(bool checked)
    {
        serverNetworkForCurrentIdentity()->setAutoIdentify(checked);
        q->setChanged(true);
    }

    void useDefaultEncodingClicked(bool checked)
    {
        serverNetworkForCurrentIdentity()->setDefaultEncoding(checked);
        q->setChanged(true);
    }

    void useReconnectionClicked(bool checked)
    {
        serverNetworkForCurrentIdentity()->setAutoReconnection(checked);
        q->setChanged(true);
    }

    void useSSLClicked(bool checked)
    {
        serverNetworkForCurrentIdentity()->setSsl(checked);
        q->setChanged(true);
    }

    void exportNetworksClicked()
    {
        const QString identityName = q->identitySelector->currentText();
        const QString fileName = KFileDialog::getSaveFileName(KUrl(), "*.xml|" +
            i18n("Network file(*.xml)"), q, i18n("Save Network File"));

        Aki::NetworkSerializer *file = new Aki::NetworkSerializer(q);
        file->setServerList(serverList[identityName]);
        if (!file->write(fileName)) {
            KMessageBox::error(q, i18n("Unable to save file %1", fileName));
            return;
        }

        /*Aki::NetworkExporter *exporter = new Aki::NetworkExporter;
        exporter->setServerList(serverList[identityName]);

        if (!exporter->write(&file)) {
            KMessageBox::error(q, i18n("Unable to save networks to file."));
        }*/
    }

    void importNetworksClicked()
    {
        const QString identityName = q->identitySelector->currentText();
        const QString fileName = KFileDialog::getOpenFileName(KUrl(), "*.xml|" +
            i18n("Network file(*.xml)\n*.ini|mIRC Server List(*.ini)"), q, i18n("Open Network File"));

        Aki::NetworkSerializer *file = new Aki::NetworkSerializer(q);
        if (!file->read(fileName)) {
            KMessageBox::error(q, i18n("Unable to import file %1", fileName));
            return;
        }

        /*QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }

        Aki::NetworkImporter *import = new Aki::NetworkImporter;
        if (!import->read(&file)) {
            KMessageBox::error(q, i18n("Unable to load network file: %1", fileName));
            return;
        }*/

        Aki::ServerList list = file->servers();
        if (list.isEmpty()) {
            kDebug() << "List is empty";
            return;
        }

        q->networkList->clear();

        foreach (const Aki::ServerPointer &s, list) {
            q->networkList->addItem(s->name());
        }

        serverList.remove(identityName);
        serverList.insert(identityName, list);

        q->networkList->setCurrentRow(0);
        q->networkList->item(0)->setSelected(true);

        q->serverTabs->setEnabled(true);
    }

    Aki::ServerPage *q;
    Aki::ServerPage::ServerMap serverList;
    Aki::ServerConfig *serverConfig;
    Aki::IdentityConfig *identityConfig;
    bool hasChanged;
}; // End of class ServerPagePrivate.
} // End of namespace Aki.

ServerPage::ServerPage(QWidget *parent)
    : Aki::SettingsPageInterface(i18n("Servers"), Aki::Settings::self(),
                                 "preferences-system-network", i18n("Configure Server Settings"),
                                 parent)
{
    d.reset(new ServerPagePrivate(this));
    setupUi(this);

    // Setup the identity and server configuration files.
    d->identityConfig = new Aki::IdentityConfig(this);
    d->serverConfig = new Aki::ServerConfig(this);

    // Setup icons for the buttons.
    addNetworkButton->setIcon(KIcon("list-add"));
    deleteNetworkButton->setIcon(KIcon("list-remove"));
    editNetworkButton->setIcon(KIcon("edit-rename"));

    QStringListIterator strIter(d->identityConfig->groupList());
    while (strIter.hasNext()) {
        identitySelector->addItem(strIter.next());
    }

    identitySelector->model()->sort(0);
    identitySelector->setCurrentIndex(0);
    d->identityActivated(identitySelector->currentText());

    // Get all available codecs.
    foreach (int mib, QTextCodec::availableMibs()) {
        defaultEncoding->addItem(QTextCodec::codecForMib(mib)->name());
    }

    defaultEncoding->model()->sort(0);

    connect(addNetworkButton, SIGNAL(clicked(bool)),
            SLOT(addNetworkClicked()));
    connect(deleteNetworkButton, SIGNAL(clicked(bool)),
            SLOT(deleteNetworkClicked()));
    connect(editNetworkButton, SIGNAL(clicked(bool)),
            SLOT(editNetworkClicked()));
    connect(identitySelector, SIGNAL(activated(QString)),
            SLOT(identityActivated(QString)));
    connect(autoJoinChannels, SIGNAL(clicked(bool)),
            SLOT(autoJoinChannelsClicked(bool)));
    connect(channelList, SIGNAL(changed()),
            SLOT(channelListChanged()));
    connect(connectOnStartup, SIGNAL(clicked(bool)),
            SLOT(connectOnStartupClicked(bool)));
    connect(connectToRandomServer, SIGNAL(clicked(bool)),
            SLOT(connectToRandomServerClicked(bool)));
    connect(defaultEncoding, SIGNAL(currentIndexChanged(QString)),
            SLOT(defaultEncodingIndexChanged(QString)));
    connect(networkList, SIGNAL(currentTextChanged(QString)),
            SLOT(loadNetworkSettings(QString)));
    connect(retryAttempts, SIGNAL(valueChanged(int)),
            SLOT(retryAttemptsValueChanged(int)));
    connect(retryInterval, SIGNAL(valueChanged(int)),
            SLOT(retryIntervalValueChanged(int)));
    connect(serverList, SIGNAL(changed()),
            SLOT(serverListChanged()));
    connect(serviceName, SIGNAL(textEdited(QString)),
            SLOT(serviceNameTextEdited(QString)));
    connect(servicePassword, SIGNAL(textEdited(QString)),
            SLOT(servicePasswordTextEdited(QString)));
    connect(useAutoIdentify, SIGNAL(clicked(bool)),
            SLOT(useAutoIdentifyClicked(bool)));
    connect(useDefaultEncoding, SIGNAL(clicked(bool)),
            SLOT(useDefaultEncodingClicked(bool)));
    connect(useReconnection, SIGNAL(clicked(bool)),
            SLOT(useReconnectionClicked(bool)));
    connect(useSSL, SIGNAL(clicked(bool)),
            SLOT(useSSLClicked(bool)));
    connect(exportNetworks, SIGNAL(clicked(bool)),
            SLOT(exportNetworksClicked()));
    connect(importNetworks, SIGNAL(clicked(bool)),
            SLOT(importNetworksClicked()));

    d->loadSettings();
}

ServerPage::~ServerPage()
{
}

bool
ServerPage::hasChanged() const
{
    return d->hasChanged;
}

void
ServerPage::updateSettings()
{
    QMapIterator<QString, Aki::ServerList> serverMapIter(d->serverList);
    while (serverMapIter.hasNext()) {
        serverMapIter.next();
        // Get current identity name.
        const QString identityName = serverMapIter.key();

        QListIterator<Aki::ServerPointer> serverListIter(serverMapIter.value());
        while (serverListIter.hasNext()) {
            Aki::ServerPointer server = serverListIter.next();

            d->serverConfig->setCurrentGroup(identityName + QChar('-') + server->name());
            d->serverConfig->setAddressList(server->addressList());
            d->serverConfig->setAutoIdentify(server->isAutoIdentifyEnabled());
            d->serverConfig->setAutoJoinChannels(server->isAutoJoinChannelsEnabled());
            d->serverConfig->setAutoReconnection(server->isAutoReconnectionEnabled());
            d->serverConfig->setChannelList(server->channelList());
            d->serverConfig->setConnectOnStartup(server->isConnectOnStartupEnabled());
            d->serverConfig->setConnectToRandomServer(server->isConnectToRandomServerEnabled());
            d->serverConfig->setDefaultEncoding(server->isDefaultEncodingEnabled());
            d->serverConfig->setEncoding(server->encoding());
            d->serverConfig->setRetryAttempts(server->retryAttemptCount());
            d->serverConfig->setRetryInterval(server->retryInterval());
            d->serverConfig->setServiceName(server->serviceName());
            d->serverConfig->setServicePassword(server->servicePassword());
            d->serverConfig->setSsl(server->isSslEnabled());
        }
    }

    d->serverConfig->save();
    setChanged(false);
}

void
ServerPage::setChanged(bool enable)
{
    d->hasChanged = enable;

    if (d->hasChanged) {
        emit widgetsModified();
    }
}

#include "serverpage.moc"
