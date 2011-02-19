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

#include "serverwidget.hpp"
#include "sql/database.hpp"
#include "sql/identity.hpp"
#include "sql/server.hpp"
#include "ui/serverlist.hpp"
#include <QtGui/QHBoxLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <KDE/KInputDialog>
#include <KDE/KLocale>
#include <KDE/KMessageBox>
#include <KDE/KPushButton>
using namespace Aki;

ServerWidget::ServerWidget(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    setLayout(mainLayout);

    _serverList = new Aki::ServerList;
    mainLayout->addWidget(_serverList);
    connect(_serverList, SIGNAL(serverClicked(Aki::Sql::Server*)),
            SLOT(slotServerListClicked(Aki::Sql::Server*)));
    connect(_serverList, SIGNAL(serverCurrentRowChanged(int)),
            SLOT(slotServerCurrentRowChanged(int)));

    QVBoxLayout* buttonLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);

    _addButton = new KPushButton;
    buttonLayout->addWidget(_addButton);
    _addButton->setEnabled(true);
    _addButton->setIcon(KIcon("list-add"));
    _addButton->setText(i18n("Add"));
    connect(_addButton, SIGNAL(clicked(bool)),
            SLOT(slotAddClicked()));

    _removeButton = new KPushButton;
    buttonLayout->addWidget(_removeButton);
    _removeButton->setDisabled(true);
    _removeButton->setIcon(KIcon("list-remove"));
    _removeButton->setText(i18n("Remove"));
    connect(_removeButton, SIGNAL(clicked(bool)),
            SLOT(slotRemoveClicked()));

    _editButton = new KPushButton;
    buttonLayout->addWidget(_editButton);
    _editButton->setDisabled(true);
    _editButton->setIcon(KIcon("edit-rename"));
    _editButton->setText(i18n("Edit"));
    connect(_editButton, SIGNAL(clicked(bool)),
            SLOT(slotEditClicked()));

    _importButton = new KPushButton;
    buttonLayout->addWidget(_importButton);
    _importButton->setEnabled(true);
    _importButton->setText(i18n("Import Servers..."));
    connect(_importButton, SIGNAL(clicked(bool)),
            SLOT(slotImportClicked()));

    _exportButton = new KPushButton;
    buttonLayout->addWidget(_exportButton);
    _exportButton->setDisabled(true);
    _exportButton->setText(i18n("Export Servers..."));
    connect(_exportButton, SIGNAL(clicked(bool)),
            SLOT(slotExportClicked()));

    buttonLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

ServerWidget::~ServerWidget()
{
}

void
ServerWidget::addServer(Aki::Sql::Server* server)
{
    _serverList->addServer(server);
}

int
ServerWidget::count() const
{
    return _serverList->count();
}

Aki::Sql::Server*
ServerWidget::currentServer() const
{
    return _serverList->currentServer();
}

void
ServerWidget::insertServer(int row, Aki::Sql::Server* server)
{
    _serverList->insertServer(row, server);
}

void
ServerWidget::removeServer(Aki::Sql::Server* server)
{
    _serverList->removeServer(row(server));
}

void
ServerWidget::repopulateServers(Aki::Sql::Identity* identity)
{
    _serverList->repopulateServers(identity);
}

int
ServerWidget::row(Aki::Sql::Server* server) const
{
    return _serverList->row(server);
}

Aki::Sql::Server*
ServerWidget::server(int index)
{
    return _serverList->server(index);
}

void
ServerWidget::setCurrentRow(int row)
{
    _serverList->setCurrentRow(row);
}

void
ServerWidget::setCurrentServer(Aki::Sql::Server* server)
{
    _serverList->setCurrentServer(server);
}

void
ServerWidget::setDatabase(Aki::Sql::Database* database)
{
    _serverList->setDatabase(database);
}

void
ServerWidget::slotAddClicked()
{
    bool ok;
    const QString server = KInputDialog::getText(i18n("Add New Server"), i18n("Enter a new network name"),
                                                 QString(), &ok, this);
    if (!ok) {
        return;
    }

    if (!_serverList->findItems(server, Qt::MatchExactly).isEmpty()) {
        KMessageBox::error(this, i18n("Unable to add network. Network already exists."));
        slotAddClicked();
        return;
    } else {
        Aki::Sql::Server* tmp = new Aki::Sql::Server;
        tmp->setName(server);
        addServer(tmp);
    }

    if (count() == 1) {
        setCurrentRow(0);
        slotServerCurrentRowChanged(0);
        _removeButton->setEnabled(true);
        _editButton->setEnabled(true);
        _exportButton->setEnabled(true);
    } else {
        _editButton->setEnabled(true);
        _exportButton->setEnabled(true);
    }
}

void
ServerWidget::slotEditClicked()
{
    Aki::Sql::Server* current = _serverList->currentServer();

    bool ok = false;
    const QString server = KInputDialog::getText(i18n("Edit Current Server"), i18n("Edit current server"),
                                                 current->name(), &ok, this);
    if (!ok) {
        return;
    }

    if (current->name() == server) {
        return;
    } else if (_serverList->findItems(server, Qt::MatchExactly).isEmpty()) {
        current->setName(server);
        return;
    } else {
        KMessageBox::error(this, i18n("Unable to modify server. Server already exists."));
        slotEditClicked();
    }
}

void
ServerWidget::slotExportClicked()
{
}

void
ServerWidget::slotImportClicked()
{
}

void
ServerWidget::slotRemoveClicked()
{
    Aki::Sql::Server* current = _serverList->currentServer();

    const int result = KMessageBox::warningYesNo(this, i18n("Are you sure you want to remove the server\n\"%1\"?",
                                                            current->name()), i18n("Remove server"));
    switch (result) {
    case KMessageBox::Yes: {
        removeServer(current);
        if (count() == 0) {
            _editButton->setDisabled(true);
            _removeButton->setDisabled(true);
            _exportButton->setDisabled(true);
        }
        break;
    }
    default: {
        return;
    }
    }
}

void
ServerWidget::slotServerCurrentRowChanged(int row)
{
    Q_UNUSED(row)
}

void
ServerWidget::slotServerListClicked(Aki::Sql::Server* server)
{
    if (!server) {
        return;
    }

    _editButton->setEnabled(true);
    _removeButton->setEnabled(true);

    emit serverChanged(server);
}

Aki::Sql::Server*
ServerWidget::takeServer(int index)
{
    return _serverList->takeServer(index);
}

#include "ui/serverwidget.moc"
