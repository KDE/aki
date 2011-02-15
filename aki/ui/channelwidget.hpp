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

#ifndef AKI_CHANNELWIDGET_HPP
#define AKI_CHANNELWIDGET_HPP

#include <QtGui/QWidget>

class KPushButton;
namespace Aki
{
namespace Sql
{
class Channel;
class Database;
class Server;
} // End of namespace Sql.
class ChannelList;
class ChannelWidget
    : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelWidget(QWidget* parent = 0);
    ~ChannelWidget();
    void addChannel(Aki::Sql::Channel* channel);
    Aki::Sql::Channel* channel(int index);
    int count() const;
    Aki::Sql::Channel* currentChannel() const;
    void insertChannel(int row, Aki::Sql::Channel* channel);
    void removeChannel(Aki::Sql::Channel* channel);
    void setCurrentChannel(Aki::Sql::Channel* channel);
    void setCurrentRow(int row);
    void setDatabase(Aki::Sql::Database* database);
    Aki::Sql::Channel* takeChannel(int row);
public Q_SLOTS:
    void repopulateChannels(Aki::Sql::Server* server);
private Q_SLOTS:
    void slotAddClicked();
    void slotChannelCurrentRowChanged(int row);
    void slotChannelListClicked(Aki::Sql::Channel* channel);
    void slotEditClicked();
    void slotMoveDownClicked();
    void slotMoveUpClicked();
    void slotRemoveClicked();
private:
    Aki::ChannelList* _channelList;
    KPushButton* _addButton;
    KPushButton* _removeButton;
    KPushButton* _editButton;
    KPushButton* _moveUpButton;
    KPushButton* _moveDownButton;
}; // End of class ChannelWidget.
} // End of namespace Aki.

#endif // AKI_CHANNELWIDGET_HPP
