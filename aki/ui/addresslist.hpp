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


#ifndef AKI_ADDRESSLIST_HPP
#define AKI_ADDRESSLIST_HPP

#include <QtGui/QListView>

namespace Aki
{
namespace Sql
{
class Address;
class Database;
class Server;
} // End of namespace Sql.
class AddressModel;
class AddressList
    : public QListView
{
    Q_OBJECT
public:
    typedef QList<Aki::Sql::Address*> List;
    explicit AddressList(QWidget* parent = 0);
    ~AddressList();
    void addAddress(Aki::Sql::Address* address);
    Aki::Sql::Address* address(int row) const;
    int count() const;
    Aki::Sql::Address* currentAddress() const;
    int currentRow() const;
    Aki::AddressList::List findItems(const QString& name, Qt::MatchFlags flags) const;
    void insertAddress(int row, Aki::Sql::Address* address);
    void removeAddress(int row);
    int row(Aki::Sql::Address* address) const;
    Aki::AddressList::List selectedAddresses() const;
    void setCurrentAddress(Aki::Sql::Address* address, QItemSelectionModel::SelectionFlags command);
    void setCurrentAddress(Aki::Sql::Address* address);
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);
    void setCurrentRow(int row);
    void setDatabase(Aki::Sql::Database* database);
    Aki::Sql::Address* takeAddress(int row);
public Q_SLOTS:
    void repopulateAddresses(Aki::Sql::Server* server);
Q_SIGNALS:
    void addressActivated(Aki::Sql::Address* address);
    void addressChanged(Aki::Sql::Address* address);
    void addressClicked(Aki::Sql::Address* address);
    void addressCurrentRowChanged(int row);
    void addressDoubleClicked(Aki::Sql::Address* address);
    void addressEntered(Aki::Sql::Address* address);
    void addressPressed(Aki::Sql::Address* address);
    void addressSelectionChanged();
    void currentAddressChanged(Aki::Sql::Address* current, Aki::Sql::Address* previous);
protected:
    Aki::Sql::Address* addressFromIndex(const QModelIndex& index) const;
    QModelIndex indexFromAddress(Aki::Sql::Address* address);
private Q_SLOTS:
    void slotItemActivated(const QModelIndex& index);
    void slotItemChanged(const QModelIndex& index);
    void slotItemClicked(const QModelIndex& index);
    void slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
    void slotItemDoubleClicked(const QModelIndex& index);
    void slotItemEntered(const QModelIndex& index);
    void slotItemPressed(const QModelIndex& index);
private:
    Aki::AddressModel* _model;
    Aki::Sql::Database* _database;
}; // End of class AddressList.
} // End of namespace Aki.

#endif // AKI_ADDRESSLIST_HPP
