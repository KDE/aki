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

namespace Aki
{
class IdentityConfig;
} // End of namespace IdentityConfig.

class IdentityDialog : public KDialog,
                       public Ui::IdentityDialog
{
    Q_OBJECT
public:
    IdentityDialog(QWidget *parent = 0);
    void loadSettings();
    void newIdentitySettings(const QString &name);
    void updateSettings();
private Q_SLOTS:
    void slotAddIdentityClicked();
    void slotIdentityActivated(int index);
    void slotEditIdentityClicked();
    void slotDeleteIdentityClicked();
    void slotAwayMessageTextEdited(const QString &message);
    void slotAwayNicknameTextEdited(const QString &nickname);
    void slotKickMessageTextEdited(const QString &message);
    void slotMarkLastPositionClicked(bool clicked);
    void slotNicknameChanged();
    void slotOkClicked();
    void slotPartMessageTextEdited(const QString &message);
    void slotQuitMessageTextEdited(const QString &message);
    void slotRealNameTextEdited(const QString &name);
    void slotReturnMessageTextEdited(const QString &message);
    void slotUseAwayMessagesClicked(bool clicked);
private:
    Aki::IdentityPointer identityAtCurrentIndex();
private:
    Aki::IdentityList m_identityList;
    Aki::IdentityConfig *m_identityConfig;
}; // End of class IdentityDialog.

#endif // IDENTITYDIALOG_H
