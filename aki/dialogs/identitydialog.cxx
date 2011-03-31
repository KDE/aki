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
#include "sql/nickname.hpp"
#include "ui/nicknamewidget.hpp"
#include <KDE/KInputDialog>
#include <KDE/KMessageBox>
#include <KDE/KUser>
using namespace Aki;

IdentityDialog::IdentityDialog(Aki::Sql::Database* database, Aki::Sql::Identity* identity, QWidget* parent)
    : KDialog(parent),
    _database(database)
{
    Q_UNUSED(identity);
    setAttribute(Qt::WA_DeleteOnClose);

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
    _identityComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _identityComboBox->repopulateIdentities();

    _addIdentity = new QToolButton;
    identityToolBarLayout->addWidget(_addIdentity);
    connect(_addIdentity, SIGNAL(triggered(QAction*)),
            SLOT(slotAddIdentityTriggered()));
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
    connect(_realName, SIGNAL(textEdited(QString)),
            SLOT(slotRealNameTextEdited(QString)));
    realNameLabel->setBuddy(_realName);
    _realName->setClearButtonShown(true);

    _nicknameWidget = new Aki::NicknameWidget;
    generalTabLayout->setWidget(1, QFormLayout::SpanningRole, _nicknameWidget);
    _nicknameWidget->setDatabase(_database);

    QWidget* awayTab = new QWidget;
    identityTabs->addTab(awayTab, i18nc("When the user is marked away", "Away"));

    QFormLayout* awayTabLayout = new QFormLayout;
    awayTab->setLayout(awayTabLayout);

    _markLastPositionCheckBox = new QCheckBox;
    awayTabLayout->setWidget(0, QFormLayout::SpanningRole, _markLastPositionCheckBox);
    connect(_markLastPositionCheckBox, SIGNAL(clicked(bool)),
            SLOT(slotMarkLastPositionClicked(bool)));
    _markLastPositionCheckBox->setText(i18n("Mark last position when away"));

    QLabel* awayNicknameLabel = new QLabel;
    awayTabLayout->setWidget(1, QFormLayout::LabelRole, awayNicknameLabel);
    awayNicknameLabel->setText(i18n("Away nickname:"));

    _awayNicknameLineEdit = new KLineEdit;
    awayTabLayout->setWidget(1, QFormLayout::FieldRole, _awayNicknameLineEdit);
    connect(_awayNicknameLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotAwayNicknameTextEdited(QString)));
    awayNicknameLabel->setBuddy(_awayNicknameLineEdit);
    _awayNicknameLineEdit->setClearButtonShown(true);

    _awayMessagesGroupBox = new QGroupBox;
    awayTabLayout->setWidget(2, QFormLayout::SpanningRole, _awayMessagesGroupBox);
    connect(_awayMessagesGroupBox, SIGNAL(clicked(bool)),
            SLOT(slotAwayMessagesClicked(bool)));
    _awayMessagesGroupBox->setTitle(i18n("Away Messages"));
    _awayMessagesGroupBox->setCheckable(true);
    _awayMessagesGroupBox->setChecked(false);

    QFormLayout* awayMessageGroupBoxLayout = new QFormLayout;
    _awayMessagesGroupBox->setLayout(awayMessageGroupBoxLayout);

    QLabel* awayMessageLabel = new QLabel;
    awayMessageGroupBoxLayout->setWidget(0, QFormLayout::LabelRole, awayMessageLabel);
    awayMessageLabel->setText(i18n("Away message:"));

    _awayMessageLineEdit = new KLineEdit;
    awayMessageGroupBoxLayout->setWidget(0, QFormLayout::FieldRole, _awayMessageLineEdit);
    connect(_awayMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotAwayMessageTextEdited(QString)));
    awayMessageLabel->setBuddy(_awayMessageLineEdit);
    _awayMessageLineEdit->setClearButtonShown(true);

    QLabel* returnMessageLabel = new QLabel;
    awayMessageGroupBoxLayout->setWidget(1, QFormLayout::LabelRole, returnMessageLabel);
    returnMessageLabel->setText(i18n("Return message:"));

    _returnMessageLineEdit = new KLineEdit;
    awayMessageGroupBoxLayout->setWidget(1, QFormLayout::FieldRole, _returnMessageLineEdit);
    connect(_returnMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotReturnMessageTextEdited(QString)));
    returnMessageLabel->setBuddy(_returnMessageLineEdit);
    _returnMessageLineEdit->setClearButtonShown(true);

    QWidget* miscTab = new QWidget;
    identityTabs->addTab(miscTab, i18n("Misc."));

    QFormLayout* miscTabLayout = new QFormLayout;
    miscTab->setLayout(miscTabLayout);

    QLabel* kickMessageLabel = new QLabel;
    miscTabLayout->setWidget(0, QFormLayout::LabelRole, kickMessageLabel);
    kickMessageLabel->setText(i18n("Kick message:"));
    kickMessageLabel->setWhatsThis(i18n("Sends this message instead of the default message, when you kick someone."));

    _kickMessageLineEdit = new KLineEdit;
    miscTabLayout->setWidget(0, QFormLayout::FieldRole, _kickMessageLineEdit);
    connect(_kickMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotKickMessageTextEdited(QString)));
    kickMessageLabel->setBuddy(_kickMessageLineEdit);
    _kickMessageLineEdit->setClearButtonShown(true);
    _kickMessageLineEdit->setWhatsThis(i18n("Sends this message instead of the default message, when you kick someone."));

    QLabel* partMessageLabel = new QLabel;
    miscTabLayout->setWidget(1, QFormLayout::LabelRole, partMessageLabel);
    partMessageLabel->setText(i18n("Part message:"));

    _partMessageLineEdit = new KLineEdit;
    miscTabLayout->setWidget(1, QFormLayout::FieldRole, _partMessageLineEdit);
    connect(_partMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotPartMessageTextEdited(QString)));
    partMessageLabel->setBuddy(_partMessageLineEdit);
    _partMessageLineEdit->setClearButtonShown(true);
    _partMessageLineEdit->setWhatsThis(i18n("Displays this message when you use /part <channel> or when you close the channel's tab."));

    QLabel* quitMessageLabel = new QLabel;
    miscTabLayout->setWidget(2, QFormLayout::LabelRole, quitMessageLabel);
    quitMessageLabel->setText(i18n("Quit message:"));

    _quitMessageLineEdit = new KLineEdit;
    miscTabLayout->setWidget(2, QFormLayout::FieldRole, _quitMessageLineEdit);
    connect(_quitMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotQuitMessageTextEdited(QString)));
    quitMessageLabel->setBuddy(_quitMessageLineEdit);
    _quitMessageLineEdit->setClearButtonShown(true);
    _quitMessageLineEdit->setWhatsThis(i18n("Displays this message when you use /quit in the channel."));

    if (_identityComboBox->count() > 0) {
        populateWidgets(_identityComboBox->identity(0));
    }
}

