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

#include "nicknamewidget.hpp"
#include "dialogs/channeldialog.hpp"
#include "sql/database.hpp"
#include "sql/nickname.hpp"
#include "ui/nicknamelist.hpp"
#include <KDE/KInputDialog>
#include <KDE/KLocale>
#include <KDE/KMessageBox>
#include <KDE/KPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QSpacerItem>
using namespace Aki;

NicknameWidget::NicknameWidget(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    setLayout(mainLayout);

    _nicknameList = new Aki::NicknameList;
    mainLayout->addWidget(_nicknameList);
    connect(_nicknameList, SIGNAL(nicknameClicked(Aki::Sql::Nickname*)),
            SLOT(slotNicknameListClicked(Aki::Sql::Nickname*)));
    connect(_nicknameList, SIGNAL(nicknameCurrentRowChanged(int)),
            SLOT(slotNicknameCurrentRowChanged(int)));

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

    _moveUpButton = new KPushButton;
    buttonLayout->addWidget(_moveUpButton);
    _moveUpButton->setDisabled(true);
    _moveUpButton->setIcon(KIcon("arrow-up"));
    _moveUpButton->setText(i18n("Move Up"));
    connect(_moveUpButton, SIGNAL(clicked(bool)),
            SLOT(slotMoveUpClicked()));

    _moveDownButton = new KPushButton;
    buttonLayout->addWidget(_moveDownButton);
    _moveDownButton->setDisabled(true);
    _moveDownButton->setIcon(KIcon("arrow-down"));
    _moveDownButton->setText(i18n("Move Down"));
    connect(_moveDownButton, SIGNAL(clicked(bool)),
            SLOT(slotMoveDownClicked()));

    buttonLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

NicknameWidget::~NicknameWidget()
{
}

void
NicknameWidget::addNickname(Aki::Sql::Nickname* nickname)
{
    _nicknameList->addNickname(nickname);
}

int
NicknameWidget::count() const
{
    return _nicknameList->count();
}

Aki::Sql::Nickname*
NicknameWidget::currentNickname() const
{
    return _nicknameList->currentNickname();
}

void
NicknameWidget::insertNickname(int row, Aki::Sql::Nickname* nickname)
{
    _nicknameList->insertNickname(row, nickname);
}

Aki::Sql::Nickname*
NicknameWidget::nickname(int index) const
{
    return _nicknameList->nickname(index);
}

void
NicknameWidget::removeNickname(Aki::Sql::Nickname* nickname)
{
    _nicknameList->removeNickname(_nicknameList->row(nickname));
}

void
NicknameWidget::repopulateNicknames(Aki::Sql::Identity* identity)
{
    _nicknameList->repopulateNicknames(identity);
}

void
NicknameWidget::setCurrentAddress(Aki::Sql::Nickname* nickname)
{
    _nicknameList->setCurrentNickname(nickname);
}

void
NicknameWidget::setCurrentRow(int row)
{
    _nicknameList->setCurrentRow(row);
}

void
NicknameWidget::setDatabase(Aki::Sql::Database* database)
{
    _nicknameList->setDatabase(database);
}

void
NicknameWidget::slotAddClicked()
{
    bool ok = false;
    const QString nickname = KInputDialog::getText(i18n("Enter a new nickname"), i18n("Enter a new nickname"),
                                                   QString(), &ok, this);
    if (!ok) {
        return;
    }

    if (_nicknameList->findNicknames(nickname, Qt::MatchExactly).isEmpty()) {
        Aki::Sql::Nickname* tmp = new Aki::Sql::Nickname;
        tmp->setNickname(nickname);
        addNickname(tmp);
    } else {
        KMessageBox::error(this, i18n("Unable to add nickname. Nickname already exists."));
        slotAddClicked();
        return;
    }

    if (count() == 1) {
        setCurrentRow(0);
        slotNicknameCurrentRowChanged(0);
        _removeButton->setEnabled(true);
        _editButton->setEnabled(true);
    } else if (count() > 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
        _editButton->setEnabled(true);
    }
}

void
NicknameWidget::slotEditClicked()
{
    // Get the current address.
    Aki::Sql::Nickname* current = _nicknameList->currentNickname();

    bool ok = false;
    const QString nickname = KInputDialog::getText(i18n("Edit current nickname"), i18n("Edit current nickname"),
                                                   current->nickname(), &ok, this);

    if (!ok) {
        return;
    }

    if (current->nickname() == nickname) {
        return;
    } else if (_nicknameList->findNicknames(nickname, Qt::MatchExactly).isEmpty()) {
        current->setNickname(nickname);
        return;
    } else {
        KMessageBox::error(this, i18n("Unable to modify nickname. Nickname already exists."));
        slotEditClicked();
    }
}

void
NicknameWidget::slotMoveDownClicked()
{
    Aki::Sql::Nickname* current = _nicknameList->currentNickname();
    const int currentIndex = _nicknameList->row(current);

    Aki::Sql::Nickname* next = _nicknameList->nickname(currentIndex + 1);
    if (!next) {
        return;
    }

    const int nextIndex = _nicknameList->row(next);
    Aki::Sql::Nickname* tmp = _nicknameList->takeNickname(nextIndex);

    _nicknameList->insertNickname(currentIndex, tmp);
    _nicknameList->insertNickname(nextIndex, current);

    if ((nextIndex) == (count() - 1)) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    } else {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setEnabled(true);
    }
}

