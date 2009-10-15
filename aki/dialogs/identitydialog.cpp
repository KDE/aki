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

#include "identitydialog.h"
#include "config/identityconfig.h"
#include <KInputDialog>
#include <KMessageBox>
//#include <QMutableListIterator>

IdentityDialog::IdentityDialog(QWidget *parent)
    : KDialog(parent),
    m_identityConfig(0)
{
    QWidget *widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);

    setButtons(Ok | Cancel);
    setSizeGripEnabled(true);

    addIdentityButton->setIcon(KIcon("list-add-user"));
    deleteIdentityButton->setIcon(KIcon("list-remove-user"));
    editIdentityButton->setIcon(KIcon("edit-rename"));

    m_identityConfig = new Aki::IdentityConfig(this);

    connect(identitySelector, SIGNAL(activated(int)),
            SLOT(slotIdentityActivated(int)));
    connect(addIdentityButton, SIGNAL(clicked(bool)),
            SLOT(slotAddIdentityClicked()));
    connect(deleteIdentityButton, SIGNAL(clicked(bool)),
            SLOT(slotDeleteIdentityClicked()));
    connect(editIdentityButton, SIGNAL(clicked(bool)),
            SLOT(slotEditIdentityClicked()));
    connect(awayMessage, SIGNAL(textEdited(QString)),
            SLOT(slotAwayMessageTextEdited(QString)));
    connect(awayNickname, SIGNAL(textEdited(QString)),
            SLOT(slotAwayNicknameTextEdited(QString)));
    connect(kickMessage, SIGNAL(textEdited(QString)),
            SLOT(slotKickMessageTextEdited(QString)));
    connect(markLastPosition, SIGNAL(clicked(bool)),
            SLOT(slotMarkLastPositionClicked(bool)));
    connect(nickList, SIGNAL(changed()),
            SLOT(slotNicknameChanged()));
    connect(partMessage, SIGNAL(textEdited(QString)),
            SLOT(slotPartMessageTextEdited(QString)));
    connect(quitMessage, SIGNAL(textEdited(QString)),
            SLOT(slotQuitMessageTextEdited(QString)));
    connect(realName, SIGNAL(textEdited(QString)),
            SLOT(slotRealNameTextEdited(QString)));
    connect(returnMessage, SIGNAL(textEdited(QString)),
            SLOT(slotReturnMessageTextEdited(QString)));
    connect(useAwayMessages, SIGNAL(clicked(bool)),
            SLOT(slotUseAwayMessagesClicked(bool)));
    connect(this, SIGNAL(okClicked()),
            SLOT(slotOkClicked()));

    loadSettings();
}

void
IdentityDialog::loadSettings()
{
    QStringListIterator identityIter(m_identityConfig->groupList());
    while (identityIter.hasNext()) {
        QString identityName = identityIter.next();

        // Switch to the new identity group.
        m_identityConfig->setCurrentGroup(identityName);
        // Add the identity to the identity selector.
        identitySelector->addItem(identityName);

        // Create the identity object.
        Aki::IdentityPointer identity(new Aki::Identity(identityName));
        identity->setAwayMessage(m_identityConfig->awayMessage());
        identity->setAwayNickname(m_identityConfig->awayNickname());
        identity->setKickMessage(m_identityConfig->kickMessage());
        identity->setMarkLastPosition(m_identityConfig->isMarkLastPositionEnabled());
        identity->setNicknameList(m_identityConfig->nicknameList());
        identity->setPartMessage(m_identityConfig->partMessage());
        identity->setQuitMessage(m_identityConfig->quitMessage());
        identity->setRealName(m_identityConfig->realName());
        identity->setReturnMessage(m_identityConfig->returnMessage());
        identity->setUseAwayMessages(m_identityConfig->isAwayMessagesEnabled());

        identitySelector->model()->sort(0);
        const int index = identitySelector->findText(identityName, Qt::MatchExactly);
        m_identityList.insert(index, identity);
    }

    if (identitySelector->count() == 1) {
        deleteIdentityButton->setDisabled(true);
    } else {
        deleteIdentityButton->setEnabled(true);
    }
    
    identitySelector->setCurrentIndex(0);
    slotIdentityActivated(0);
}

