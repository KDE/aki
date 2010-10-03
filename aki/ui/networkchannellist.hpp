/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
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
class ChannelModel;
class SqlChannel;
class SqlNetwork;
class NetworkChannelList : public QListView
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlChannel*> List;
    explicit NetworkChannelList(QWidget* parent = 0);
    ~NetworkChannelList();
    void addNetworkChannel(Aki::SqlChannel* channel);
    int count() const;
    Aki::SqlChannel* currentNetworkChannel() const;
    int currentRow() const;
    Aki::NetworkChannelList::List findItems(const QString& name, Qt::MatchFlags flags) const;
    void insertNetworkChannel(int row, Aki::SqlChannel* channel);
    Aki::SqlChannel* networkChannel(int row) const;
    int row(Aki::SqlChannel* channel) const;
    Aki::NetworkChannelList::List selectedNetworkChannels() const;
    void setCurrentNetworkChannel(Aki::SqlChannel* channel, QItemSelectionModel::SelectionFlags command);
    void setCurrentNetworkChannel(Aki::SqlChannel* channel);
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);
    void setCurrentRow(int row);
    Aki::SqlChannel* takeNetworkChannel(int row);
public Q_SLOTS:
    void repopulateNetworkChannels(Aki::SqlNetwork* network);
Q_SIGNALS:
    void currentNetworkChannelChanged(Aki::SqlChannel* current, Aki::SqlChannel* previous);
    void networkChannelActivated(Aki::SqlChannel* channel);
    void networkChannelChanged(Aki::SqlChannel* channel);
    void networkChannelClicked(Aki::SqlChannel* channel);
    void networkChannelCurrentRowChanged(int row);
    void networkChannelDoubleClicked(Aki::SqlChannel* channel);
    void networkChannelEntered(Aki::SqlChannel* channel);
    void networkChannelPressed(Aki::SqlChannel* channel);
    void networkChannelSelectionChanged();
protected:
    QModelIndex indexFromNetworkChannel(Aki::SqlChannel* channel);
    Aki::SqlChannel* networkChannelFromIndex(const QModelIndex& index) const;
private:
    Aki::ChannelModel* _model;
}; // End of class NetworkChannelList.
} // End of namespace Aki.

#endif // AKI_NETWORKCHANNELLIST_HPP
