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

    QVBoxLayout* buttonLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);

    _addButton = new KPushButton;
    buttonLayout->addWidget(_addButton);
    _addButton->setIcon(KIcon("list-add"));
    _addButton->setText(i18n("Add"));
    connect(_addButton, SIGNAL(clicked(bool)),
            SLOT(slotAddClicked()));

    _removeButton = new KPushButton;
    buttonLayout->addWidget(_removeButton);
    _removeButton->setIcon(KIcon("list-remove"));
    _removeButton->setText(i18n("Remove"));

    _editButton = new KPushButton;
    buttonLayout->addWidget(_editButton);
    _editButton->setIcon(KIcon("edit-rename"));
    _editButton->setText(i18n("Edit"));

    _moveUpButton = new KPushButton;
    buttonLayout->addWidget(_moveUpButton);
    _moveUpButton->setIcon(KIcon("arrow-up"));
    _moveUpButton->setText(i18n("Move Up"));

    _moveDownButton = new KPushButton;
    buttonLayout->addWidget(_moveDownButton);
    _moveDownButton->setIcon(KIcon("arrow-down"));
    _moveDownButton->setText(i18n("Move Down"));

    buttonLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

AddressWidget::~AddressWidget()
{
}

void
AddressWidget::addAddress(const QString& address)
{
    Aki::Sql::Address* tmp = new Aki::Sql::Address;
    addAddress(tmp);
}

void
AddressWidget::addAddress(Aki::Sql::Address* address)
{
    _addressList->addAddress(address);
}

Aki::Sql::Address*
AddressWidget::address(int index)
{
    Q_UNUSED(index)
    return 0;
}

int
AddressWidget::count() const
{
    return -1;
}

Aki::Sql::Address*
AddressWidget::currentAddress() const
{
    return 0;
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
    Q_UNUSED(row)
    Q_UNUSED(address)
}

void
AddressWidget::removeAddress(Aki::Sql::Address* address)
{
    Q_UNUSED(address)
}

int
AddressWidget::row(Aki::Sql::Address* address)
{
    Q_UNUSED(address)
    return -1;
}

void
AddressWidget::setCurrentAddress(Aki::Sql::Address* address)
{
    Q_UNUSED(address)
}

void
AddressWidget::setCurrentRow(int row)
{
    Q_UNUSED(row)
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
            KMessageBox::error(this, i18n("Empty address given"), i18n("Empty address"));
            return;
        }

        address->setAddress(addressDialog.address());
        address->setPassword(addressDialog.password());
        address->setPort(addressDialog.port());
        qDebug() << addressDialog.isSslEnabled();
        address->setSsl(addressDialog.isSslEnabled());

        addAddress(address);
        break;
    }
    default: {
        break;
    }
    }
}

void
AddressWidget::slotEditClicked()
{
}

void
AddressWidget::slotMoveDownClicked()
{
}

void
AddressWidget::slotMoveUpClicked()
{
}

void
AddressWidget::slotRemoveClicked()
{
}

Aki::Sql::Address*
AddressWidget::takeAddress(int row)
{
    Q_UNUSED(row)
    return 0;
}
