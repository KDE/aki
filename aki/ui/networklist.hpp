#ifndef AKI_NETWORKLIST_HPP
#define AKI_NETWORKLIST_HPP

#include <QtGui/QListView>

namespace Aki
{
class NetworkListModel;
class SqlServer;
class NetworkList : public QListView
{
    Q_OBJECT
public:
    typedef QList<Aki::NetworkListModel*> List;
    NetworkList(QWidget* parent = 0);
    ~NetworkList();
    void addModel(Aki::NetworkListModel* model);
    void addNetwork(Aki::SqlServer* network);
    int count() const;
    int modelCount() const;
    Aki::SqlServer* currentNetwork() const;
    Aki::NetworkListModel* currentModel() const;
    int currentRow() const;
    int currentModelRow() const;
    QList<Aki::SqlServer*> findItems(const QString& name, Qt::MatchFlags flags) const;
    void insertModel(int row, Aki::NetworkListModel* model);
    void insertNetwork(int row, Aki::SqlServer* network);
    Aki::NetworkListModel* model(int row) const;
    Aki::SqlServer* network(int row) const;
    int row(const Aki::NetworkListModel* model) const;
    int row(const Aki::SqlServer* network) const;
    QList<Aki::SqlServer*> selectedItems() const;
    void setCurrentModel(Aki::NetworkListModel* model);
    void setCurrentModel(int row);
    void setCurrentNetwork(Aki::SqlServer* network);
    void setCurrentNetwork(Aki::SqlServer* network, QItemSelectionModel::SelectionFlags command);
    void setCurrentRow(int row);
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);
    Aki::NetworkListModel* takeModel(int row);
    Aki::SqlServer* takeNetwork(int row);
private:
    Aki::NetworkList::List _modelList;
    int _modelCurrentRow;
}; // End of class NetworkList.
} // End of namespace Aki.

#endif // AKI_NETWORKLIST_HPP
