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

#ifndef AKI_CHANNELLIST_HPP
#define AKI_CHANNELLIST_HPP

#include <QtGui/QListView>

namespace Aki
{
namespace Sql
{
class Channel;
class Database;
class Server;
} // End of namespace Sql.
class ChannelModel;
class ChannelList
    : public QListView
{
    Q_OBJECT
public:
    typedef QList<Aki::Sql::Channel*> List;
    explicit ChannelList(QWidget* parent = 0);
    ~ChannelList();
    void addChannel(Aki::Sql::Channel* channel);
    Aki::Sql::Channel* channel(int row) const;
    int count() const;
    Aki::Sql::Channel* currentChannel() const;
    int currentRow() const;
    Aki::ChannelList::List findChannels(const QString& name, Qt::MatchFlags flags) const;
    void insertChannel(int row, Aki::Sql::Channel* channel);
    void removeChannel(int row);
    int row(Aki::Sql::Channel* channel);
    Aki::ChannelList::List selectedAddresses() const;
    void setCurrentChannel(Aki::Sql::Channel* channel, QItemSelectionModel::SelectionFlags command = QItemSelectionModel::ClearAndSelect);
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command = QItemSelectionModel::ClearAndSelect);
    void setDatabase(Aki::Sql::Database* database);
    Aki::Sql::Channel* takeChannel(int row);
public Q_SLOTS:
    void repopulateChannels(Aki::Sql::Server* server);
Q_SIGNALS:
    void channelActivated(Aki::Sql::Channel* channel);
    void channelChanged(Aki::Sql::Channel* channel);
    void channelClicked(Aki::Sql::Channel* channel);
    void channelCurrentRowChanged(int row);
    void channelDoubleClicked(Aki::Sql::Channel* channel);
    void channelEntered(Aki::Sql::Channel* channel);
    void channelPressed(Aki::Sql::Channel* channel);
    void channelSelectionChanged();
    void currentChannelChanged(Aki::Sql::Channel* current, Aki::Sql::Channel* previous);
protected:
    Aki::Sql::Channel* channelFromIndex(const QModelIndex& index) const;
    QModelIndex indexFromChannel(Aki::Sql::Channel* channel);
private Q_SLOTS:
    void slotItemActivated(const QModelIndex& index);
    void slotItemChanged(const QModelIndex& index);
    void slotItemClicked(const QModelIndex& index);
    void slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
    void slotItemDoubleClicked(const QModelIndex& index);
    void slotItemEntered(const QModelIndex& index);
    void slotItemPressed(const QModelIndex& index);
private:
    Aki::ChannelModel* _model;
    Aki::Sql::Database* _database;
}; // End of class ChannelList.
} // End of namespace Aki.

#endif // AKI_CHANNELLIST_HPP
