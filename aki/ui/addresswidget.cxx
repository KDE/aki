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


#include "addresswidget.hpp"
#include "dialogs/addressdialog.hpp"
#include "sql/address.hpp"
#include "sql/database.hpp"
#include "sql/server.hpp"
#include "ui/addresslist.hpp"
#include <QtGui/QHBoxLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <KDE/KLocale>
#include <KDE/KMessageBox>
#include <KDE/KPushButton>
using namespace Aki;

AddressWidget::AddressWidget(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    setLayout(mainLayout);

    _addressList = new Aki::AddressList;
    mainLayout->addWidget(_addressList);
    connect(_addressList, SIGNAL(addressClicked(Aki::Sql::Address*)),
            SLOT(slotAddressListClicked(Aki::Sql::Address*)));
    connect(_addressList, SIGNAL(addressCurrentRowChanged(int)),
            SLOT(slotAddressCurrentRowChanged(int)));

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

AddressWidget::~AddressWidget()
{
}

void
AddressWidget::addAddress(const QString& address)
{
    Q_UNUSED(address)
    Aki::Sql::Address* tmp = new Aki::Sql::Address;
    addAddress(tmp);
}

void
AddressWidget::addAddress(Aki::Sql::Address* address)
{
    _addressList->addAddress(address);

    if (count() == 1) {
        setCurrentRow(0);
        slotAddressCurrentRowChanged(0);
        _removeButton->setEnabled(true);
    } else if (count() > 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    }
}

Aki::Sql::Address*
AddressWidget::address(int index)
{
    return _addressList->address(index);
}

int
AddressWidget::count() const
{
    return _addressList->count();
}

Aki::Sql::Address*
AddressWidget::currentAddress() const
{
    return _addressList->currentAddress();
}

void
AddressWidget::insertAddress(int row, const QString& address)
{
    Q_UNUSED(row)
    Q_UNUSED(address)
}

void
AddressWidget::insertAddress(int row, Aki::Sql::Address* address)
{
    _addressList->insertAddress(row, address);

    if (count() == 1) {
        setCurrentRow(0);
        slotAddressCurrentRowChanged(0);
        _removeButton->setEnabled(true);
    } else if (count() > 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    }
}

void
AddressWidget::removeAddress(Aki::Sql::Address* address)
{
    _addressList->removeAddress(_addressList->row(address));

    if (count() == 0) {
        _editButton->setDisabled(true);
        _moveDownButton->setDisabled(true);
        _moveUpButton->setDisabled(true);
        _removeButton->setDisabled(true);
    }
}

void
AddressWidget::reorderPosition(int current)
{
    for (int i = current, c = count(); i < c; ++i) {
        _addressList->address(i)->setPosition(i);
    }
}

void
AddressWidget::repopulateAddresses(Aki::Sql::Server* server)
{
    _addressList->repopulateAddresses(server);

    if (count() == 1) {
        setCurrentRow(0);
        slotAddressCurrentRowChanged(0);
        _removeButton->setEnabled(true);
    } else if (count() > 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    } else if (count() == 0) {
        _editButton->setDisabled(true);
        _moveDownButton->setDisabled(true);
        _moveUpButton->setDisabled(true);
        _removeButton->setDisabled(true);
    }
}

int
AddressWidget::row(Aki::Sql::Address* address)
{
    return _addressList->row(address);
}

void
AddressWidget::setCurrentAddress(Aki::Sql::Address* address)
{
    _addressList->setCurrentAddress(address);
}

void
AddressWidget::setCurrentRow(int row)
{
    _addressList->setCurrentRow(row);
}

void
AddressWidget::setDatabase(Aki::Sql::Database* database)
{
    _addressList->setDatabase(database);
}

void
AddressWidget::slotAddClicked()
{
    Aki::AddressDialog addressDialog;

    switch (addressDialog.exec()) {
    case QDialog::Accepted: {
        Aki::Sql::Address* address = new Aki::Sql::Address;
        if (!address || addressDialog.address().isEmpty()) {
            delete address;
            KMessageBox::error(this, i18n("Empty address given."), i18n("Empty address"));
            return;
        }

        address->setAddress(addressDialog.address());
        address->setPassword(addressDialog.password());
        address->setPort(addressDialog.port());
        address->setPosition(count());
        address->setSsl(addressDialog.isSslEnabled());

        if (!_addressList->findAddresses(address->address() + ':' + QString::number(address->port()), Qt::MatchExactly).isEmpty()) {
            delete address;
            KMessageBox::error(this, i18n("Address already exists, please enter a different address"),
                               i18n("Address already in use."));
            return;
        }

        addAddress(address);
        break;
    }
    default: {
        break;
    }
    }
}

void
AddressWidget::slotAddressCurrentRowChanged(int row)
{
    const Aki::Sql::Address* current = _addressList->address(row);
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
        _moveUpButton->setDisabled(true);
    }

    _editButton->setEnabled(true);
}

