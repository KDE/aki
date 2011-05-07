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

#include "importidentitypage.hpp"
#include "sql/database.hpp"
#include "sql/identity.hpp"
#include <QtGui/QRegExpValidator>
#include <KDE/KInputDialog>
#include <KDE/KMessageBox>
using namespace Aki;
using namespace Aki::Sql;

ImportIdentityPage::ImportIdentityPage(Aki::Sql::Database* database, QWidget* parent)
    : QWidget(parent),
    _database(database)
{
    setupUi(this);

    addIdentityButton->setIcon(KIcon("list-add-user"));
    connect(addIdentityButton, SIGNAL(clicked(bool)),
            SLOT(slotAddIdentityClicked()));

    copyIdentityButton->setIcon(KIcon("list-copy"));
    connect(copyIdentityButton, SIGNAL(clicked(bool)),
            SLOT(slotCopyIdentityClicked()));

    editIdentityButton->setIcon(KIcon("edit-rename"));
    connect(editIdentityButton, SIGNAL(clicked(bool)),
            SLOT(slotEditIdentityClicked()));

    removeIdentityButton->setIcon(KIcon("list-remove-user"));
    connect(removeIdentityButton, SIGNAL(clicked(bool)),
            SLOT(slotRemoveIdentityClicked()));

    connect(awayMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotAwayMessageTextEdited(QString)));
    connect(awayMessagesGroupBox, SIGNAL(toggled(bool)),
            SLOT(slotAwayMessagesToggled(bool)));
    connect(kickMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotKickMessageTextEdited(QString)));
    connect(markLastPositionCheckBox, SIGNAL(toggled(bool)),
            SLOT(slotMarkLastPositionToggled(bool)));
    connect(partMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotPartMessageTextEdited(QString)));
    connect(quitMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotQuitMessageTextEdited(QString)));
    connect(realNameLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotRealNameTextEdited(QString)));
    connect(returnMessageLineEdit, SIGNAL(textEdited(QString)),
            SLOT(slotReturnMessageTextEdited(QString)));
}

ImportIdentityPage::~ImportIdentityPage()
{
}

void
ImportIdentityPage::loadNewIdentity()
{
}

bool
ImportIdentityPage::saveIdentity(Aki::Sql::Identity* identity)
{
    if (_database->transaction()) {
        if (!_database->update(identity)) {
            if (!_database->rollback()) {
                kDebug() << "Unable to rollback Identity data";
                return false;
            }
        } else {
            if (!_database->commit()) {
                kDebug() << "Unable to commit Identity data";
                return false;
            }
        }
    }

    return true;
}

void
ImportIdentityPage::setWidgetValues(Aki::Sql::Identity* identity)
{
    awayMessageLineEdit->setText(identity->awayMessage());
    awayMessagesGroupBox->setChecked(identity->isMessagesEnabled());
    kickMessageLineEdit->setText(identity->kickMessage());
    markLastPositionCheckBox->setChecked(identity->isMarkLastPositionEnabled());
    partMessageLineEdit->setText(identity->partMessage());
    quitMessageLineEdit->setText(identity->quitMessage());
    realNameLineEdit->setText(identity->realName());
    returnMessageLineEdit->setText(identity->returnMessage());
}

void
ImportIdentityPage::slotAddIdentityClicked()
{
    bool ok = false;
    QScopedPointer<QRegExpValidator> validator(new QRegExpValidator);
    validator->setRegExp(QRegExp("[0-9a-zA-Z]+"));

    const QString name = KInputDialog::getText(i18n("Enter a name for the new identity"),
                                               i18n("Please enter a name for the new identity:"),
                                               QString(), &ok, 0, validator.data());
    if (!ok) {
        return;
    }

    if (identityComboBox->contains(name)) {
        KMessageBox::error(this, i18n("Unable to create a new identity. Identity already exists."),
                           i18n("Identity already exists."));
        slotAddIdentityClicked();
        return;
    }

    Aki::Sql::Identity* previousIdentity = 0;

    if (identityComboBox->count() > 0) {
        previousIdentity = identityComboBox->currentIdentity();
    }

    Aki::Sql::Identity* identity = new Aki::Sql::Identity;
    identity->setName(name);

    if (_database->transaction()) {
        if (!_database->add(identity)) {
            if (!_database->rollback()) {
                kDebug() << "Unable to rollback Identity data";
                return;
            }
        } else {
            if (!_database->commit()) {
                kDebug() << "Unable to commit Identity data";
                return;
            }
        }
    } else {
        kDebug() << "Unable to start transaction";
        return;
    }

    identityComboBox->addIdentity(identity);
    identityComboBox->setCurrentIdentity(identity);
}

