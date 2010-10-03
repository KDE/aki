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

#ifndef AKI_NETWORKMODEL_HPP
#define AKI_NETWORKMODEL_HPP

#include <QtCore/QAbstractItemModel>

namespace Aki
{
class SqlNetwork;
/**
 * NetworkModel holds all the Networks for a specific Identity.
 */
class NetworkModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /**
     * Create a NetworkModel object.
     *
     * @param parent Parent of the object.
     */
    explicit NetworkModel(QObject* parent = 0);
    /**
     * Destroys the object.
     */
    ~NetworkModel();
    /**
     * Adds a @p network to the model.
     *
     * @param network Network.
     */
    void addNetwork(Aki::SqlNetwork* network);
    /**
     * Gets information about an item in the model.
     *
     * @param index Model index of the item.
     * @param role Role to get information of the item's attributes.
     *
     * @return QVariant data of the information received. null QVariant if index is invalid.
     */
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    /**
     * Inserts a new @p network at the given @p row.
     *
     * @param row Row to be inserted at.
     * @param network Network.
     */
    void insertNetwork(int row, Aki::SqlNetwork* network);
    /**
     * Gets the list of Networks in the model.
     *
     * @return List of networks in the model. Empty list otherwise.
     */
    QList<Aki::SqlNetwork*> networks() const;
    /**
     * Removes the @p network from the model.
     *
     * @param network Network to be removed.
     */
    void removeNetwork(Aki::SqlNetwork* network);
    /**
     * Gets a count of how many Networks are in the model.
     *
     * @param parent Not used.
     *
     * @return Number of Networks in the model.
     */
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    /**
     * Takes the Network from the model.
     * @note You are responsible for deletion of the Network afterwards.
     *
     * @param row Row to the Network that will be removes.
     *
     * @return Network that is removed, null pointer if invalid row was specified.
     */
    Aki::SqlNetwork* takeNetwork(int row);
private:
    QList<Aki::SqlNetwork*> _networkList;
}; // End of class NetworkModel.
} // End of namespace Aki.

#endif // AKI_NETWORKMODEL_HPP
