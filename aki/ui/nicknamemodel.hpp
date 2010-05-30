#ifndef AKI_NICKNAMEMODEL_HPP
#define AKI_NICKNAMEMODEL_HPP

#include <QtCore/QAbstractItemModel>

namespace Aki
{
class SqlNickname;
class NicknameModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit NicknameModel(QObject* parent = 0);
    ~NicknameModel();
    void addNickname(Aki::SqlNickname* nickname);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void insertNickname(int row, Aki::SqlNickname* nickname);
    QList<Aki::SqlNickname*> nicknames() const;
    void removeNickname(Aki::SqlNickname* nickname);
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Aki::SqlNickname* takeNickname(int row);
private:
    QList<Aki::SqlNickname*> _nicknameList;
}; // End of class NicknameModel.
} // End of namespace Aki.

#endif // AKI_NICKNAMEMODEL_HPP