void
ImportIdentityPage::slotAwayMessageTextEdited(const QString& message)
{
    if (!_identity) {
        return;
    }

    _identity->setAwayMessage(message);
    if (!saveIdentity(_identity)) {
        KMessageBox::error(this, i18n("Unable to commit changes."),
                           i18n("Unable to commit changes."));
    }
}

void
ImportIdentityPage::slotAwayMessagesToggled(bool enabled)
{
    if (!_identity) {
        return;
    }

    _identity->setMessagesEnabled(enabled);
    if (!saveIdentity(_identity)) {
        KMessageBox::error(this, i18n("Unable to commit changes."),
                           i18n("Unable to commit changes."));
    }
}

void
ImportIdentityPage::slotAwayNicknameTextEdited(const QString& nickname)
{
    if (!_identity) {
        return;
    }

    _identity->setAwayNickname(nickname);
    if (!saveIdentity(_identity)) {
        KMessageBox::error(this, i18n("Unable to commit changes."),
                           i18n("Unable to commit changes."));
        return;
    }
}

void
ImportIdentityPage::slotCopyIdentityClicked()
{
    bool ok = false;
    QScopedPointer<QRegExpValidator> validator(new QRegExpValidator);
    validator->setRegExp(QRegExp("[0-9a-zA-Z]+"));

    const QString name = KInputDialog::getText(i18n("Enter a name for the new identity."),
                                               i18n("Enter a name for the new identity:"), QString(), &ok, 0, validator.data());

    if (!ok) {
        return;
    }

    if (identityComboBox->contains(name)) {
        KMessageBox::error(this, i18n("Unable to create a new identity. Identity already exists."),
                           i18n("Identity already exists."));
        slotCopyIdentityClicked();
    }

    Aki::Sql::Identity* current = identityComboBox->currentIdentity();
    Aki::Sql::Identity* identity = new Aki::Sql::Identity;
    identity->setAwayMessage(current->awayMessage());
    identity->setAwayNickname(current->awayNickname());
    identity->setKickMessage(current->kickMessage());
    identity->setMarkLastPositionEnabled(current->isMarkLastPositionEnabled());
    identity->setMessagesEnabled(current->isMessagesEnabled());
    identity->setName(name);
    identity->setPartMessage(current->partMessage());
    identity->setQuitMessage(current->quitMessage());
    identity->setRealName(current->realName());
    identity->setReturnMessage(current->returnMessage());

    if (_database->transaction()) {
        if (!_database->add(identity)) {
            if (!_database->rollback()) {
                kDebug() << "Unable to rollback Identity data";
                return;
            }
        } else {
            if (!_database->commit()) {
                kDebug() << "Unable to commit Identity data";
                return;
            }
        }
    } else {
        kDebug() << "Unable to start transaction";
        return;
    }

    identityComboBox->addIdentity(identity);
    identityComboBox->setCurrentIdentity(identity);
}

