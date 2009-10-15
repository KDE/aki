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

#ifndef IDENTITYWELCOMEPAGE_H
#define IDENTITYWELCOMEPAGE_H

#include "identity.h"
#include "ui_identitywelcomepage.h"
#include <QWidget>

namespace Aki
{
class IdentityConfig;
}

class IdentityWelcomePage : public QWidget,
                            private Ui::IdentityWelcomePage
{
    Q_OBJECT
public:
    IdentityWelcomePage(QWidget *parent = 0);
    ~IdentityWelcomePage();
    void loadNewIdentity();
    void save();
private Q_SLOTS:
    void awayMessageTextEdited(const QString &message);
    void markLastPositionClicked(bool checked);
    void nicknameListChanged();
    void realNameTextEdited(const QString &name);
    void returnMessageTextEdited(const QString &message);
    void useAwayMessagesClicked(bool checked);
private:
    Aki::IdentityConfig *m_config;
    Aki::IdentityPointer m_identity;
};

#endif // IDENTITYWELCOMEPAGE_H
