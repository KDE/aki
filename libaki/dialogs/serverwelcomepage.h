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

#ifndef SERVERWELCOMEPAGE_H
#define SERVERWELCOMEPAGE_H

#include "ui_serverwelcomepage.h"
#include <QScopedPointer>

namespace Aki
{
class ServerConfig;
class ServerWelcomePagePrivate;
class ServerWelcomePage : public QWidget,
                          private Ui::ServerWelcomePage
{
    Q_OBJECT
public:
    ServerWelcomePage(QWidget *parent = 0);
    ~ServerWelcomePage();
    void save();
private:
    Q_PRIVATE_SLOT(d, void channelListChanged())
    Q_PRIVATE_SLOT(d, void networkNameTextEdited(const QString &name))
    Q_PRIVATE_SLOT(d, void serverListChanged())
    Q_PRIVATE_SLOT(d, void serviceNameTextEdited(const QString &name))
    Q_PRIVATE_SLOT(d, void servicePasswordTextEdited(const QString &password))
    Q_PRIVATE_SLOT(d, void useAutoIdentifyClicked(bool checked))
private:
    friend class ServerWelcomePagePrivate;
    QScopedPointer<ServerWelcomePagePrivate> d;
}; // End of class ServerWelcomePage.
} // End of namespace Aki.

#endif // SERVERWELCOMEPAGE_H