void
ImportIdentityPage::slotEditIdentityClicked()
{
    bool ok = false;
    Aki::Sql::Identity* current = identityComboBox->currentIdentity();
    if (!current) {
        return;
    }

    QScopedPointer<QRegExpValidator> validator(new QRegExpValidator);
    validator->setRegExp(QRegExp("[0-9a-zA-Z]+"));

    const QString name = KInputDialog::getText(i18n("Enter a new identity name"),
                                               i18n("Enter a new identity name:"), current->name(), &ok, 0, validator.data());

    if (!ok) {
        return;
    }

    if (identityComboBox->contains(name)) {
        KMessageBox::error(this, i18n("Unable to create a new identity. Identity already exists."),
                           i18n("Identity already exists."));
        slotEditIdentityClicked();
        return;
    }

    current->setName(name);

    if (_database->transaction()) {
        if (!_database->update(current)) {
            if (!_database->rollback()) {
                kDebug() << "Unable to rollback Identity data";
                return;
            }
        } else {
            if (!_database->commit()) {
                kDebug() << "Unable to commit Identity data";
                return;
            }
        }
    } else {
        kDebug() << "Unable to start transaction";
        return;
    }
}

void
ImportIdentityPage::slotIdentityActivated(Aki::Sql::Identity* identity)
{
    if (!identity) {
        return;
    }

    _identity = identity;

    if (identity->name() == "Default Identity") {
        addIdentityButton->setDisabled(true);
        editIdentityButton->setDisabled(true);
        removeIdentityButton->setDisabled(true);
    }

    setWidgetValues(identity);
}

void
ImportIdentityPage::slotKickMessageTextEdited(const QString& message)
{
    if (!_identity) {
        return;
    }

    _identity->setKickMessage(message);
    if (!saveIdentity(_identity)) {
        KMessageBox::error(this, i18n("Unable to commit changes."),
                           i18n("Unable to commit changes."));
    }
}

void
ImportIdentityPage::slotMarkLastPositionToggled(bool enabled)
{
    if (!_identity) {
        return;
    }

    _identity->setMarkLastPositionEnabled(enabled);
    if (!saveIdentity(_identity)) {
        KMessageBox::error(this, i18n("Unable to commit changes."),
                           i18n("Unable to commit changes."));
    }
}

void
ImportIdentityPage::slotPartMessageTextEdited(const QString& message)
{
    if (!_identity) {
        return;
    }

    _identity->setPartMessage(message);
    if (!saveIdentity(_identity)) {
        KMessageBox::error(this, i18n("Unable to commit changes."),
                           i18n("Unable to commit changes."));
    }
}

void
ImportIdentityPage::slotQuitMessageTextEdited(const QString& message)
{
    if (!_identity) {
        return;
    }

    _identity->setQuitMessage(message);
    if (!saveIdentity(_identity)) {
        KMessageBox::error(this, i18n("Unable to commit changes."),
                           i18n("Unable to commit changes."));
    }
}

void
ImportIdentityPage::slotRealNameTextEdited(const QString& name)
{
    if (!_identity) {
        return;
    }

    _identity->setRealName(name);
    if (!saveIdentity(_identity)) {
        KMessageBox::error(this, i18n("Unable to commit changes."),
                           i18n("Unable to commit changes."));
    }
}

void
ImportIdentityPage::slotRemoveIdentityClicked()
{
    Aki::Sql::Identity* current = identityComboBox->currentIdentity();
    if (!current) {
        return;
    }

    const int result = KMessageBox::warningYesNo(this, i18n("Are you sure you want to delete the identity: '%1'?", current->name()),
                                                 i18n("Are you sure you want to delete the identity?"));
    switch (result) {
    case KMessageBox::Yes: {
        if (_database->transaction()) {
            if (!_database->remove(current)) {
                if (!_database->rollback()) {
                    kDebug() << "Unable to rollback Identity data";
                    return;
                }
            } else {
                if (!_database->commit()) {
                    kDebug() << "Unable to commit Identity data";
                    return;
                }
            }
        } else {
            kDebug() << "Unable to start transaction";
            return;
        }
        break;
    }
    default: {
        return;
    }
    }

    delete identityComboBox->takeIdentity(identityComboBox->row(current));
}

void
ImportIdentityPage::slotReturnMessageTextEdited(const QString& message)
{
    if (!_identity) {
        return;
    }

    _identity->setReturnMessage(message);
    if (!saveIdentity(_identity)) {
        KMessageBox::error(this, i18n("Unable to commit changes."),
                           i18n("Unable to commit changes."));
    }
}