void
NicknameWidget::slotMoveUpClicked()
{
    Aki::Sql::Nickname* current = _nicknameList->currentNickname();
    const int currentIndex = _nicknameList->row(current);

    Aki::Sql::Nickname* previous = _nicknameList->nickname(currentIndex - 1);
    if (!previous) {
        return;
    }

    const int previousIndex = _nicknameList->row(previous);
    Aki::Sql::Nickname* tmp = _nicknameList->takeNickname(previousIndex);

    _nicknameList->insertNickname(previousIndex, current);
    _nicknameList->insertNickname(currentIndex, tmp);

    if ((previousIndex) == 0) {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setDisabled(true);
    } else {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setEnabled(true);
    }
}

void
NicknameWidget::slotNicknameCurrentRowChanged(int row)
{
    const Aki::Sql::Nickname* current = _nicknameList->nickname(row);
    if (!current) {
        return;
    }

    if (count() == 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setDisabled(true);
    } else if (row == (count() - 1)) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    } else if (row == 0) {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setDisabled(true);
    } else {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setEnabled(true);
    }
}

void
NicknameWidget::slotNicknameListClicked(Aki::Sql::Nickname* nickname)
{
    if (!nickname) {
        return;
    }

    _editButton->setEnabled(true);
    _removeButton->setEnabled(true);

    const int row = _nicknameList->row(nickname);

    if (count() == 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setDisabled(true);
    } else if (row == (count() - 1)) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    } else if (row == 0) {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setDisabled(true);
    } else {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setEnabled(true);
    }
}

void
NicknameWidget::slotRemoveClicked()
{
    Aki::Sql::Nickname* current = _nicknameList->currentNickname();
    if (!current) {
        return;
    }

    int result = KMessageBox::warningYesNo(this, i18n("Are you sure you want to remove the nickname\n\"%1\"?", current->nickname()),
                                           i18n("Remove nickname"));
    switch (result) {
    case KMessageBox::Yes: {
        removeNickname(current);
        if (count() == 0) {
            _editButton->setDisabled(true);
            _moveDownButton->setDisabled(true);
            _moveUpButton->setDisabled(true);
            _removeButton->setDisabled(true);
        }
        break;
    }
    default: {
        return;
    }
    }
}

Sql::Nickname*
NicknameWidget::takeNickname(int row)
{
    return _nicknameList->takeNickname(row);
}
