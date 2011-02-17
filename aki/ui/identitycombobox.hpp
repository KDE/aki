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

#ifndef AKI_IDENTITYCOMBOBOX_HPP
#define AKI_IDENTITYCOMBOBOX_HPP

#include <KDE/KComboBox>

namespace Aki
{
namespace Sql
{
class Database;
class Identity;
} // End of namespace Sql.
class IdentityModel;
class IdentityComboBox
    : public KComboBox
{
    Q_OBJECT
public:
    typedef QList<Aki::Sql::Identity*> List;
    explicit IdentityComboBox(QWidget* parent = 0);
    ~IdentityComboBox();
    void addIdentity(Aki::Sql::Identity* identity);
    Aki::Sql::Identity* currentIdentity() const;
    Aki::IdentityComboBox::List findIdentities(const QString& name, Qt::MatchFlags flags);
    void insertIdentity(int row, Aki::Sql::Identity* identity);
    Aki::Sql::Identity* identity(int index) const;
    void repopulateIdentities();
    int row(Aki::Sql::Identity* identity) const;
    void setCurrentIdentity(Aki::Sql::Identity* identity);
    void setDatabase(Aki::Sql::Database* database);
    Aki::Sql::Identity* takeIdentity(int index);
Q_SIGNALS:
    void identityActivated(Aki::Sql::Identity* identity);
    void currentIndexChanged(Aki::Sql::Identity* identity);
protected:
    QModelIndex indexFromIdentity(Aki::Sql::Identity* identity) const;
    Aki::Sql::Identity* identityFromIndex(const QModelIndex& index) const;
private Q_SLOTS:
    void slotActivated(int index);
    void slotCurrentIndexChanged(int index);
private:
    Aki::Sql::Database* _database;
    Aki::IdentityModel* _model;
}; // End of class IdentityComboBox.
} // End of namespace Aki.

#endif // AKI_IDENTITYCOMBOBOX_HPP
