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

#ifndef QUICKCONNECTIONDIALOG_H
#define QUICKCONNECTIONDIALOG_H

#include "ui_quickconnection.h"
#include <KDialog>
#include <QScopedPointer>

namespace Aki
{
class QuickConnectionDialogPrivate;
class QuickConnectionDialog : public KDialog,
                              public Ui::QuickConnection
{
    Q_OBJECT
public:
    QuickConnectionDialog(QWidget *parent = 0);
    ~QuickConnectionDialog();
    QString address() const;
    QString pass() const;
    QString nick() const;
    quint16 port() const;
    bool isSslEnabled() const;
public:
    Q_PRIVATE_SLOT(d, void serverAddressTextEdited(const QString &address))
    Q_PRIVATE_SLOT(d, void serverPortTextEdited(const QString &port))
    Q_PRIVATE_SLOT(d, void enableSSLToggled(bool clicked))
    Q_PRIVATE_SLOT(d, void passwordTextEdited(const QString &password))
    Q_PRIVATE_SLOT(d, void nicknameTextEdited(const QString &nickname))
private:
    friend class QuickConnectionDialogPrivate;
    QScopedPointer<QuickConnectionDialogPrivate> d;
}; // End of class QuickConnectionDialog.
} // End of namespace Aki.

#endif // QUICKCONNECTIONDIALOG_H
