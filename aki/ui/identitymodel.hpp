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

#ifndef AKI_IDENTITYMODEL_HPP
#define AKI_IDENTITYMODEL_HPP

#include <QtCore/QAbstractItemModel>

namespace Aki
{
namespace Sql
{
class Identity;
} // End of namespace Sql.
class IdentityModel
    : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit IdentityModel(QObject* parent = 0);
    ~IdentityModel();
    void addIdentity(Aki::Sql::Identity* identity);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void insertIdentity(int row, Aki::Sql::Identity* identity);
    QList<Aki::Sql::Identity*> identities() const;
    void removeIdentity(Aki::Sql::Identity* identity);
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Aki::Sql::Identity* takeIdentity(int row);
private:
    QList<Aki::Sql::Identity*> _identityList;
}; // End of class IdentityModel.
} // End of namespace Aki.

#endif // AKI_IDENTITYMODEL_HPP
