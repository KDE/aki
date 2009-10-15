/*
    This file is part of Aki IRC.
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

#ifndef IDENTITYDIALOG_H
#define IDENTITYDIALOG_H

#include "identity.h"
#include "ui_identitydialog.h"
#include <KDialog>
#include <QScopedPointer>

namespace Aki
{
class IdentityConfig;
class IdentityDialogPrivate;
class IdentityDialog : public KDialog,
                       public Ui::IdentityDialog
{
    Q_OBJECT
public:
    IdentityDialog(QWidget *parent = 0);
    ~IdentityDialog();
    void updateSettings();
private:
    Q_PRIVATE_SLOT(d, void addIdentityClicked())
    Q_PRIVATE_SLOT(d, void identityActivated(int index))
    Q_PRIVATE_SLOT(d, void editIdentityClicked())
    Q_PRIVATE_SLOT(d, void deleteIdentityClicked())
    Q_PRIVATE_SLOT(d, void awayMessageTextEdited(const QString &message))
    Q_PRIVATE_SLOT(d, void awayNicknameTextEdited(const QString &nickname))
    Q_PRIVATE_SLOT(d, void kickMessageTextEdited(const QString &message))
    Q_PRIVATE_SLOT(d, void markLastPositionClicked(bool clicked))
    Q_PRIVATE_SLOT(d, void nicknameChanged())
    Q_PRIVATE_SLOT(d, void _okClicked())
    Q_PRIVATE_SLOT(d, void partMessageTextEdited(const QString &message))
    Q_PRIVATE_SLOT(d, void quitMessageTextEdited(const QString &message))
    Q_PRIVATE_SLOT(d, void realNameTextEdited(const QString &name))
    Q_PRIVATE_SLOT(d, void returnMessageTextEdited(const QString &message))
    Q_PRIVATE_SLOT(d, void useAwayMessagesClicked(bool clicked))
private:
    friend class IdentityDialogPrivate;
    QScopedPointer<IdentityDialogPrivate> d;
}; // End of class IdentityDialog.
} // End of namespace Aki.

#endif // IDENTITYDIALOG_H