void
IdentityDialog::slotAddIdentityClicked()
{
    bool ok = false;

    // Get the identity name.
    QString identity = KInputDialog::getText(i18n("Add a new identity"),
                                             i18n("Enter a new identity name"), QString(), &ok, this);

    // Check that the identity isn't empty and not found.
    if (ok && !identity.isEmpty() &&
        identitySelector->findText(identity, Qt::MatchExactly) == -1) {
        newIdentitySettings(identity);
    } else if (ok && !identity.isEmpty() && // Check that the identity isn't empty and already exists.
               !identitySelector->findText(identity) >= 0) {
        KMessageBox::error(this, i18n("Unable to add identity. Identity name already exists."));
        slotAddIdentityClicked();
    } else if (ok && identity.isEmpty()) { // Check to see if it's empty.
        KMessageBox::error(this, i18n("Unable to add identity. Identity name is empty."));
        slotAddIdentityClicked();
    }

    if (identitySelector->count() > 1) {
        deleteIdentityButton->setEnabled(true);
    }
}

void
IdentityDialog::newIdentitySettings(const QString &name)
{
    // Create the new group.
    m_identityConfig->createGroup(name);
    // Add the new identity to the identity selector.
    identitySelector->addItem(name);

    // Create the new identity.
    Aki::IdentityPointer identity(new Aki::Identity(name));
    identity->setAwayMessage(m_identityConfig->awayMessage());
    identity->setAwayNickname(m_identityConfig->awayNickname());
    identity->setKickMessage(m_identityConfig->kickMessage());
    identity->setMarkLastPosition(m_identityConfig->isMarkLastPositionEnabled());
    identity->setNicknameList(m_identityConfig->nicknameList());
    identity->setPartMessage(m_identityConfig->partMessage());
    identity->setQuitMessage(m_identityConfig->quitMessage());
    identity->setRealName(m_identityConfig->realName());
    identity->setReturnMessage(m_identityConfig->returnMessage());
    identity->setUseAwayMessages(m_identityConfig->isAwayMessagesEnabled());

    identitySelector->model()->sort(0);
    const int index = identitySelector->findText(name, Qt::MatchExactly);
    m_identityList.insert(index, identity);
    identitySelector->setCurrentIndex(index);
    slotIdentityActivated(index);
}

void
IdentityDialog::slotIdentityActivated(int index)
{
    Aki::IdentityPointer identity(m_identityList.value(index));
    nickList->setItems(identity->nicknameList());
    markLastPosition->setChecked(identity->isMarkLastPositionEnabled());
    awayNickname->setText(identity->awayNickname());
    useAwayMessages->setChecked(identity->isAwayMessagesEnabled());
    awayMessage->setText(identity->awayMessage());
    returnMessage->setText(identity->returnMessage());
    kickMessage->setText(identity->kickMessage());
    partMessage->setText(identity->partMessage());
    realName->setText(identity->realName());
    quitMessage->setText(identity->quitMessage());
}

void
IdentityDialog::updateSettings()
{
    QListIterator<Aki::IdentityPointer> identityIter(m_identityList);
    while (identityIter.hasNext()) {
        Aki::IdentityPointer identity = identityIter.next();
        m_identityConfig->setCurrentGroup(identity->name());
        m_identityConfig->setAwayMessage(identity->awayMessage());
        m_identityConfig->setAwayNickname(identity->awayNickname());
        m_identityConfig->setKickMessage(identity->kickMessage());
        m_identityConfig->setMarkLastPosition(identity->isMarkLastPositionEnabled());
        m_identityConfig->setNicknameList(identity->nicknameList());
        m_identityConfig->setPartMessage(identity->partMessage());
        m_identityConfig->setQuitMessage(identity->quitMessage());
        m_identityConfig->setRealName(identity->realName());
        m_identityConfig->setReturnMessage(identity->returnMessage());
        m_identityConfig->setUseAwayMessages(identity->isAwayMessagesEnabled());
    }

    m_identityConfig->save();
}

Aki::IdentityPointer
IdentityDialog::identityAtCurrentIndex()
{
    return m_identityList.value(identitySelector->currentIndex());
}

