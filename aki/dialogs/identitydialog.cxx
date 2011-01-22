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
#include <KDE/KInputDialog>
#include <KDE/KMessageBox>
#include <KDE/KUser>
using namespace Aki;

IdentityDialog::IdentityDialog(QWidget* parent)
    : KDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    _database = new Aki::Sql::Database("QSQLITE");
    _database->setDatabaseName(Aki::databaseFile());
    if (!_database->open()) {
        qDebug() << "Unable to open database";
    } else {
        qDebug() << "Database successfully opened";
    }

    if (!_database->create<Aki::Sql::Identity>()) {
        qDebug() << "Unable to create table";
    } else {
        qDebug() << "Successfully created table";
    }

    setupDialog();

    if (identitySelector->count()) {
        slotIdentityActivated(identitySelector->currentIdentity());
    }
}

IdentityDialog::~IdentityDialog()
{
    delete _database;
}

void
IdentityDialog::createNewIdentity(const QString& name)
{
    KUser user(KUser::UseRealUserID);

    QString tmp;
    if (user.property(KUser::FullName).isNull()) {
        tmp = "Aki";
    } else {
        tmp = user.property(KUser::FullName).toString();
    }

    _database->transaction();
    // Create a new identity.
    Aki::Sql::Identity* identity = new Aki::Sql::Identity;
    identity->setAwayMessage("I'm now away.");
    identity->setAwayNickname(user.loginName() + "|tmp");
    identity->setKickMessage(QString());
    identity->setMarkLastPositionEnabled(false);
    identity->setMessagesEnabled(false);
    identity->setName(name);
    identity->setPartMessage("Aki IRC Client %v");
    identity->setQuitMessage("Aki IRC Client %v");
    identity->setRealName(user.property(KUser::FullName).toString());
    identity->setReturnMessage("I'm now back");

    if (_database->add(identity)) {
        qDebug() << "Added identity: " << name;
        if (_database->commit()) {
            qDebug() << "Commit was successful on: " << name;
            identitySelector->addIdentity(identity);
            identitySelector->setCurrentIdentity(identity);
        }
    } else {
        qDebug() << "Unable to Add new identity: " << name;
        if (!_database->rollback()) {
            qDebug() << "Unable to rollback: " << name;
        } else {
            qDebug() << "Successfully rollbacked data";
        }
    }
}

void
IdentityDialog::setupActions()
{
    connect(identitySelector, SIGNAL(currentIndexChanged(Aki::Sql::Identity*)),
            SLOT(slotIdentityActivated(Aki::Sql::Identity*)));
    connect(realNameLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotRealNameTextEdited(QString)));
    connect(addIdentityButton, SIGNAL(clicked(bool)),
            SLOT(slotAddIdentityClicked()));
    connect(addNicknameButton,SIGNAL(clicked(bool)),
            SLOT(slotAddNicknameClicked()));
    connect(removeNicknameButton, SIGNAL(clicked(bool)),
            SLOT(slotRemoveNicknameClicked()));
    connect(removeIdentityButton, SIGNAL(clicked(bool)),
            SLOT(slotRemoveIdentityClicked()));
    connect(editNicknameButton, SIGNAL(clicked(bool)),
            SLOT(slotEditNicknameClicked()));
    connect(editIdentityButton, SIGNAL(clicked(bool)),
            SLOT(slotEditIdentityClicked()));
    connect(markLastPositionCheckBox, SIGNAL(clicked(bool)),
            SLOT(slotMarkLastPositionClicked(bool)));
    connect(awayNicknameLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotAwayNicknameTextEdited(QString)));
    connect(awayMessagesGroupBox, SIGNAL(clicked(bool)),
            SLOT(slotAwayMessagesClicked(bool)));
    connect(awayMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotAwayMessageTextEdited(QString)));
    connect(returnMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotReturnMessageTextEdited(QString)));
    connect(kickMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotKickMessageTextEdited(QString)));
    connect(partMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotPartMessageTextEdited(QString)));
    connect(quitMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotQuitMessageTextEdited(QString)));
}

void
IdentityDialog::setupDialog()
{
    QWidget* widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);
    setButtons(KDialog::Ok);

    setupActions();
    setupIcons();
}

