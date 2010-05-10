#ifndef AKI_NETWORKLIST_HPP
#define AKI_NETWORKLIST_HPP

#include <QtGui/QListView>

namespace Aki
{
class NetworkListModel;
class SqlIdentity;
class SqlServer;
/**
 * Creates a network list that managed a map of identities (Aki::SqlIdentity) with their list
 * of networks. An identity (Aki::SqlIdentity) is a name that represents a user in the application.
 * Aki can have multiple identities (Aki::SqlIdentity) that will holds multiple networks (Aki::SqlServer)
 * in it. A network (Aki::SqlServer) represents exactly 1 IRC server that they will connect to.
 *
 * @code
 * +------------+
 * |  Identity  |------
 * +------------+     |
 *       |            |
 *       |       +---------+
 *  +---------+  | Network |
 *  | Network |  +---------+
 *  +---------+
 * @endcode
 */
class NetworkList : public QListView
{
    Q_OBJECT
public:
    /**
     * Typedef of QList<Aki::NetworkListModel*>.
     */
    typedef QList<Aki::NetworkListModel*> List;
    /**
     * Typedef of QMap<QString,Aki::NetworkList::List>.
     */
    typedef QMap<QString,Aki::NetworkList::List> Map;
    /**
     * Creates a new NetworkList object.
     *
     * @param parent Parent of the object.
     */
    NetworkList(QWidget* parent = 0);
    /**
     * Destroys the object.
     */
    ~NetworkList();
    /**
     * Adds an identity at the given @p row with the given @p identity name.
     * If the @p identity is the first one to added it will set it as the current one and sets
     * the current model to the first slot.
     *
     * @param row Row to be inserted at.
     * @param identity Identity name to be given.
     */
    void addIdentity(const QString& identity);

    void addModel(Aki::NetworkListModel* model);
    /**
     * Addes a new @p network to the current identity and current model. If the network
     * list when this @p network is added is 0. It will set it as the current network. Else
     * it will just append and keep the existing network as the current.
     *
     * @param network IRC Network.
     */
    void addNetwork(Aki::SqlServer* network);
    /**
     * Gets the current count of networks for the current identity and current model.
     *
     * @return Current network count.
     */
    int count() const;
    /**
     * Gets the current count of models the current identity contains.
     *
     * @return Current model count.
     */
    int modelCount() const;
    /**
     * Gets the current network for the current identity and current model.
     * @note You should not delete the pointer this returns to delete the network
     * Please use takeNetwork for deletion.
     *
     * @return Current network.
     */
    Aki::SqlServer* currentNetwork();
    Aki::SqlServer* currentNetwork() const;
    /**
     * Gets the current model for the current identity.
     * @note You should not delete the pointer this returns to delete the model
     * Please use takeModel for deletion.
     *
     * @return Current network.
     */
    Aki::NetworkListModel* currentModel();
    Aki::NetworkListModel* currentModel() const;
    /**
     * Gets the current identity.
     * @note You should not delete the pointer this returns to delete the identity
     * Please use takeIdentity.
     *
     * @return Current identity.
     */
    Aki::NetworkList::List& currentIdentity();
    const Aki::NetworkList::List currentIdentity() const;
    /**
     * Gets the current network row of the current identity and the current model.
     *
     * @return Current row.
     */
    int currentRow() const;
    /**
     * Gets the current model row of the current identity.
     *
     * @return Current row.
     */
    int currentModelRow() const;
    /**
     * Finds networks matching the @p name with the @p flags.
     *
     * @param name Name to search for.
     * @param flags Search flags.
     * 
     * @return Networks matching the @p flags.
     */
    QList<Aki::SqlServer*> findItems(const QString& name, Qt::MatchFlags flags) const;
    /**
     * Inserts an model in the current identity at the given @p row with the
     * given @p model. If this @p model is the first one it will set it as the default.
     *
     * @param row Row to be inserted at.
     * @param model Network model.
     */
    void insertModel(int row, Aki::NetworkListModel* model);
    /**
     * Inserts an network in the current identity and the current model at the given
     * @p row with the given @p network. If this @p network is the first one, it will
     * set it as the default.
     *
     * @param row Row to be inserted at.
     * @param network IRC network.
     */
    void insertNetwork(int row, Aki::SqlServer* network);
    /**
     * Gets the model at the given @p row.
     *
     * @param row Row to get the network model at.
     *
     * @return The network model. If @p row is invalid it will return 0.
     */
    Aki::NetworkListModel* modelAt(int row) const;
    /**
     * Gets the network of the current identity and current model at the given @p row.
     *
     * @param row Row to get the network at.
     *
     * @return The network. If @p row is invalid it will return 0.
     */
    Aki::SqlServer* network(int row) const;
    /**
     * Gets the row number of the given @p model in the current identity.
     *
     * @param model Model.
     *
     * @return Row of the given model. 
     */
    int row(const Aki::NetworkListModel* model) const;
    /**
     * Gets the row number of the given @p network in the current identity and current model.
     *
     * @param network IRC network.
     *
     * @return Row of the given network.
     */
    int row(const Aki::SqlServer* network) const;
    /**
     * Gets the selected networks of the current identity and current model.
     *
     * @return List of networks that are current selected. If there is none,
     * it will return an empty list.
     */
    QList<Aki::SqlServer*> selectedItems() const;
    /**
     * Sets the current @p identity. Since the identity is the main one.
     * It will set the network model to the first one in the list.
     *
     * @param identity Name of the identity.
     */
    void setCurrentIdentity(const QString& identity);
    /**
     * Sets the current @p model. It will reset the current network to the first one.
     *
     * @param model Model.
     */
    void setCurrentModel(Aki::NetworkListModel* model);
    /**
     * Sets the current model to the given @p row.
     *
     * @param row Row to set as the current.
     */
    void setCurrentModel(int row);
    /**
     * Sets the current @p network.
     *
     * @param network IRC network.
     */
    void setCurrentNetwork(Aki::SqlServer* network);
    /**
     * Sets the current @p network with selection @p command flags.
     *
     * @param network IRC network.
     * @param command Selection command flags.
     */
    void setCurrentNetwork(Aki::SqlServer* network, QItemSelectionModel::SelectionFlags command);
    /**
     * Sets the current @p row for the network.
     *
     * @param row Row of the IRC network.
     */
    void setCurrentRow(int row);
    /**
     * Sets the current @p row for the network with selection @p command flags.
     *
     * @param row Row of the IRC network.
     * @param command Selection command flags.
     */
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);
    /**
     * Takes the network  from the current identity with the given @p row.
     *
     * @param row Row to remove from the current identity.
     *
     * @return Network mode that was taken from the identity.
     */
    Aki::NetworkListModel* takeModel(int row);
    /**
     * Takes the network from the current identity and the current model with the given
     * @p row.
     *
     * @param row Row to remove from the current identity and current network.
     *
     * @return Network that was taken from the model.
     */
    Aki::SqlServer* takeNetwork(int row);
Q_SIGNALS:
    void modelChanged(Aki::NetworkListModel* model);
public Q_SLOTS:
    void identityActivated(Aki::SqlIdentity* identity);
private:
    Aki::NetworkList::Map _modelList;
    QString _currentIdentityModel;
    int _modelCurrentRow;
}; // End of class NetworkList.
} // End of namespace Aki.

#endif // AKI_NETWORKLIST_HPP
