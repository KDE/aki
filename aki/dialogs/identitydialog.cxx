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

#include "identitydialog.hpp"
#include "sql/database.hpp"
#include "sql/identity.hpp"
#include "ui/nicknamewidget.hpp"
#include <KDE/KInputDialog>
#include <KDE/KMessageBox>
#include <KDE/KUser>
using namespace Aki;

IdentityDialog::IdentityDialog(QWidget* parent)
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
    identityLabel->setText(i18n("Identity: "));

    QWidget* identityToolBar = new QWidget;
    mainLayout->setWidget(0, QFormLayout::FieldRole, identityToolBar);
    identityLabel->setBuddy(identityToolBar);

    QHBoxLayout* identityToolBarLayout = new QHBoxLayout;
    identityToolBar->setLayout(identityToolBarLayout);

    _identityComboBox = new Aki::IdentityComboBox;
    identityToolBarLayout->addWidget(_identityComboBox);
    _identityComboBox->setDatabase(_database);

    _addIdentity = new QToolButton;
    identityToolBarLayout->addWidget(_addIdentity);
    _addIdentity->setIcon(KIcon("list-add-user"));
    _addIdentity->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _addIdentity->setWhatsThis(i18n("Adds a new identity"));

    _copyIdentity = new QToolButton;
    identityToolBarLayout->addWidget(_copyIdentity);
    _copyIdentity->setIcon(KIcon("list-copy"));
    _copyIdentity->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _copyIdentity->setWhatsThis(i18n("Copies the current identity and creates a new identity with the same information"));

    _editIdentity = new QToolButton;
    identityToolBarLayout->addWidget(_editIdentity);
    _editIdentity->setIcon(KIcon("edit-rename"));
    _editIdentity->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _editIdentity->setWhatsThis(i18n("Edits the current identity's name"));

    _removeIdentity = new QToolButton;
    identityToolBarLayout->addWidget(_removeIdentity);
    _removeIdentity->setIcon(KIcon("list-remove-user"));
    _removeIdentity->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _removeIdentity->setWhatsThis(i18n("Removes the current identity and deletes it"));

    KTabWidget* identityTabs = new KTabWidget;
    mainLayout->setWidget(1, QFormLayout::SpanningRole, identityTabs);

    QWidget* generalTab = new QWidget;
    identityTabs->addTab(generalTab, i18nc("General objects", "General"));

    QFormLayout* generalTabLayout = new QFormLayout;
    generalTab->setLayout(generalTabLayout);

    QLabel* realNameLabel = new QLabel;
    generalTabLayout->setWidget(0, QFormLayout::LabelRole, realNameLabel);
    realNameLabel->setText(i18n("Real name:"));

    _realName = new KLineEdit;
    generalTabLayout->setWidget(0, QFormLayout::FieldRole, _realName);
    _realName->setClearButtonShown(true);

    _nicknameWidget = new Aki::NicknameWidget;
    generalTabLayout->setWidget(1, QFormLayout::SpanningRole, _nicknameWidget);
    _nicknameWidget->setDatabase(_database);

    QWidget* awayTab = new QWidget;
    identityTabs->addTab(awayTab, i18nc("When the user is marked away", "Away"));

    QWidget* miscTab = new QWidget;
    identityTabs->addTab(miscTab, i18n("Misc."));
}
IdentityDialog::~IdentityDialog()
{
}
