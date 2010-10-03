/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
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

#include "nicknamelist.hpp"
#include "aki.hpp"
#include "ui/nicknamemodel.hpp"
#include "utils/sqlidentity.hpp"
#include "utils/sqlnickname.hpp"
using namespace Aki;

NicknameList::NicknameList(QWidget* parent)
    : QListView(parent)
{
    _model = new Aki::NicknameModel(this);
    setModel(_model);

    connect(this, SIGNAL(pressed(QModelIndex)),
            SLOT(slotItemPressed(QModelIndex)));
    connect(this, SIGNAL(clicked(QModelIndex)),
            SLOT(slotItemClicked(QModelIndex)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)),
            SLOT(slotItemDoubleClicked(QModelIndex)));
    connect(this, SIGNAL(activated(QModelIndex)),
            SLOT(slotItemActivated(QModelIndex)));
    connect(this, SIGNAL(entered(QModelIndex)),
            SLOT(slotItemEntered(QModelIndex)));
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(slotItemChanged(QModelIndex)));
    connect(selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            SLOT(slotItemCurrentChanged(QModelIndex,QModelIndex)));
    connect(selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SIGNAL(nicknameSelectionChanged()));
}

NicknameList::~NicknameList()
{
}

void
NicknameList::addNickname(Aki::SqlNickname* nickname)
{
    _model->addNickname(nickname);
}

int
NicknameList::count() const
{
    return _model->rowCount();
}

Aki::SqlNickname*
NicknameList::currentNickname() const
{
    QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return 0;
    }

    return nicknameFromIndex(index);
}

int
NicknameList::currentRow() const
{
    QModelIndex index = selectionModel()->currentIndex();
    if (!index.isValid()) {
        return -1;
    }

    return index.row();
}

Aki::NicknameList::List
NicknameList::findNicknames(const QString& name, Qt::MatchFlags flags) const
{
    Aki::NicknameList::List list;

    QModelIndexList indexes = _model->match(_model->index(0), Qt::DisplayRole, name, -1, flags);
    for (int i = 0; i < indexes.size(); ++i) {
        list.append(nicknameFromIndex(indexes.at(i)));
    }

    return list;
}

QModelIndex
NicknameList::indexFromNickname(Aki::SqlNickname* nickname)
{
    return _model->index(_model->nicknames().indexOf(nickname));
}

void
NicknameList::insertNickname(int row, Aki::SqlNickname* nickname)
{
    Q_ASSERT(nickname);

    if (_model->nicknames().contains(nickname)) {
        return;
    }

    _model->insertNickname(row, nickname);
}

Aki::SqlNickname*
NicknameList::nickname(int row) const
{
    return nicknameFromIndex(_model->index(row));
}

Aki::SqlNickname*
NicknameList::nicknameFromIndex(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return _model->nicknames().value(index.row());
}

void
NicknameList::repopulateNicknames(Aki::SqlIdentity* identity)
{
    foreach (Aki::SqlNickname* nickname, _model->nicknames()) {
        qxtLog->info() << QString("Removing Nickname: %1").arg(nickname->nickname());
        _model->removeNickname(nickname);
    }

    if (!identity) {
        return;
    }

    Aki::SqlNickname::List list = Aki::SqlNickname::nicknamesForIdentity(identity);
    if (list.isEmpty()) {
        qxtLog->info() << QString("List is empty for Identity: %1").arg(identity->name());
        return;
    }

    foreach (Aki::SqlNickname* nickname, list) {
        qxtLog->info() << QString("Adding new Nickname: %1").arg(nickname->nickname());
        addNickname(nickname);
    }
}

int
NicknameList::row(Aki::SqlNickname* nickname) const
{
    return _model->nicknames().indexOf(nickname);
}

Aki::NicknameList::List
NicknameList::selectedNicknames() const
{
    Aki::NicknameList::List list;

    QModelIndexList indexes = selectionModel()->selectedRows();
    foreach (const QModelIndex& index, indexes) {
        list.append(nicknameFromIndex(index));
    }

    return list;
}

void
NicknameList::setCurrentNickname(Aki::SqlNickname* nickname)
{
    setCurrentNickname(nickname, QItemSelectionModel::ClearAndSelect);
}

void
NicknameList::setCurrentNickname(Aki::SqlNickname* nickname, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(indexFromNickname(nickname), command);
}

void
NicknameList::setCurrentRow(int row)
{
    setCurrentRow(row, QItemSelectionModel::ClearAndSelect);
}

void
NicknameList::setCurrentRow(int row, QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->setCurrentIndex(_model->index(row), command);
}

void
NicknameList::slotItemActivated(const QModelIndex& index)
{
    emit nicknameActivated(nicknameFromIndex(index));
}

void
NicknameList::slotItemChanged(const QModelIndex& index)
{
    emit nicknameChanged(nicknameFromIndex(index));
}

void
NicknameList::slotItemClicked(const QModelIndex& index)
{
    emit nicknameClicked(nicknameFromIndex(index));
}

void
NicknameList::slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    QPersistentModelIndex persistentCurrent = current;
    Aki::SqlNickname* currentNickname = nickname(persistentCurrent.row());
    emit currentNicknameChanged(currentNickname, nickname(previous.row()));
    emit nicknameCurrentRowChanged(persistentCurrent.row());
}

void
NicknameList::slotItemDoubleClicked(const QModelIndex& index)
{
    emit nicknameDoubleClicked(nicknameFromIndex(index));
}

void
NicknameList::slotItemEntered(const QModelIndex& index)
{
    emit nicknameEntered(nicknameFromIndex(index));
}

void
NicknameList::slotItemPressed(const QModelIndex& index)
{
    emit nicknamePressed(nicknameFromIndex(index));
}

Aki::SqlNickname*
NicknameList::takeNickname(int row)
{
    return _model->takeNickname(row);
}
