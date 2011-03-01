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

#ifndef AKI_SERVERWIDGET_HPP
#define AKI_SERVERWIDGET_HPP

#include <QtGui/QWidget>

class KPushButton;
namespace Aki
{
namespace Sql
{
class Database;
class Identity;
class Server;
} // End of namespace Sql.
class ServerList;
class ServerListSerializer;
class ServerWidget
    : public QWidget
{
    Q_OBJECT
public:
    explicit ServerWidget(QWidget* parent = 0);
    ~ServerWidget();
    void addServer(Aki::Sql::Server* server);
    int count() const;
    Aki::Sql::Server* currentServer() const;
    void insertServer(int row, Aki::Sql::Server* server);
    void removeServer(Aki::Sql::Server* server);
    int row(Aki::Sql::Server* server) const;
    void setCurrentServer(Aki::Sql::Server* server);
    void setCurrentRow(int row);
    void setDatabase(Aki::Sql::Database* database);
    Aki::Sql::Server* server(int index);
    Aki::Sql::Server* takeServer(int index);
public Q_SLOTS:
    void repopulateServers(Aki::Sql::Identity* identity);
Q_SIGNALS:
    void serverChanged(Aki::Sql::Server* server);
private Q_SLOTS:
    void slotAddClicked();
    void slotEditClicked();
    void slotExportClicked();
    void slotImportClicked();
    void slotServerCurrentRowChanged(int row);
    void slotServerListClicked(Aki::Sql::Server* server);
    void slotRemoveClicked();
private:
    Aki::ServerList* _serverList;
    Aki::ServerListSerializer* _serverListSerializer;
    KPushButton* _addButton;
    KPushButton* _removeButton;
    KPushButton* _editButton;
    KPushButton* _importButton;
    KPushButton* _exportButton;
}; // End of class ServerWidget.
} // End of namespace Aki.

#endif // AKI_SERVERWIDGET_HPP
