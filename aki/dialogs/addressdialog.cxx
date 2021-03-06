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

#include "addressdialog.hpp"
#include "aki.hpp"
#include <KDE/KIcon>
#include <KDE/KIntSpinBox>
#include <KDE/KLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
using namespace Aki;

AddressDialog::AddressDialog(QWidget* parent)
    : KDialog(parent)
{
    setButtons(KDialog::Ok | KDialog::Cancel);
    QWidget* widget = new QWidget(this);

    QGridLayout* layout = new QGridLayout(this);

    QFormLayout* mainLayout = new QFormLayout;
    layout->addLayout(mainLayout, 0, 0, 1, 1);

    QLabel* label = new QLabel(i18n("Server address:"), this);
    QLabel* label2 = new QLabel(i18n("Server password:"), this);
    QLabel* label3 = new QLabel(i18n("Server port:"), this);

    mainLayout->setWidget(0, QFormLayout::LabelRole, label);
    _serverAddressLineEdit = new KLineEdit(this);
    mainLayout->setWidget(0, QFormLayout::FieldRole, _serverAddressLineEdit);

    mainLayout->setWidget(1, QFormLayout::LabelRole, label2);
    _serverPasswordLineEdit = new KLineEdit(this);
    _serverPasswordLineEdit->setClearButtonShown(true);
    _serverPasswordLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    _serverPasswordLineEdit->setPasswordMode(true);
    _serverPasswordLineEdit->setUrlDropsEnabled(false);
    mainLayout->setWidget(1, QFormLayout::FieldRole, _serverPasswordLineEdit);

    mainLayout->setWidget(2, QFormLayout::LabelRole, label3);
    _serverPortSpinBox = new KIntSpinBox(0, 65535, 1, 6667, this);
    _serverPortSpinBox->setAccelerated(true);
    mainLayout->setWidget(2, QFormLayout::FieldRole, _serverPortSpinBox);

    _useSslCheckBox = new QCheckBox(i18n("Use Ssl"), this);
    _useSslCheckBox->setTristate(false);
    _useSslCheckBox->setIcon(KIcon("document-encrypt"));
    //_useSslCheckBox->setIconSize(QSize(16, 16));
    mainLayout->setWidget(3, QFormLayout::SpanningRole, _useSslCheckBox);

#ifndef QT_NO_SHORTCUT
    label->setBuddy(_serverAddressLineEdit);
    label2->setBuddy(_serverPasswordLineEdit);
    label3->setBuddy(_serverPortSpinBox);
#endif // QT_NO_SHORTCUT

    widget->setLayout(layout);
    setMainWidget(widget);
}

AddressDialog::~AddressDialog()
{
}

QString
AddressDialog::address() const
{
    return _serverAddressLineEdit->text();
}

bool
AddressDialog::isSslEnabled() const
{
    return _useSslCheckBox->isChecked();
}

QString
AddressDialog::password() const
{
    return _serverPasswordLineEdit->text();
}

unsigned int
AddressDialog::port() const
{
    return _serverPortSpinBox->value();
}

void
AddressDialog::setAddress(const QString& name)
{
    _serverAddressLineEdit->setText(name);
}

void
AddressDialog::setPassword(const QString& name)
{
    _serverPasswordLineEdit->setText(name);
}

void
AddressDialog::setPort(quint16 port)
{
    _serverPortSpinBox->setValue(port);
}

void
AddressDialog::setSsl(bool enabled)
{
    _useSslCheckBox->setChecked(enabled);
}
