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
#include "identity.h"
#include "config/identityconfig.h"
#include <KInputDialog>
#include <KMessageBox>
using namespace Aki;

namespace Aki
{
class IdentityDialogPrivate
{
public:
    IdentityDialogPrivate(Aki::IdentityDialog *qq)
        : q(qq),
        identityConfig(0)
    {
        identityList.clear();
    }

    void loadSettings()
    {
        QStringListIterator identityIter(identityConfig->groupList());
        while (identityIter.hasNext()) {
            QString identityName = identityIter.next();

            // Switch to the new identity group.
            identityConfig->setCurrentGroup(identityName);

            // Add the identity to the identity selector.
            q->identitySelector->addItem(identityName);

            // Create the identity object.
            Aki::IdentityPointer identity(new Aki::Identity(identityName));
            identity->setAwayMessage(identityConfig->awayMessage());
            identity->setAwayNickname(identityConfig->awayNickname());
            identity->setKickMessage(identityConfig->kickMessage());
            identity->setMarkLastPosition(identityConfig->isMarkLastPositionEnabled());
            identity->setNicknameList(identityConfig->nicknameList());
            identity->setPartMessage(identityConfig->partMessage());
            identity->setQuitMessage(identityConfig->quitMessage());
            identity->setRealName(identityConfig->realName());
            identity->setReturnMessage(identityConfig->returnMessage());
            identity->setUseAwayMessages(identityConfig->isAwayMessagesEnabled());

            q->identitySelector->model()->sort(0);
            const int index = q->identitySelector->findText(identityName, Qt::MatchExactly);
            identityList.insert(index, identity);
        }

        if (q->identitySelector->count() == 1) {
            q->deleteIdentityButton->setDisabled(true);
        } else {
            q->deleteIdentityButton->setEnabled(true);
        }

        q->identitySelector->setCurrentIndex(0);
        identityActivated(0);
    }

    void addIdentityClicked()
    {
        bool ok = false;

        // Get the identity name.
        QString identity = KInputDialog::getText(i18n("Add a new identity"),
                                                i18n("Enter a new identity name"), QString(), &ok, q);

        // Check that the identity isn't empty and not found.
        if (ok && !identity.isEmpty() &&
            q->identitySelector->findText(identity, Qt::MatchExactly) == -1) {
            newIdentitySettings(identity);
        } else if (ok && !identity.isEmpty() && // Check that the identity isn't empty and already exists.
                !q->identitySelector->findText(identity) >= 0) {
            KMessageBox::error(q, i18n("Unable to add identity. Identity name already exists."));
            addIdentityClicked();
        } else if (ok && identity.isEmpty()) { // Check to see if it's empty.
            KMessageBox::error(q, i18n("Unable to add identity. Identity name is empty."));
            addIdentityClicked();
        }

        if (q->identitySelector->count() > 1) {
            q->deleteIdentityButton->setEnabled(true);
        }
    }

    void newIdentitySettings(const QString &name)
    {
        // Create the new group.
        identityConfig->createGroup(name);
        // Add the new identity to the identity selector.
        q->identitySelector->addItem(name);

        // Create the new identity.
        Aki::IdentityPointer identity(new Aki::Identity(name));
        identity->setAwayMessage(identityConfig->awayMessage());
        identity->setAwayNickname(identityConfig->awayNickname());
        identity->setKickMessage(identityConfig->kickMessage());
        identity->setMarkLastPosition(identityConfig->isMarkLastPositionEnabled());
        identity->setNicknameList(identityConfig->nicknameList());
        identity->setPartMessage(identityConfig->partMessage());
        identity->setQuitMessage(identityConfig->quitMessage());
        identity->setRealName(identityConfig->realName());
        identity->setReturnMessage(identityConfig->returnMessage());
        identity->setUseAwayMessages(identityConfig->isAwayMessagesEnabled());

        q->identitySelector->model()->sort(0);
        const int index = q->identitySelector->findText(name, Qt::MatchExactly);
        identityList.insert(index, identity);
        q->identitySelector->setCurrentIndex(index);
        identityActivated(index);
    }

