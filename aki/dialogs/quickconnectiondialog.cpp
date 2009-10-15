/*
    This file is part of Aki IRC client.
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

#include "quickconnectiondialog.h"
#include <KUser>

QuickConnectionDialog::QuickConnectionDialog(QWidget *parent)
    : KDialog(parent),
    m_serverAddress(QString()),
    m_serverPassword(QString()),
    m_port(6667),
    m_enableSSL(false)
{
    KUser user(KUser::UseRealUserID);
    m_nickname = user.loginName();

    QWidget *widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);

    setButtons(Ok | Cancel);
    setSizeGripEnabled(true);

    serverPort->setText(QString::number(m_port));
    nickname->setText(m_nickname);

    setButtonText(Ok, i18n("Connect"));

    connect(serverAddress, SIGNAL(textEdited(QString)),
            SLOT(serverAddressTextEdited(QString)));
    connect(serverPort, SIGNAL(textEdited(QString)),
            SLOT(serverPortTextEdited(QString)));
    connect(enableSSL, SIGNAL(toggled(bool)),
            SLOT(enableSSLToggled(bool)));
    connect(password, SIGNAL(textEdited(QString)),
            SLOT(passwordTextEdited(QString)));
    connect(nickname, SIGNAL(textEdited(QString)),
            SLOT(nicknameTextEdited(QString)));
}

void
QuickConnectionDialog::serverAddressTextEdited(const QString &address)
{
    m_serverAddress = address;
}

void
QuickConnectionDialog::serverPortTextEdited(const QString &port)
{
    m_port = port.toUShort();
}

void
QuickConnectionDialog::enableSSLToggled(bool clicked)
{
    m_enableSSL = clicked;
}

void
QuickConnectionDialog::passwordTextEdited(const QString &password)
{
    m_serverPassword = password;
}

void
QuickConnectionDialog::nicknameTextEdited(const QString &nickname)
{
    m_nickname = nickname;
}

QString
QuickConnectionDialog::address() const
{
    return m_serverAddress;
}

QString
QuickConnectionDialog::pass() const
{
    return m_serverPassword;
}

QString
QuickConnectionDialog::nick() const
{
    return m_nickname;
}

quint16
QuickConnectionDialog::port() const
{
    return m_port;
}

bool
QuickConnectionDialog::isSslEnabled() const
{
    return m_enableSSL;
}
