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

#ifndef AKI_SERVERLIST_HPP
#define AKI_SERVERLIST_HPP

#include <QtGui/QListView>

namespace Aki
{
namespace Sql
{
class Database;
class Identity;
class Server;
} // End of namespace Sql.
class ServerModel;
class ServerList
    : public QListView
{
    Q_OBJECT
public:
    typedef QList<Aki::Sql::Server*> List;
    explicit ServerList(QWidget* parent = 0);
    ~ServerList();
    void addServer(Aki::Sql::Server* server);
    int count() const;
    int currentRow() const;
    Aki::Sql::Server* currentServer() const;
    Aki::ServerList::List findItems(const QString& name, Qt::MatchFlags flags) const;
    void insertServer(int row, Aki::Sql::Server* server);
    void removeServer(int row);
    int row(Aki::Sql::Server* server) const;
    Aki::ServerList::List selectedServers() const;
    Aki::Sql::Server* server(int row) const;
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command = QItemSelectionModel::ClearAndSelect);
    void setCurrentServer(Aki::Sql::Server* server, QItemSelectionModel::SelectionFlags command = QItemSelectionModel::ClearAndSelect);
    void setDatabase(Aki::Sql::Database* database);
    Aki::Sql::Server* takeServer(int row);
public Q_SLOTS:
    void repopulateServers(Aki::Sql::Identity* identity);
Q_SIGNALS:
    void currentServerChanged(Aki::Sql::Server* current, Aki::Sql::Server* previous);
    void serverActivated(Aki::Sql::Server* server);
    void serverChanged(Aki::Sql::Server* server);
    void serverClicked(Aki::Sql::Server* server);
    void serverCurrentRowChanged(int row);
    void serverDoubleClicked(Aki::Sql::Server* server);
    void serverEntered(Aki::Sql::Server* server);
    void serverPressed(Aki::Sql::Server* server);
    void serverSelectionChanged();
protected:
    Aki::Sql::Server* serverFromIndex(const QModelIndex& index) const;
    QModelIndex indexFromServer(Aki::Sql::Server* server);
private Q_SLOTS:
    void slotItemActivated(const QModelIndex& index);
    void slotItemChanged(const QModelIndex& index);
    void slotItemClicked(const QModelIndex& index);
    void slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
    void slotItemDoubleClicked(const QModelIndex& index);
    void slotItemEntered(const QModelIndex& index);
    void slotItemPressed(const QModelIndex& index);
private:
    Aki::ServerModel* _model;
    Aki::Sql::Database* _database;
}; // End of class ServerList.
} // End of namespace Aki.

#endif // AKI_SERVERLIST_HPP
