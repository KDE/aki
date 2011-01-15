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

#include "quickconnectiondialog.hpp"
#include <KDE/KUser>
using namespace Aki;

QuickConnectionDialog::QuickConnectionDialog(QWidget* parent)
    : KDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    QWidget* widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);

    setWindowTitle(widget->windowTitle());

    KUser user(KUser::UseRealUserID);

    setServerPort(6667);
    setNickname(user.loginName());

    setButtons(KDialog::Ok | KDialog::Cancel);
    setSizeGripEnabled(true);

    setButtonText(KDialog::Ok, i18n("Connect"));
}

QuickConnectionDialog::~QuickConnectionDialog()
{
}

bool
QuickConnectionDialog::isSslEnabled() const
{
    return sslCheckBox->isChecked();
}

QString
QuickConnectionDialog::nickname() const
{
    return nicknameLineEdit->text();
}

QString
QuickConnectionDialog::serverAddress() const
{
    return serverAddressLineEdit->text();
}

QString
QuickConnectionDialog::serverPassword() const
{
    return serverPasswordLineEdit->text();
}

quint16
QuickConnectionDialog::serverPort() const
{
    return serverPortSpinBox->value();
}

void
QuickConnectionDialog::setNickname(const QString& nickname)
{
    nicknameLineEdit->setText(nickname);
}

void
QuickConnectionDialog::setServerAddress(const QString& address)
{
    serverAddressLineEdit->setText(address);
}

void
QuickConnectionDialog::setServerPassword(const QString& password)
{
    serverPasswordLineEdit->setText(password);
}

void
QuickConnectionDialog::setServerPort(quint16 port)
{
    serverPortSpinBox->setValue(port);
}

void
QuickConnectionDialog::setSsl(bool enabled)
{
    sslCheckBox->setChecked(enabled);
}