IdentityDialog::~IdentityDialog()
{
}

void
IdentityDialog::slotAddIdentityTriggered()
{
    bool ok = false;
    const QString name = KInputDialog::getText(i18n("Enter a name for the new identity"), i18n("Please enter a name for the new identity:"), QString(), &ok, this);

    if (!ok) {
        return;
    }

    if (_identityComboBox->contains(name)) {
        KMessageBox::error(this, i18n("Unable to create a new identity. Name already exists."), i18n("Identity already exists."));
        slotAddIdentityTriggered();
        return;
    }
}

void
IdentityDialog::populateWidgets(Aki::Sql::Identity* identity)
{
    _realName->setText(identity->realName());

    const QList<Aki::Sql::Nickname*> nicknameList = _database->find<Aki::Sql::Nickname>()
                                                        .where("nicknameIdentity = :nicknameIdentity").bind("nicknameIdentity", identity->id())
                                                        .result();
    foreach (Aki::Sql::Nickname* nickname, nicknameList) {
        _nicknameWidget->addNickname(nickname);
    }

    _markLastPositionCheckBox->setChecked(identity->isMarkLastPositionEnabled());
    _awayNicknameLineEdit->setText(identity->awayNickname());
    _awayMessageLineEdit->setText(identity->awayMessage());
    _returnMessageLineEdit->setText(identity->returnMessage());
    _awayMessagesGroupBox->setChecked(identity->isMessagesEnabled());
    _kickMessageLineEdit->setText(identity->kickMessage());
    _partMessageLineEdit->setText(identity->partMessage());
    _quitMessageLineEdit->setText(identity->quitMessage());
}

void
IdentityDialog::slotAwayMessagesClicked(bool clicked)
{
    _identityComboBox->currentIdentity()->setMessagesEnabled(clicked);
}

void
IdentityDialog::slotAwayMessageTextEdited(const QString& message)
{
    _identityComboBox->currentIdentity()->setAwayMessage(message);
}

void
IdentityDialog::slotAwayNicknameTextEdited(const QString& nickname)
{
    _identityComboBox->currentIdentity()->setAwayNickname(nickname);
}

void
IdentityDialog::slotKickMessageTextEdited(const QString& message)
{
    _identityComboBox->currentIdentity()->setKickMessage(message);
}

void
IdentityDialog::slotMarkLastPositionClicked(bool clicked)
{
    _identityComboBox->currentIdentity()->setMarkLastPositionEnabled(clicked);
}

void
IdentityDialog::slotPartMessageTextEdited(const QString& message)
{
    _identityComboBox->currentIdentity()->setPartMessage(message);
}

void
IdentityDialog::slotQuitMessageTextEdited(const QString& message)
{
    _identityComboBox->currentIdentity()->setQuitMessage(message);
}

void
IdentityDialog::slotRealNameTextEdited(const QString& name)
{
    if (name.isEmpty()) {
        return;
    }

    _identityComboBox->currentIdentity()->setRealName(name);
}

void
IdentityDialog::slotReturnMessageTextEdited(const QString& message)
{
    _identityComboBox->currentIdentity()->setReturnMessage(message);
}