void
AddressWidget::slotAddressListClicked(Aki::Sql::Address* address)
{
    if (!address) {
        return;
    }

    _editButton->setEnabled(true);
    _removeButton->setEnabled(true);

    const int count = this->count() - 1;
    const int row = _addressList->row(address);

    if (this->count() == 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setDisabled(true);
    } else if (count == row) {
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
AddressWidget::slotEditClicked()
{
    // Get the current address.
    Aki::Sql::Address* currentAddress = _addressList->currentAddress();

    // Can we safely remove this check?
    if (!currentAddress) {
        return;
    }

    Aki::AddressDialog dialog;
    dialog.setAddress(currentAddress->address());
    dialog.setPassword(currentAddress->password());
    dialog.setPort(currentAddress->port());
    dialog.setSsl(currentAddress->isSslEnabled());

    switch (dialog.exec()) {
    case QDialog::Accepted: {
        if (currentAddress->address() == dialog.address() &&
            currentAddress->password() == dialog.password() &&
            currentAddress->port() == dialog.port() &&
            currentAddress->isSslEnabled() == dialog.isSslEnabled()) {
            return;
        }

        if (currentAddress->address() != dialog.address()) {
            if (!_addressList->findAddresses(dialog.address(), Qt::MatchExactly).isEmpty()) {
                KMessageBox::error(this, i18n("Address already exists, please enter a different address"),
                                   i18n("Address already in use."));
                return;
            }
        }

        currentAddress->setAddress(dialog.address());
        currentAddress->setPassword(dialog.password());
        currentAddress->setPort(dialog.port());
        currentAddress->setSsl(dialog.isSslEnabled());
        return;
    }
    default: {
        break;
    }
    }
}

void
AddressWidget::slotMoveDownClicked()
{
    Aki::Sql::Address* current = _addressList->currentAddress();
    const int currentIndex = _addressList->row(current);

    Aki::Sql::Address* next = _addressList->address(currentIndex + 1);
    if (!next) {
        return;
    }
    const int nextIndex = _addressList->row(next);

    Aki::Sql::Address* tmp = _addressList->takeAddress(nextIndex);

    tmp->setPosition(currentIndex);
    _addressList->insertAddress(currentIndex, tmp);

    current->setPosition(nextIndex);
    _addressList->insertAddress(nextIndex, current);

    if ((nextIndex) == (count() - 1)) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    } else {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setEnabled(true);
    }
}

void
AddressWidget::slotMoveUpClicked()
{
    Aki::Sql::Address* current = _addressList->currentAddress();
    const int currentIndex = _addressList->row(current);

    Aki::Sql::Address* previous = _addressList->address(currentIndex - 1);
    if (!previous) {
        return;
    }
    const int previousIndex = _addressList->row(previous);

    Aki::Sql::Address* tmp = _addressList->takeAddress(previousIndex);

    current->setPosition(previousIndex);
    _addressList->insertAddress(previousIndex, current);

    tmp->setPosition(currentIndex);
    _addressList->insertAddress(currentIndex, tmp);

    if ((previousIndex) == 0) {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setDisabled(true);
    } else {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setEnabled(true);
    }
}

void
AddressWidget::slotRemoveClicked()
{
    Aki::Sql::Address* current = _addressList->currentAddress();
    if (!current) {
        return;
    }

    int result = KMessageBox::warningYesNo(this, i18n("Are you sure you want to remove the server\n\"%1\"?", current->address()),
                                           i18n("Remove server"));
    switch (result) {
    case KMessageBox::Yes: {
        const int position = _addressList->row(current);
        if (position == (count() - 1)) {
            removeAddress(current);
        } else {
            removeAddress(current);
            reorderPosition(position);
        }
        break;
    }
    case KMessageBox::No:
    default: {
        return;
    }
    }
}

Aki::Sql::Address*
AddressWidget::takeAddress(int row)
{
    Aki::Sql::Address* tmp = _addressList->takeAddress(row);

    if (count() == 0) {
        _editButton->setDisabled(true);
        _moveDownButton->setDisabled(true);
        _moveUpButton->setDisabled(true);
        _removeButton->setDisabled(true);
    }

    return tmp;
}
