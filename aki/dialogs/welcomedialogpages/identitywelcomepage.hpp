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

#ifndef AKI_IDENTITYWELCOMEPAGE_HPP
#define AKI_IDENTITYWELCOMEPAGE_HPP

#include <QtGui/QWidget>

class KEditListBox;
class KLineEdit;
class QGroupBox;
namespace Aki
{
namespace Sql
{
class Database;
class Identity;
} // End of namespace Sql.
class NicknameWidget;
class IdentityWelcomePage
    : public QWidget
{
    Q_OBJECT
public:
    explicit IdentityWelcomePage(Aki::Sql::Database* database, QWidget* parent = 0);
    ~IdentityWelcomePage();
    Aki::Sql::Identity* identity() const;
    void loadNewIdentity();
    void save();
private Q_SLOTS:
    void slotAwayMessagesClicked(bool clicked);
    void slotAwayMessageTextEdited(const QString& message);
    void slotMarkLastPositionClicked(bool clicked);
    void slotRealNameTextEdited(const QString& name);
    void slotReturnMessageTextEdited(const QString& message);
private:
    Aki::Sql::Identity* _identity;
    Aki::Sql::Database* _database;
    KLineEdit* _realName;
    KLineEdit* _awayMessage;
    KLineEdit* _returnMessage;
    Aki::NicknameWidget* _nicknameList;
    QGroupBox* _awayMessagesGroupBox;
    QStringList _nicknames;
}; // End of class IdentityWelcomePage.
} // End of namespace Aki.

#endif // AKI_IDENTITYWELCOMEPAGE_HPP
