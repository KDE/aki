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

#ifndef AKI_ADDRESSMODEL_HPP
#define AKI_ADDRESSMODEL_HPP

#include <QtCore/QAbstractItemModel>

namespace Aki
{
namespace Sql
{
class Address;
} // End of namespace Sql.
class AddressModel
    : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AddressModel(QObject* parent = 0);
    ~AddressModel();
    void addAddress(Aki::Sql::Address* address);
    const QList<Aki::Sql::Address*>& addresses() const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    void insertAddress(int row, Aki::Sql::Address* address);
    void removeAddress(Aki::Sql::Address* address);
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Aki::Sql::Address* takeAddress(int row);
private:
    QList<Aki::Sql::Address*> _addressList;
}; // End of class AddressModel.
} // End of namespace Aki.

#endif // AKI_ADDRESSMODEL_HPP