void
IdentityDialog::slotEditIdentityClicked()
{
    bool ok = false;

    // Get the current identity name.
    const QString oldName = identitySelector->currentText();
    int index = 0;

    // Get the new identity name.
    QString newName = KInputDialog::getText(i18n("Edit current identity."),
                                            i18n("Enter a new identity name."), oldName, &ok, this);

    // Check if new name isn't empty and the old name isn't the same
    // as the new name.
    if (ok && !newName.isEmpty() && oldName != newName) {
        // Check to see if the identity exists
        if (identitySelector->findText(newName) == -1) {
            // Delete the old group.
            m_identityConfig->deleteGroup(oldName);
            // Create the new group.
            m_identityConfig->createGroup(newName);

            // Search through the identity list.
            QMutableListIterator<Aki::IdentityPointer> identityIter(m_identityList);
            while (identityIter.hasNext()) {
                Aki::IdentityPointer identity = identityIter.next();

                if (identity->name() == oldName) {
                    // Get the index of the identity.
                    index = m_identityList.indexOf(identity);
                    // Change the old name to the new one in the identity selector.
                    identitySelector->setItemText(index, newName);
                    // Sort the identity selector.
                    identitySelector->model()->sort(0);
                    // Copy the old identity.
                    Aki::IdentityPointer newIdent = m_identityList.value(index);
                    // Set the old identity name to the new one.
                    newIdent->setName(newName);
                    // Remove it from the list.
                    m_identityList.removeAt(index);
                    // Get the index of the new name in the identity selector.
                    index = identitySelector->findText(newName);
                    m_identityList.insert(index, newIdent);
                }
            }
        } else {
            KMessageBox::error(this, i18n("Unable to change identity name. Identity name is already taken."));
            slotEditIdentityClicked();
        }
    } else if (ok && !newName.isEmpty() && oldName == newName) {
        KMessageBox::error(this, i18n("Unable to change identity name. Identity name cannot be the same as the old one."));
        slotEditIdentityClicked();
    } else if (ok && newName.isEmpty()) {
        KMessageBox::error(this, i18n("Unable to change identity name. Identity name is empty."));
        slotEditIdentityClicked();
    }
}

void
IdentityDialog::slotDeleteIdentityClicked()
{
    if (identitySelector->count() == 1) {
        return;
    }

    // Get the current identity name.
    const QString identityName = identitySelector->currentText();
    int index = 0;

    QListIterator<Aki::IdentityPointer> identityIter(m_identityList);
    while (identityIter.hasNext()) {
        Aki::IdentityPointer identity = identityIter.next();
        if (identity->name() == identityName) {
            index = m_identityList.indexOf(identity);
        }
    }

    // Remove the identity name from the identity selector.
    identitySelector->removeItem(identitySelector->currentIndex());
    // Delete the settings group for the identity.
    m_identityConfig->deleteGroup(identityName);
    // Remove the identity from the list.
    m_identityList.removeAt(index);

    if (identitySelector->count() == 1) {
        deleteIdentityButton->setDisabled(true);
    }
}

void
IdentityDialog::slotAwayMessageTextEdited(const QString &message)
{
    identityAtCurrentIndex()->setAwayMessage(message);
}

void
IdentityDialog::slotAwayNicknameTextEdited(const QString &nickname)
{
    identityAtCurrentIndex()->setAwayNickname(nickname);
}

void
IdentityDialog::slotKickMessageTextEdited(const QString &message)
{
    identityAtCurrentIndex()->setKickMessage(message);
}

void
IdentityDialog::slotMarkLastPositionClicked(bool clicked)
{
    identityAtCurrentIndex()->setMarkLastPosition(clicked);
}

void
IdentityDialog::slotNicknameChanged()
{
    identityAtCurrentIndex()->setNicknameList(nickList->items());
}

void
IdentityDialog::slotOkClicked()
{
    updateSettings();
}

void
IdentityDialog::slotPartMessageTextEdited(const QString &message)
{
    identityAtCurrentIndex()->setPartMessage(message);
}

void
IdentityDialog::slotQuitMessageTextEdited(const QString &message)
{
    identityAtCurrentIndex()->setQuitMessage(message);
}

void
IdentityDialog::slotRealNameTextEdited(const QString &name)
{
    identityAtCurrentIndex()->setRealName(name);
}

void
IdentityDialog::slotReturnMessageTextEdited(const QString &message)
{
    identityAtCurrentIndex()->setReturnMessage(message);
}

void
IdentityDialog::slotUseAwayMessagesClicked(bool clicked)
{
    identityAtCurrentIndex()->setUseAwayMessages(clicked);
}
