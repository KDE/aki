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
#include "sql/server.hpp"
#include "ui/serverlist.hpp"
#include <QtGui/QHBoxLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <KDE/KLocale>
#include <KDE/KMessageBox>
#include <KDE/KPushButton>
using namespace Aki;

ServerWidget::ServerWidget(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    setLayout(mainLayout);

    QVBoxLayout* buttonLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);

    _addButton = new KPushButton;
    buttonLayout->addWidget(_addButton);
    _addButton->setEnabled(true);
    _addButton->setIcon(KIcon("list-add"));
    _addButton->setText(i18n("Add"));

    _removeButton = new KPushButton;
    buttonLayout->addWidget(_removeButton);
    _removeButton->setDisabled(true);
    _removeButton->setIcon(KIcon("list-remove"));
    _removeButton->setText(i18n("Remove"));

    _editButton = new KPushButton;
    buttonLayout->addWidget(_editButton);
    _editButton->setDisabled(true);
    _editButton->setIcon(KIcon("edit-rename"));
    _editButton->setText(i18n("Edit"));

    _importButton = new KPushButton;
    buttonLayout->addWidget(_importButton);
    _importButton->setEnabled(true);
    _importButton->setText(i18n("Import Servers"));

    _exportButton = new KPushButton;
    buttonLayout->addWidget(_exportButton);
    _exportButton->setDisabled(true);
    _exportButton->setText(i18n("Export Servers"));

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

Aki::Sql::Server*
ServerWidget::takeServer(int index)
{
    return _serverList->takeServer(index);
}

#include "ui/serverwidget.moc"
