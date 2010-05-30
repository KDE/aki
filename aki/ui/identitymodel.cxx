#include "identitymodel.hpp"
#include "aki.hpp"
#include "utils/sqlidentity.hpp"
using namespace Aki;

IdentityModel::IdentityModel(QObject* parent)
    : QAbstractListModel(parent)
{
    _identityList.clear();
}

IdentityModel::~IdentityModel()
{
    qDeleteAll(_identityList);
}

void
IdentityModel::addIdentity(Aki::SqlIdentity* identity)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _identityList.append(identity);
    endInsertRows();
}

QVariant
IdentityModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        qxtLog->warning() << "Index is invalid so ignoring data call";
        return QVariant();
    }

    Aki::SqlIdentity* identity = _identityList.value(index.row());
    if (!identity) {
        qxtLog->warning() << "Uh oh invalid identity at row";
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return identity->name();
    }
    default: {
        break;
    }
    }

    return QVariant();
}

QList<Aki::SqlIdentity*>
IdentityModel::identities() const
{
    return _identityList;
}

void
IdentityModel::insertIdentity(int row, Aki::SqlIdentity* identity)
{
    if (!identity) {
        return;
    }

    if (row > rowCount()) {
        addIdentity(identity);
        return;
    } else if (row < 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        _identityList.prepend(identity);
        endInsertRows();
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    _identityList.insert(row, identity);
    endInsertRows();
}

void
IdentityModel::removeIdentity(Aki::SqlIdentity* identity)
{
    if (!identity) {
        return;
    }

    const int row = _identityList.indexOf(identity);
    delete takeIdentity(row);
}

int
IdentityModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return _identityList.count();
}

Aki::SqlIdentity*
IdentityModel::takeIdentity(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Aki::SqlIdentity* identity = _identityList.takeAt(row);
    endInsertRows();

    return identity;
}
