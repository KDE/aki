/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
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
#include "aki.hpp"
#include "utils/database.hpp"
#include "utils/sqlidentity.hpp"
#include "utils/sqlnickname.hpp"
#include <KDE/KInputDialog>
#include <KDE/KMessageBox>
using namespace Aki;

IdentityDialog::IdentityDialog(QWidget* parent)
    : KDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setupDialog();
    if (identitySelector->count()) {
        slotIdentityActivated(identitySelector->currentIdentity());
    }
}

IdentityDialog::~IdentityDialog()
{
}

void
IdentityDialog::createNewIdentity(const QString& name)
{
    // Create a new identity.
    Aki::SqlIdentity* identity = Aki::SqlIdentity::newIdentity(name);
    // Add the new identity to the combo box.
    identitySelector->addIdentity(identity);
    identitySelector->setCurrentIdentity(identity);
}

void
IdentityDialog::setupActions()
{
    connect(identitySelector, SIGNAL(currentIndexChanged(Aki::SqlIdentity*)),
            SLOT(slotIdentityActivated(Aki::SqlIdentity*)));
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
            Aki::SqlNickname* nick = Aki::SqlNickname::newNickname(nickname, identitySelector->currentIdentity());
            nicknameList->addNickname(nick);
        }
    }
}

void
IdentityDialog::slotAwayMessagesClicked(bool checked)
{
    Aki::SqlIdentity* identity = identitySelector->currentIdentity();
    identity->setEnableMessages(checked);
    identity->save();
}

void
IdentityDialog::slotAwayMessageTextEdited(const QString& message)
{
    Aki::SqlIdentity* identity = identitySelector->currentIdentity();
    identity->setAwayMessage(message);
    identity->save();
}

void
IdentityDialog::slotAwayNicknameTextEdited(const QString& nickname)
{
    Aki::SqlIdentity* identity = identitySelector->currentIdentity();
    identity->setAwayNickname(nickname);
    identity->save();
}

void
IdentityDialog::slotEditIdentityClicked()
{
    Aki::SqlIdentity* currentIdentity = identitySelector->currentIdentity();
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
            currentIdentity->save();
        }
    }
}

void
IdentityDialog::slotEditNicknameClicked()
{
    Aki::SqlNickname* currentNickname = nicknameList->currentNickname();
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
            currentNickname->save();
        }
    }
}

void
IdentityDialog::slotIdentityActivated(Aki::SqlIdentity* identity)
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
    Aki::SqlIdentity* identity = identitySelector->currentIdentity();
    identity->setKickMessage(message);
    identity->save();
}

void
IdentityDialog::slotMarkLastPositionClicked(bool checked)
{
    Aki::SqlIdentity* identity = identitySelector->currentIdentity();
    identity->setMarkLastPosition(checked);
    identity->save();
}

void
IdentityDialog::slotPartMessageTextEdited(const QString& message)
{
    Aki::SqlIdentity* identity = identitySelector->currentIdentity();
    identity->setPartMessage(message);
    identity->save();
}

void
IdentityDialog::slotQuitMessageTextEdited(const QString& message)
{
    kDebug() << message;
    Aki::SqlIdentity* identity = identitySelector->currentIdentity();
    identity->setQuitMessage(message);
    identity->save();
}

void
IdentityDialog::slotRealNameTextEdited(const QString& name)
{
    Aki::SqlIdentity* identity = identitySelector->currentIdentity();
    identity->setRealName(name);
    identity->save();
}

void
IdentityDialog::slotRemoveIdentityClicked()
{
    Aki::SqlIdentity* currentIdentity = identitySelector->currentIdentity();
    if (!currentIdentity) {
        return;
    }

    int result = KMessageBox::warningYesNo(this, i18n("You are about to remove %1. This cannot be undone.",
                                                      currentIdentity->name()),
                                           i18n("Confirm Identity Deletion"));
    switch (result) {
    case KMessageBox::Yes: {
        // Remove the identity.
        currentIdentity->remove();
        // Take the identity from the identity combo box.
        delete identitySelector->takeIdentity(identitySelector->currentIndex());
        break;
    }
    }
}

void
IdentityDialog::slotRemoveNicknameClicked()
{
    Aki::SqlNickname* currentNickname = nicknameList->currentNickname();
    if (!currentNickname) {
        return;
    }

    int result = KMessageBox::warningYesNo(this, i18n("You are about to remove %1. This cannot be undone.",
                                                      currentNickname->nickname()),
                                           i18n("Confirm Nickname Deletion"));

    switch (result) {
    case KMessageBox::Yes: {
        // Remove the nickname from the identity.
        currentNickname->remove();
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
    Aki::SqlIdentity* identity = identitySelector->currentIdentity();
    identity->setReturnMessage(message);
    identity->save();
}
