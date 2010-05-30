#ifndef AKI_NICKNAMELIST_HPP
#define AKI_NICKNAMELIST_HPP

#include <QtGui/QListView>

namespace Aki
{
class NicknameModel;
class SqlIdentity;
class SqlNickname;
class NicknameList : public QListView
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlNickname*> List;
    explicit NicknameList(QWidget* parent = 0);
    ~NicknameList();
    void addNickname(Aki::SqlNickname* nickname);
    int count() const;
    Aki::SqlNickname* currentNickname() const;
    int currentRow() const;
    Aki::NicknameList::List findNicknames(const QString& name, Qt::MatchFlags flags) const;
    void insertNickname(int row, Aki::SqlNickname* nickname);
    Aki::SqlNickname* nickname(int row) const;
    int row(Aki::SqlNickname* nickname) const;
    Aki::NicknameList::List selectedNicknames() const;
    void setCurrentNickname(Aki::SqlNickname* nickname);
    void setCurrentNickname(Aki::SqlNickname* nickname, QItemSelectionModel::SelectionFlags command);
    void setCurrentRow(int row);
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);
    Aki::SqlNickname* takeNickname(int row);
public Q_SLOTS:
    void repopulateNicknames(Aki::SqlIdentity* identity);
Q_SIGNALS:
    void currentNicknameChanged(Aki::SqlNickname* current, Aki::SqlNickname* previous);
    void nicknameActivated(Aki::SqlNickname* nickname);
    void nicknameChanged(Aki::SqlNickname* nickname);
    void nicknameClicked(Aki::SqlNickname* nickname);
    void nicknameCurrentRowChanged(int row);
    void nicknameDoubleClicked(Aki::SqlNickname* nickname);
    void nicknameEntered(Aki::SqlNickname* nickname);
    void nicknamePressed(Aki::SqlNickname* nickname);
    void nicknameSelectionChanged();
protected:
    QModelIndex indexFromNickname(Aki::SqlNickname* nickname);
    Aki::SqlNickname* nicknameFromIndex(const QModelIndex& index) const;
private Q_SLOTS:
    void slotItemActivated(const QModelIndex& index);
    void slotItemClicked(const QModelIndex& index);
    void slotItemChanged(const QModelIndex& index);
    void slotItemCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
    void slotItemDoubleClicked(const QModelIndex& index);
    void slotItemEntered(const QModelIndex& index);
    void slotItemPressed(const QModelIndex& index);
private:
    Aki::NicknameModel* _model;
}; // End of class NicknameList.
} // End of namespace Aki.

#endif // AKI_NICKNAMELIST_HPP
