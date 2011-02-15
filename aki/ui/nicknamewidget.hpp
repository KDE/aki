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

#ifndef AKI_NICKNAMEWIDGET_HPP
#define AKI_NICKNAMEWIDGET_HPP

#include <QtGui/QWidget>

class KPushButton;
namespace Aki
{
namespace Sql
{
class Database;
class Identity;
class Nickname;
}
class NicknameList;
class NicknameWidget
    : public QWidget
{
    Q_OBJECT
public:
    explicit NicknameWidget(QWidget* parent = 0);
    ~NicknameWidget();
    void addNickname(Aki::Sql::Nickname* nickname);
    int count() const;
    Aki::Sql::Nickname* currentNickname() const;
    void insertNickname(int row, Aki::Sql::Nickname* nickname);
    Aki::Sql::Nickname* nickname(int index) const;
    void removeNickname(Aki::Sql::Nickname* nickname);
    void setCurrentAddress(Aki::Sql::Nickname* nickname);
    void setCurrentRow(int row);
    void setDatabase(Aki::Sql::Database* database);
    Aki::Sql::Nickname* takeNickname(int row);
public Q_SLOTS:
    void repopulateNicknames(Aki::Sql::Identity* identity);
private Q_SLOTS:
    void slotAddClicked();
    void slotEditClicked();
    void slotMoveDownClicked();
    void slotMoveUpClicked();
    void slotNicknameCurrentRowChanged(int row);
    void slotNicknameListClicked(Aki::Sql::Nickname* nickname);
    void slotRemoveClicked();
private:
    Aki::NicknameList* _nicknameList;
    KPushButton* _addButton;
    KPushButton* _removeButton;
    KPushButton* _editButton;
    KPushButton* _moveUpButton;
    KPushButton* _moveDownButton;
}; // End of class NicknameWidget.
} // End of namespace Aki.

#endif // AKI_NICKNAMEWIDGET_HPP
