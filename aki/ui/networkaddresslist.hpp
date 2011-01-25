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

#ifndef AKI_NETWORKADDRESSLIST_HPP
#define AKI_NETWORKADDRESSLIST_HPP

#include <QtGui/QListView>

namespace Aki
{
namespace Sql
{
class Address;
class Server;
} // End of namespace Sql.
class AddressModel;
class NetworkAddressList
    : public QListView
{
    Q_OBJECT
public:
    typedef QList<Aki::Sql::Address*> List;
    explicit NetworkAddressList(QWidget* parent = 0);
    ~NetworkAddressList();
    void addNetworkAddress(Aki::Sql::Address* address);
    int count() const;
    Aki::Sql::Address* currentNetworkAddress() const;
    int currentRow() const;
    Aki::NetworkAddressList::List findItems(const QString& name, Qt::MatchFlags flags) const;
    void insertNetworkAddress(int row, Aki::Sql::Address* network);
    Aki::Sql::Address* networkAddress(int row) const;
    int row(Aki::Sql::Address* address) const;
    Aki::NetworkAddressList::List selectedNetworkAddresses() const;
    void setCurrentNetworkAddress(Aki::Sql::Address* address, QItemSelectionModel::SelectionFlags command);
    void setCurrentNetworkAddress(Aki::Sql::Address* address);
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);
    void setCurrentRow(int row);
    Aki::Sql::Address* takeNetworkAddress(int row);
public Q_SLOTS:
    void repopulateNetworkAddresses(Aki::Sql::Server* network);
Q_SIGNALS:
    void currentNetworkAddressChanged(Aki::Sql::Address* current, Aki::Sql::Address* previous);
    void networkAddressActivated(Aki::Sql::Address* address);
    void networkAddressChanged(Aki::Sql::Address* address);
    void networkAddressClicked(Aki::Sql::Address* address);
    void networkAddressCurrentRowChanged(int row);
    void networkAddressDoubleClicked(Aki::Sql::Address* address);
    void networkAddressEntered(Aki::Sql::Address* address);
    void networkAddressPressed(Aki::Sql::Address* address);
    void networkAddressSelectionChanged();
private Q_SLOTS:
    void slotItemActivated(const QModelIndex& index);
    void slotItemChanged(const QModelIndex& index);
    void slotItemClicked(const QModelIndex& index);
    void slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
    void slotItemDoubleClicked(const QModelIndex& index);
    void slotItemEntered(const QModelIndex& index);
    void slotItemPressed(const QModelIndex& index);
protected:
    Aki::Sql::Address* networkAddressFromIndex(const QModelIndex& index) const;
    QModelIndex indexFromNetworkAddress(Aki::Sql::Address* address);
private:
    Aki::AddressModel* _model;
}; // End of class NetworkAddressList.
} // End of namespace Aki.

#endif // AKI_NETWORKADDRESSLIST_HPP