void
IdentityDialog::setupIcons()
{
    addIdentityButton->setIcon(KIcon("list-add-user"));
    editIdentityButton->setIcon(KIcon("edit-rename"));
    removeIdentityButton->setIcon(KIcon("list-remove-user"));

    addNicknameButton->setIcon(KIcon("list-add"));
    removeNicknameButton->setIcon(KIcon("list-remove"));
    editNicknameButton->setIcon(KIcon("edit-rename"));
}

void
IdentityDialog::slotAddIdentityClicked()
{
    bool ok;
    const QString identity = KInputDialog::getText(i18n("Add New Identity"), i18n("Enter a new identity name."),
                                                   QString(), &ok, this);
    // Check to see if the identity already exists.
    const bool emptyIdentity = identitySelector->findIdentities(identity, Qt::MatchExactly).isEmpty();
    // Check that the user clicked okay.
    if (ok) {
        if (!emptyIdentity) {
            KMessageBox::error(this, i18n("Unable to add identity. Identity already exists."));
            // Re show thie dialog.
            slotAddIdentityClicked();
        } else {
            createNewIdentity(identity);
        }
    }
}

void
IdentityDialog::slotAddNicknameClicked()
{
    bool ok;
    const QString nickname = KInputDialog::getText(i18n("Add New Nickname"), i18n("Enter a new nickname."),
                                                   QString(), &ok, this);
    // Check to see if the nickname already exists.
    const bool emptyNickname = nicknameList->findNicknames(nickname, Qt::MatchExactly).isEmpty();
    // Check that the user clicked okay.
    if (ok) {
        if (!emptyNickname) {
            KMessageBox::error(this, i18n("Unable to add nickname. Nickname already exists."));
            // Re show this dialog.
            slotAddNicknameClicked();
        } else {
#if defined(Q_CC_GNU)
#   warning "Fix this"
#endif // defined(Q_CC_GNU)
            /*Aki::Sql::Nickname* nick = Aki::SqlNickname::newNickname(nickname, identitySelector->currentIdentity());
            nicknameList->addNickname(nick);*/
        }
    }
}

void
IdentityDialog::slotAwayMessagesClicked(bool checked)
{
    Aki::Sql::Identity* identity = identitySelector->currentIdentity();
    identity->setMessagesEnabled(checked);
    _database->update(identity);
}

void
IdentityDialog::slotAwayMessageTextEdited(const QString& message)
{
    Aki::Sql::Identity* identity = identitySelector->currentIdentity();
    identity->setAwayMessage(message);
    _database->update(identity);
}

void
IdentityDialog::slotAwayNicknameTextEdited(const QString& nickname)
{
    Aki::Sql::Identity* identity = identitySelector->currentIdentity();
    identity->setAwayNickname(nickname);
    _database->update(identity);
}

void
IdentityDialog::slotEditIdentityClicked()
{
    Aki::Sql::Identity* currentIdentity = identitySelector->currentIdentity();
    if (!currentIdentity) {
        return;
    }

    bool ok;
    const QString identity = KInputDialog::getText(i18n("Edit Identity"), i18n("Enter a new identity."),
                                                   currentIdentity->name(), &ok, this);
    // Check to see if the identity already exists.
    const bool emptyIdentity = identitySelector->findIdentities(identity, Qt::MatchExactly).isEmpty();
    // Check that the user clicked okay.
    if (ok) {
        if (!emptyIdentity) {
            KMessageBox::error(this, i18n("Unable to edit Identity. Identity already exists."));
            // Re show this dialog.
            slotEditIdentityClicked();
        } else {
            currentIdentity->setName(identity);
            _database->update(currentIdentity);
        }
    }
}

void
IdentityDialog::slotEditNicknameClicked()
{
    Aki::Sql::Nickname* currentNickname = nicknameList->currentNickname();
    if (!currentNickname) {
        return;
    }

    bool ok;
    const QString nickname = KInputDialog::getText(i18n("Edit Nickname"), i18n("Enter a new nickname."),
                                                   currentNickname->nickname(), &ok, this);
    // Check to see if the nickname already exists.
    const bool emptyNickname = nicknameList->findNicknames(nickname, Qt::MatchExactly).isEmpty();
    // Check that the user clicked okay.
    if (ok) {
        if (!emptyNickname) {
            KMessageBox::error(this, i18n("Unable to edit nickname. Nickname already exists."));
            // Re show this dialog.
            slotEditNicknameClicked();
        } else {
            currentNickname->setNickname(nickname);
            _database->update(currentNickname);
        }
    }
}

