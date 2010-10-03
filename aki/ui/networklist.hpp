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

#ifndef AKI_NETWORKLIST_HPP
#define AKI_NETWORKLIST_HPP

#include <QtGui/QListView>

namespace Aki
{
class NetworkModel;
class SqlIdentity;
class SqlNetwork;
/**
 * Network list for the Identity that is set.
 */
class NetworkList : public QListView
{
    Q_OBJECT
public:
    /**
     * typedef for QList<Aki::SqlServer*>
     */
    typedef QList<Aki::SqlNetwork*> List;
    /**
     * Creates a NetworkList object.
     *
     * @param parent Parent of the object.
     */
    explicit NetworkList(QWidget* parent = 0);
    /**
     * Destroys the object.
     */
    ~NetworkList();
    /**
     * Adds a new @p network to the list.
     *
     * @param network IRC Network.
     */
    void addNetwork(Aki::SqlNetwork* network);
    /**
     * Gets the current network count.
     *
     * @return Network count.
     */
    int count() const;
    /**
     * Gets the current Network.
     *
     * @return Current Network.
     *
     * @sa NetworkList::currentRow()
     */
    Aki::SqlNetwork* currentNetwork();
    /**
     * Gets the current Network row.
     *
     * @return Current row.
     *
     * @sa NetworkList::currentNetwork()
     */
    int currentRow() const;
    /**
     * Search for Networks the match the @p name and search @p flags.
     *
     * @param name Name to search for.
     * @param flags Search flags.
     *
     * @return List of Networks that was found, empty list if nothing was found.
     */
    Aki::NetworkList::List findNetworks(const QString& name, Qt::MatchFlags flags) const;
    /**
     * Inserts a @p network at the given @p row.
     *
     * @param row Row to be inserted at.
     * @param network Network to be inserted.
     */
    void insertNetwork(int row, Aki::SqlNetwork* network);
    /**
     * Get the Network at the given @p row.
     *
     * @param row Network row.
     *
     * @return Network for row, null pointer if an invalid row was specified.
     */
    Aki::SqlNetwork* item(int row) const;
    /**
     * Gets the row of the given @p network.
     *
     * @param network Network.
     *
     * @return Row of the given network. -1 if not found.
     */
    int row(Aki::SqlNetwork* network) const;
    /**
     * Gets the list of all selected networks.
     *
     * @return Network list of the current selected networks. Empty list of nothing is selected.
     */
    Aki::NetworkList::List selectedNetworks() const;
    /**
     * Sets the current @p network.
     *
     * @param network Network.
     */
    void setCurrentNetwork(Aki::SqlNetwork* network);
    /**
     * Sets the current @p network with @p command flags.
     *
     * @param network Network.
     * @param command Selection flags.
     */
    void setCurrentNetwork(Aki::SqlNetwork* network, QItemSelectionModel::SelectionFlags command);
    /**
     * Sets the current @p row.
     *
     * @param row Network row.
     */
    void setCurrentRow(int row);
    /**
     * Sets the current @p row with @p command flags.
     *
     * @param row Network row.
     * @param command Selection flags.
     */
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);
    /**
     * Removes the current Network at the given @p row.
     * @note You are responsible for deletion of this Network.
     *
     * @param row Network row.
     *
     * @return Network at the given row, null pointer if row is invalid.
     */
    Aki::SqlNetwork* takeNetwork(int row);
public Q_SLOTS:
    /**
     * Repopulates the NetworkList with the servers for the given @p identity.
     * It deletes all the Networks current in the list then queries the identity
     * for the servers it holds and populates the list.
     *
     * @param identity Network Identity.
     */
    void repopulateNetwork(Aki::SqlIdentity* identity);
Q_SIGNALS:
    /**
     * This signal is emitted when the network has changed.
     *
     * @param current Current Network selected.
     * @param previous Previous Network selected.
     */
    void currentNetworkChanged(Aki::SqlNetwork* current, Aki::SqlNetwork* previous);
    /**
     * This signal is emitted when the @p network is activated. The @p network is activated
     * when the user clickes or double clicks on it, depending on the system configuration.
     * It is also activated when the user presses the activation key (on Windows and X11 this is
     * the Return key, on Mac OS X it is Ctrl+0).
     *
     * @param network Network that was activated.
     */
    void networkActivated(Aki::SqlNetwork* network);
    /**
     * This signal is emitted whenever the data of @p network has changed.
     *
     * @param network Network that was changed.
     */
    void networkChanged(Aki::SqlNetwork* network);
    /**
     * This signal is emitted with the specified @p network when a mouse button
     * is clicked on an item in the widget.
     *
     * @param network Network that was clicked on.
     */
    void networkClicked(Aki::SqlNetwork* network);
    /**
     * This signal is emitted whenever the current item changes.
     *
     * @param row Row of the current item. If there is no current item, the @p row is -1.
     */
    void networkCurrentRowChanged(int row);
    /**
     * This signal is emitted with the specified @p network when a mouse button is double clicked
     * on an item in the widget.
     *
     * @param network Network that was double clicked on.
     */
    void networkDoubleClicked(Aki::SqlNetwork* network);
    /**
     * This signal is emitted when the mouse cursor enters an item. The @p network is the item entered.
     * This signal is only emitted when mouseTracking is turned on, or when a mouse button is pressed while
     * moving in to an item.
     *
     * @param network Network that was entered.
     */
    void networkEntered(Aki::SqlNetwork* network);
    /**
     * This signal is emitted with the specified @p network when a mouse button is pressed on an network
     * in the widget.
     *
     * @param network Network that was pressed with the mouse.
     */
    void networkPressed(Aki::SqlNetwork* network);
    /**
     * Yhis signal is emitted whenever the selection changes.
     */
    void networkSelectionChanged();
protected:
    QModelIndex indexFromNetwork(Aki::SqlNetwork* network);
    Aki::SqlNetwork* networkFromIndex(const QModelIndex& index) const;
private Q_SLOTS:
    void slotItemActivated(const QModelIndex& index);
    void slotItemClicked(const QModelIndex& index);
    void slotItemChanged(const QModelIndex& index);
    void slotItemDoubleClicked(const QModelIndex& index);
    void slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
    void slotItemEntered(const QModelIndex& index);
    void slotItemPressed(const QModelIndex& index);
private:
    Aki::NetworkModel* _model;
}; // End of class NetworkList.
} // End of namespace Aki.

#endif // AKI_NETWORKLIST_HPP
