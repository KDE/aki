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
#include <KMessageBox>
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
}

void
ImportIdentityPage::slotAwayMessageTextEdited(const QString& message)
{
    if (!_identity) {
        return;
    }

    _identity->setAwayMessage(message);
}

void
ImportIdentityPage::slotAwayMessagesToggled(bool enabled)
{
    if (!_identity) {
        return;
    }

    _identity->setMessagesEnabled(enabled);
}

void
ImportIdentityPage::slotAwayNicknameTextEdited(const QString& nickname)
{
    if (!_identity) {
        return;
    }

    _identity->setAwayNickname(nickname);
}

void
ImportIdentityPage::slotCopyIdentityClicked()
{
}

void
ImportIdentityPage::slotEditIdentityClicked()
{
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
}

void
ImportIdentityPage::slotMarkLastPositionToggled(bool enabled)
{
    if (!_identity) {
        return;
    }

    _identity->setMarkLastPositionEnabled(enabled);
}

void
ImportIdentityPage::slotPartMessageTextEdited(const QString& message)
{
    if (!_identity) {
        return;
    }

    _identity->setPartMessage(message);
}

void
ImportIdentityPage::slotQuitMessageTextEdited(const QString& message)
{
    if (!_identity) {
        return;
    }

    _identity->setQuitMessage(message);
}

void
ImportIdentityPage::slotRealNameTextEdited(const QString& name)
{
    if (!_identity) {
        return;
    }

    _identity->setRealName(name);
}

void
ImportIdentityPage::slotRemoveIdentityClicked()
{
}

void
ImportIdentityPage::slotReturnMessageTextEdited(const QString& message)
{
    if (!_identity) {
        return;
    }

    _identity->setReturnMessage(message);
}
