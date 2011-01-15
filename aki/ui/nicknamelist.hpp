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

#ifndef AKI_NICKNAMELIST_HPP
#define AKI_NICKNAMELIST_HPP

#include <QtGui/QListView>

namespace Aki
{
class NicknameModel;
class SqlIdentity;
class SqlNickname;
class NicknameList : public QListView
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlNickname*> List;
    explicit NicknameList(QWidget* parent = 0);
    ~NicknameList();
    void addNickname(Aki::SqlNickname* nickname);
    int count() const;
    Aki::SqlNickname* currentNickname() const;
    int currentRow() const;
    Aki::NicknameList::List findNicknames(const QString& name, Qt::MatchFlags flags) const;
    void insertNickname(int row, Aki::SqlNickname* nickname);
    Aki::SqlNickname* nickname(int row) const;
    int row(Aki::SqlNickname* nickname) const;
    Aki::NicknameList::List selectedNicknames() const;
    void setCurrentNickname(Aki::SqlNickname* nickname);
    void setCurrentNickname(Aki::SqlNickname* nickname, QItemSelectionModel::SelectionFlags command);
    void setCurrentRow(int row);
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);
    Aki::SqlNickname* takeNickname(int row);
public Q_SLOTS:
    void repopulateNicknames(Aki::SqlIdentity* identity);
Q_SIGNALS:
    void currentNicknameChanged(Aki::SqlNickname* current, Aki::SqlNickname* previous);
    void nicknameActivated(Aki::SqlNickname* nickname);
    void nicknameChanged(Aki::SqlNickname* nickname);
    void nicknameClicked(Aki::SqlNickname* nickname);
    void nicknameCurrentRowChanged(int row);
    void nicknameDoubleClicked(Aki::SqlNickname* nickname);
    void nicknameEntered(Aki::SqlNickname* nickname);
    void nicknamePressed(Aki::SqlNickname* nickname);
    void nicknameSelectionChanged();
protected:
    QModelIndex indexFromNickname(Aki::SqlNickname* nickname);
    Aki::SqlNickname* nicknameFromIndex(const QModelIndex& index) const;
private Q_SLOTS:
    void slotItemActivated(const QModelIndex& index);
    void slotItemClicked(const QModelIndex& index);
    void slotItemChanged(const QModelIndex& index);
    void slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
    void slotItemDoubleClicked(const QModelIndex& index);
    void slotItemEntered(const QModelIndex& index);
    void slotItemPressed(const QModelIndex& index);
private:
    Aki::NicknameModel* _model;
}; // End of class NicknameList.
} // End of namespace Aki.

#endif // AKI_NICKNAMELIST_HPP