    void identityActivated(int index)
    {
        Aki::IdentityPointer identity(identityList.value(index));
        q->nickList->setItems(identity->nicknameList());
        q->markLastPosition->setChecked(identity->isMarkLastPositionEnabled());
        q->awayNickname->setText(identity->awayNickname());
        q->useAwayMessages->setChecked(identity->isAwayMessagesEnabled());
        q->awayMessage->setText(identity->awayMessage());
        q->returnMessage->setText(identity->returnMessage());
        q->kickMessage->setText(identity->kickMessage());
        q->partMessage->setText(identity->partMessage());
        q->realName->setText(identity->realName());
        q->quitMessage->setText(identity->quitMessage());
    }

    Aki::IdentityPointer identityAtCurrentIndex()
    {
        return identityList.value(q->identitySelector->currentIndex());
    }

    void editIdentityClicked()
    {
        bool ok = false;

        // Get the current identity name.
        const QString oldName = q->identitySelector->currentText();
        int index = 0;

        // Get the new identity name.
        QString newName = KInputDialog::getText(i18n("Edit current identity."),
                                                i18n("Enter a new identity name."), oldName, &ok, q);

        // Check if new name isn't empty and the old name isn't the same
        // as the new name.
        if (ok && !newName.isEmpty() && oldName != newName) {
            // Check to see if the identity exists
            if (q->identitySelector->findText(newName) == -1) {
                // Delete the old group.
                identityConfig->deleteGroup(oldName);
                // Create the new group.
                identityConfig->createGroup(newName);

                // Search through the identity list.
                QMutableListIterator<Aki::IdentityPointer> identityIter(identityList);
                while (identityIter.hasNext()) {
                    Aki::IdentityPointer identity = identityIter.next();

                    if (identity->name() == oldName) {
                        // Get the index of the identity.
                        index = identityList.indexOf(identity);
                        // Change the old name to the new one in the identity selector.
                        q->identitySelector->setItemText(index, newName);
                        // Sort the identity selector.
                        q->identitySelector->model()->sort(0);
                        // Copy the old identity.
                        Aki::IdentityPointer newIdent = identityList.value(index);
                        // Set the old identity name to the new one.
                        newIdent->setName(newName);
                        // Remove it from the list.
                        identityList.removeAt(index);
                        // Get the index of the new name in the identity selector.
                        index = q->identitySelector->findText(newName);
                        identityList.insert(index, newIdent);
                    }
                }
            } else {
                KMessageBox::error(q, i18n("Unable to change identity name. Identity name is already taken."));
                editIdentityClicked();
            }
        } else if (ok && !newName.isEmpty() && oldName == newName) {
            KMessageBox::error(q, i18n("Unable to change identity name. Identity name cannot be the same as the old one."));
            editIdentityClicked();
        } else if (ok && newName.isEmpty()) {
            KMessageBox::error(q, i18n("Unable to change identity name. Identity name is empty."));
            editIdentityClicked();
        }
    }

    void deleteIdentityClicked()
    {
        if (q->identitySelector->count() == 1) {
            return;
        }

        // Get the current identity name.
        const QString identityName = q->identitySelector->currentText();
        int index = 0;

        QListIterator<Aki::IdentityPointer> identityIter(identityList);
        while (identityIter.hasNext()) {
            Aki::IdentityPointer identity = identityIter.next();
            if (identity->name() == identityName) {
                index = identityList.indexOf(identity);
            }
        }

        // Remove the identity name from the identity selector.
        q->identitySelector->removeItem(q->identitySelector->currentIndex());
        // Delete the settings group for the identity.
        identityConfig->deleteGroup(identityName);
        // Remove the identity from the list.
        identityList.removeAt(index);

        if (q->identitySelector->count() == 1) {
            q->deleteIdentityButton->setDisabled(true);
        }
    }

    void awayMessageTextEdited(const QString &message)
    {
        identityAtCurrentIndex()->setAwayMessage(message);
    }

    void awayNicknameTextEdited(const QString &nickname)
    {
        identityAtCurrentIndex()->setAwayNickname(nickname);
    }

    void kickMessageTextEdited(const QString &message)
    {
        identityAtCurrentIndex()->setKickMessage(message);
    }

    void markLastPositionClicked(bool clicked)
    {
        identityAtCurrentIndex()->setMarkLastPosition(clicked);
    }

    void nicknameChanged()
    {
        identityAtCurrentIndex()->setNicknameList(q->nickList->items());
    }