void
IdentityDialog::slotIdentityActivated(Aki::Sql::Identity* identity)
{
    if (!identity) {
        nicknameList->repopulateNicknames(0);
        realNameLineEdit->setText(QString());
        markLastPositionCheckBox->setChecked(false);
        awayNicknameLineEdit->setText(QString());
        awayMessagesGroupBox->setChecked(false);
        awayMessageLineEdit->setText(QString());
        returnMessageLineEdit->setText(QString());
        kickMessageLineEdit->setText(QString());
        partMessageLineEdit->setText(QString());
        quitMessageLineEdit->setText(QString());
        return;
    }

    nicknameList->repopulateNicknames(identity);
    realNameLineEdit->setText(identity->realName());
    markLastPositionCheckBox->setChecked(identity->isMarkLastPositionEnabled());
    awayNicknameLineEdit->setText(identity->awayNickname());
    awayMessagesGroupBox->setChecked(identity->isMessagesEnabled());
    awayMessageLineEdit->setText(identity->awayMessage());
    returnMessageLineEdit->setText(identity->returnMessage());
    kickMessageLineEdit->setText(identity->kickMessage());
    partMessageLineEdit->setText(identity->partMessage());
    quitMessageLineEdit->setText(identity->quitMessage());
}

void
IdentityDialog::slotKickMessageTextEdited(const QString& message)
{
    Aki::Sql::Identity* identity = identitySelector->currentIdentity();
    identity->setKickMessage(message);
    _database->update(identity);
}

void
IdentityDialog::slotMarkLastPositionClicked(bool checked)
{
    Aki::Sql::Identity* identity = identitySelector->currentIdentity();
    identity->setMarkLastPositionEnabled(checked);
    _database->update(identity);
}

void
IdentityDialog::slotPartMessageTextEdited(const QString& message)
{
    Aki::Sql::Identity* identity = identitySelector->currentIdentity();
    identity->setPartMessage(message);
    _database->update(identity);
}

void
IdentityDialog::slotQuitMessageTextEdited(const QString& message)
{
    Aki::Sql::Identity* identity = identitySelector->currentIdentity();
    identity->setQuitMessage(message);
    _database->update(identity);
}

void
IdentityDialog::slotRealNameTextEdited(const QString& name)
{
    Aki::Sql::Identity* identity = identitySelector->currentIdentity();
    identity->setRealName(name);
    _database->update(identity);
}

void
IdentityDialog::slotRemoveIdentityClicked()
{
    Aki::Sql::Identity* currentIdentity = identitySelector->currentIdentity();
    if (!currentIdentity) {
        return;
    }

    int result = KMessageBox::warningYesNo(this, i18n("You are about to remove %1. This cannot be undone.",
                                                      currentIdentity->name()),
                                           i18n("Confirm Identity Deletion"));
    switch (result) {
    case KMessageBox::Yes: {
        // Remove the identity.
        _database->remove(currentIdentity);
        // Take the identity from the identity combo box.
        delete identitySelector->takeIdentity(identitySelector->currentIndex());
        break;
    }
    }
}

void
IdentityDialog::slotRemoveNicknameClicked()
{
    Aki::Sql::Nickname* currentNickname = nicknameList->currentNickname();
    if (!currentNickname) {
        return;
    }

    int result = KMessageBox::warningYesNo(this, i18n("You are about to remove %1. This cannot be undone.",
                                                      currentNickname->nickname()),
                                           i18n("Confirm Nickname Deletion"));

    switch (result) {
    case KMessageBox::Yes: {
        // Remove the nickname from the identity.
        _database->remove(currentNickname);
        // Take the nickname from the nickname list.
        delete nicknameList->takeNickname(nicknameList->row(currentNickname));
        break;
    }
    default: {
        break;
    }
    }
}

void
IdentityDialog::slotReturnMessageTextEdited(const QString& message)
{
    Aki::Sql::Identity* identity = identitySelector->currentIdentity();
    identity->setReturnMessage(message);
    _database->update(identity);
}
