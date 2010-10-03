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

#ifndef AKI_CHANNELMODEL_HPP
#define AKI_CHANNELMODEL_HPP

#include <QtCore/QAbstractItemModel>

namespace Aki
{
class SqlChannel;
class ChannelModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ChannelModel(QObject* parent = 0);
    ~ChannelModel();
    void addChannel(Aki::SqlChannel* channel);
    QList<Aki::SqlChannel*> channels() const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void insertChannel(int row, Aki::SqlChannel* channel);
    void removeChannel(Aki::SqlChannel* channel);
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Aki::SqlChannel* takeChannel(int row);
private:
    QList<Aki::SqlChannel*> _channelList;
}; // End of class ChannelModel.
} // End of namespace Aki.

#endif // AKI_CHANNELMODEL_HPP