    void _okClicked()
    {
        q->updateSettings();
    }

    void partMessageTextEdited(const QString &message)
    {
        identityAtCurrentIndex()->setPartMessage(message);
    }

    void quitMessageTextEdited(const QString &message)
    {
        identityAtCurrentIndex()->setQuitMessage(message);
    }

    void realNameTextEdited(const QString &name)
    {
        identityAtCurrentIndex()->setRealName(name);
    }

    void returnMessageTextEdited(const QString &message)
    {
        identityAtCurrentIndex()->setReturnMessage(message);
    }

    void useAwayMessagesClicked(bool clicked)
    {
        identityAtCurrentIndex()->setUseAwayMessages(clicked);
    }

    Aki::IdentityDialog *q;
    Aki::IdentityList identityList;
    Aki::IdentityConfig *identityConfig;
}; // End of class IdentityDialogPrivate.
} // End of namespace Aki.

IdentityDialog::IdentityDialog(QWidget *parent)
    : KDialog(parent)
{
    d.reset(new Aki::IdentityDialogPrivate(this));

    QWidget *widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);

    setButtons(Ok | Cancel);
    setSizeGripEnabled(true);

    addIdentityButton->setIcon(KIcon("list-add-user"));
    deleteIdentityButton->setIcon(KIcon("list-remove-user"));
    editIdentityButton->setIcon(KIcon("edit-rename"));

    d->identityConfig = new Aki::IdentityConfig(this);

    connect(identitySelector, SIGNAL(activated(int)),
            SLOT(identityActivated(int)));
    connect(addIdentityButton, SIGNAL(clicked(bool)),
            SLOT(addIdentityClicked()));
    connect(deleteIdentityButton, SIGNAL(clicked(bool)),
            SLOT(deleteIdentityClicked()));
    connect(editIdentityButton, SIGNAL(clicked(bool)),
            SLOT(editIdentityClicked()));
    connect(awayMessage, SIGNAL(textEdited(QString)),
            SLOT(awayMessageTextEdited(QString)));
    connect(awayNickname, SIGNAL(textEdited(QString)),
            SLOT(awayNicknameTextEdited(QString)));
    connect(kickMessage, SIGNAL(textEdited(QString)),
            SLOT(kickMessageTextEdited(QString)));
    connect(markLastPosition, SIGNAL(clicked(bool)),
            SLOT(markLastPositionClicked(bool)));
    connect(nickList, SIGNAL(changed()),
            SLOT(nicknameChanged()));
    connect(partMessage, SIGNAL(textEdited(QString)),
            SLOT(partMessageTextEdited(QString)));
    connect(quitMessage, SIGNAL(textEdited(QString)),
            SLOT(quitMessageTextEdited(QString)));
    connect(realName, SIGNAL(textEdited(QString)),
            SLOT(realNameTextEdited(QString)));
    connect(returnMessage, SIGNAL(textEdited(QString)),
            SLOT(returnMessageTextEdited(QString)));
    connect(useAwayMessages, SIGNAL(clicked(bool)),
            SLOT(useAwayMessagesClicked(bool)));
    connect(this, SIGNAL(okClicked()),
            SLOT(_okClicked()));

    d->loadSettings();
}

IdentityDialog::~IdentityDialog()
{
}

void
IdentityDialog::updateSettings()
{
    QListIterator<Aki::IdentityPointer> identityIter(d->identityList);
    while (identityIter.hasNext()) {
        Aki::IdentityPointer identity = identityIter.next();
        d->identityConfig->setCurrentGroup(identity->name());
        d->identityConfig->setAwayMessage(identity->awayMessage());
        d->identityConfig->setAwayNickname(identity->awayNickname());
        d->identityConfig->setKickMessage(identity->kickMessage());
        d->identityConfig->setMarkLastPosition(identity->isMarkLastPositionEnabled());
        d->identityConfig->setNicknameList(identity->nicknameList());
        d->identityConfig->setPartMessage(identity->partMessage());
        d->identityConfig->setQuitMessage(identity->quitMessage());
        d->identityConfig->setRealName(identity->realName());
        d->identityConfig->setReturnMessage(identity->returnMessage());
        d->identityConfig->setUseAwayMessages(identity->isAwayMessagesEnabled());
    }

    d->identityConfig->save();
}

#include "identitydialog.moc"
