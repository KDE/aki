#include "useritem.hpp"
using namespace Aki;

UserItem::UserItem(QTreeWidget* parent)
    : QTreeWidgetItem(parent)
{
}

UserItem::UserItem(Aki::Irc::User* user, QTreeWidgetItem* parent)
    : QTreeWidgetItem(parent)
{
    Q_UNUSED(user);
}

UserItem::~UserItem()
{
}

QVariant
UserItem::data(int column, int role) const
{
    return QTreeWidgetItem::data(column, role);
}

bool
UserItem::operator<(const QTreeWidgetItem& other) const
{
    return QTreeWidgetItem::operator<(other);
}

void
UserItem::setData(int column, int role, const QVariant& value)
{
    QTreeWidgetItem::setData(column, role, value);
}

void
UserItem::setUser(Aki::Irc::User* user)
{
    Q_UNUSED(user);
}

Aki::Irc::User*
UserItem::user() const
{
    return 0;
}
