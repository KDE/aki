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

class QuickConnectionDialog : public KDialog,
                              public Ui::QuickConnection
{
    Q_OBJECT
public:
    QuickConnectionDialog(QWidget *parent = 0);
public Q_SLOTS:
    void serverAddressTextEdited(const QString &address);
    void serverPortTextEdited(const QString &port);
    void enableSSLToggled(bool clicked);
    void passwordTextEdited(const QString &password);
    void nicknameTextEdited(const QString &nickname);
    QString address() const;
    QString pass() const;
    QString nick() const;
    quint16 port() const;
    bool isSslEnabled() const;
private:
    QString m_serverAddress;
    QString m_serverPassword;
    QString m_nickname;
    quint16 m_port;
    bool m_enableSSL;
}; // End of class QuickConnectionDialog.

#endif // QUICKCONNECTIONDIALOG_H
