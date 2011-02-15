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

#ifndef AKI_NETWORKCHANNELLIST_HPP
#define AKI_NETWORKCHANNELLIST_HPP

#include <QtGui/QListView>

namespace Aki
{
namespace Sql
{
class Channel;
class Server;
}
class ChannelModel;
class NetworkChannelList : public QListView
{
    Q_OBJECT
public:
    typedef QList<Aki::Sql::Channel*> List;
    explicit NetworkChannelList(QWidget* parent = 0);
    ~NetworkChannelList();
    void addNetworkChannel(Aki::Sql::Channel* channel);
    int count() const;
    Aki::Sql::Channel* currentNetworkChannel() const;
    int currentRow() const;
    Aki::NetworkChannelList::List findItems(const QString& name, Qt::MatchFlags flags) const;
    void insertNetworkChannel(int row, Aki::Sql::Channel* channel);
    Aki::Sql::Channel* networkChannel(int row) const;
    int row(Aki::Sql::Channel* channel) const;
    Aki::NetworkChannelList::List selectedNetworkChannels() const;
    void setCurrentNetworkChannel(Aki::Sql::Channel* channel, QItemSelectionModel::SelectionFlags command);
    void setCurrentNetworkChannel(Aki::Sql::Channel* channel);
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);
    void setCurrentRow(int row);
    Aki::Sql::Channel* takeNetworkChannel(int row);
public Q_SLOTS:
    void repopulateNetworkChannels(Aki::Sql::Server* network);
Q_SIGNALS:
    void currentNetworkChannelChanged(Aki::Sql::Channel* current, Aki::Sql::Channel* previous);
    void networkChannelActivated(Aki::Sql::Channel* channel);
    void networkChannelChanged(Aki::Sql::Channel* channel);
    void networkChannelClicked(Aki::Sql::Channel* channel);
    void networkChannelCurrentRowChanged(int row);
    void networkChannelDoubleClicked(Aki::Sql::Channel* channel);
    void networkChannelEntered(Aki::Sql::Channel* channel);
    void networkChannelPressed(Aki::Sql::Channel* channel);
    void networkChannelSelectionChanged();
protected:
    QModelIndex indexFromNetworkChannel(Aki::Sql::Channel* channel);
    Aki::Sql::Channel* networkChannelFromIndex(const QModelIndex& index) const;
private:
    Aki::ChannelModel* _model;
}; // End of class NetworkChannelList.
} // End of namespace Aki.

#endif // AKI_NETWORKCHANNELLIST_HPP
