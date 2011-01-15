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

#ifndef AKI_IDENTITYDIALOG_HPP
#define AKI_IDENTITYDIALOG_HPP

#include "ui_identitydialog.h"
#include <KDE/KDialog>

namespace Aki
{
class IdentityDialog : public KDialog,
                       private Ui::IdentityDialog
{
    Q_OBJECT
public:
    explicit IdentityDialog(QWidget* parent = 0);
    ~IdentityDialog();
private Q_SLOTS:
    void slotAddIdentityClicked();
    void slotAddNicknameClicked();
    void slotAwayMessagesClicked(bool checked);
    void slotAwayMessageTextEdited(const QString& message);
    void slotAwayNicknameTextEdited(const QString& nickname);
    void slotEditIdentityClicked();
    void slotEditNicknameClicked();
    void slotIdentityActivated(Aki::SqlIdentity* identity);
    void slotKickMessageTextEdited(const QString& message);
    void slotMarkLastPositionClicked(bool checked);
    void slotPartMessageTextEdited(const QString& message);
    void slotQuitMessageTextEdited(const QString& message);
    void slotRealNameTextEdited(const QString& name);
    void slotRemoveIdentityClicked();
    void slotRemoveNicknameClicked();
    void slotReturnMessageTextEdited(const QString& message);
private:
    void createNewIdentity(const QString& name);
private:
    void setupActions();
    void setupDialog();
    void setupIcons();
}; // End of class IdentityDialog.
} // End of namespace Aki.

#endif // AKI_IDENTITYDIALOG_HPP
