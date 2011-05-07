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

#ifndef AKI_IMPORTIDENTITYPAGE_HPP
#define AKI_IMPORTIDENTITYPAGE_HPP

#include "ui_importidentitypage.h"
#include <QtGui/QWidget>

namespace Aki
{
namespace Sql
{
class Database;
class Identity;
} // End of namespace Sql.
class ImportIdentityPage
    : public QWidget,
      private Ui::ImportIdentityPage
{
    Q_OBJECT
public:
    explicit ImportIdentityPage(Aki::Sql::Database* database, QWidget* parent = 0);
    ~ImportIdentityPage();
private Q_SLOTS:
    void slotAddIdentityClicked();
    void slotAwayMessageTextEdited(const QString& message);
    void slotAwayMessagesToggled(bool enabled);
    void slotAwayNicknameTextEdited(const QString& nickname);
    void slotCopyIdentityClicked();
    void slotEditIdentityClicked();
    void slotIdentityActivated(Aki::Sql::Identity* identity);
    void slotKickMessageTextEdited(const QString& message);
    void slotMarkLastPositionToggled(bool enabled);
    void slotPartMessageTextEdited(const QString& message);
    void slotQuitMessageTextEdited(const QString& message);
    void slotRealNameTextEdited(const QString& name);
    void slotRemoveIdentityClicked();
    void slotReturnMessageTextEdited(const QString& message);
private:
    void loadNewIdentity();
    /**
     * Updates the widgets with the identity information.
     *
     * @p identity Identity to populate widgets with.
     */
    void setWidgetValues(Aki::Sql::Identity* identity);
    /**
     * Updates the identity in the database using transactions.
     *
     * @p identity Identity that needs to be updated.
     * @return true if update was successful; false otherwise.
     */
    bool saveIdentity(Aki::Sql::Identity* identity);
private:
    Aki::Sql::Identity* _identity;
    Aki::Sql::Database* _database;
}; // End of class ImportIdentityPage.
} // End of class QWidget.

#endif // AKI_IMPORTIDENTITYPAGE_HPP
