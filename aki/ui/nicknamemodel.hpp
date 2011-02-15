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

#ifndef AKI_NICKNAMEMODEL_HPP
#define AKI_NICKNAMEMODEL_HPP

#include <QtCore/QAbstractItemModel>

namespace Aki
{
namespace Sql
{
class Nickname;
} // End of namespace Sql.
/**
 * NicknameModel is a class that stores a list of Aki::Sql::Nickname for a particular view.
 */
class NicknameModel
    : public QAbstractListModel
{
    Q_OBJECT
public:
    /**
     * Creates a new NicknameModel with a @p parent.
     *
     * @param parent Parent of the object.
     */
    explicit NicknameModel(QObject* parent = 0);
    /**
     * Destroys the object.
     */
    ~NicknameModel();
    /**
     * Adds an @p nickname to the end of the list.
     *
     * @param nickname Identity nickname.
     */
    void addNickname(Aki::Sql::Nickname* nickname);
    /**
     * Gets the data stored under the given @p role for the item referred to by the @p index.
     * @note If you do not have a value to return, return an invalid QVariant instead of returning 0.
     *
     * @param index Index of the item to get the value of.
     * @param role Model role to get the value of the item at the @p index.
     *
     * @return Data for the given @p role of the item at @p index. QVariant() if no value is to be returned.
     */
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    /**
     * Gets the item flags for the given @p index.
     *
     * @param index Item to receive the flags for.
     *
     * @return Flags for the item at the given @p index.
     */
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    /**
     * Inserts the @p nickname at the given @p row.
     * @note If the @p row is below 0 it will be pushed at the beginning.
     * @note If the @p row is more than the number of items in the model it will be pushed at the end.
     *
     * @param row Row to insert the @p address at.
     * @param nickname Identity nickname.
     */
    void insertNickname(int row, Aki::Sql::Nickname* nickname);
    /**
     * Gets a list of nicknames this model currently contains.
     *
     * @return List of identity nicknames in this model; empty list if no nicknames are found.
     */
    const QList<Aki::Sql::Nickname*>& nicknames() const;
    /**
     * Gets the number of items in the model.
     *
     * @param parent Parent is ignored.
     *
     * @return Number of items in the model.
     */
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    /**
     * Gets the item at the given @p row. This will remove the item from the model
     * but it will not delete it. You are responsible for deletion of the item if
     * the return value is not 0.
     *
     * @param row Row index.
     *
     * @return Identity nickname at the given @p row; 0 if the @p row is invalid.
     */
    Aki::Sql::Nickname* takeNickname(int row);
private:
    QList<Aki::Sql::Nickname*> _nicknameList;
}; // End of class NicknameModel.
} // End of namespace Aki.

#endif // AKI_NICKNAMEMODEL_HPP
