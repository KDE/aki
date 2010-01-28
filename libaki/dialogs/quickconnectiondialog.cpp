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
#include <QRegExpValidator>
using namespace Aki;

namespace Aki
{
class QuickConnectionDialogPrivate
{
public:
    QuickConnectionDialogPrivate(Aki::QuickConnectionDialog *qq)
        : q(qq),
        serverAddress(QString()),
        serverPassword(QString()),
        port(6667),
        enableSsl(false)
    {
        KUser user(KUser::UseRealUserID);
        nickname = user.loginName();
    }

    void serverAddressTextEdited(const QString &address)
    {
        serverAddress = address;
    }

    void serverPortTextEdited(const QString &port)
    {
        this->port = port.toUShort();
    }

    void enableSSLToggled(bool clicked)
    {
        enableSsl = clicked;
    }

    void passwordTextEdited(const QString &password)
    {
        serverPassword = password;
    }

    void nicknameTextEdited(const QString &nickname)
    {
        this->nickname = nickname;
    }

    Aki::QuickConnectionDialog *q;
    QString serverAddress;
    QString serverPassword;
    QString nickname;
    quint16 port;
    bool enableSsl;
}; // End of class QuickConnectionDialogPrivate.
} // End of namespace Aki.

QuickConnectionDialog::QuickConnectionDialog(QWidget *parent)
    : KDialog(parent)
{
    d.reset(new QuickConnectionDialogPrivate(this));

    QWidget *widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);

    serverPort->setValidator(new QRegExpValidator(QRegExp("^\\d+$"), this));

    setButtons(Ok | Cancel);
    setSizeGripEnabled(true);

    serverPort->setText(QString::number(d->port));
    nickname->setText(d->nickname);

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

QuickConnectionDialog::~QuickConnectionDialog()
{
}

QString
QuickConnectionDialog::address() const
{
    return d->serverAddress;
}

QString
QuickConnectionDialog::pass() const
{
    return d->serverPassword;
}

QString
QuickConnectionDialog::nick() const
{
    return d->nickname;
}

quint16
QuickConnectionDialog::port() const
{
    return d->port;
}

bool
QuickConnectionDialog::isSslEnabled() const
{
    return d->enableSsl;
}

#include "quickconnectiondialog.moc"
