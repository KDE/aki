/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef NICKLISTMODEL_H
#define NICKLISTMODEL_H

#include "libaki_export.h"
#include <QAbstractItemModel>
#include <QScopedPointer>

namespace Aki
{
namespace Irc
{
class User;
} // End of namespace Irc.
class NickListModelPrivate;
class LIBAKI_EXPORT NickListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /**
     * Nick list role for access IRC data.
     */
    enum NickRole {
        /**
         * Allows access to the Aki::Irc::Role.
         */
        IrcUserRole = Qt::UserRole,
        /**
         * Allows access to if the user is away or online.
         */
        IrcUserAwayRole
    }; // end of enum NickRole.
    typedef QList<Aki::Irc::User*> NickList;

    NickListModel(QObject *parent = 0);
    ~NickListModel();
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    void addUser(Aki::Irc::User *user);
    void removeUser(Aki::Irc::User *user);
    QString toolTip(Aki::Irc::User *user) const;
    Aki::NickListModel::NickList users() const;
    Aki::NickListModel::NickList users();
    int count() const;
private:
    friend class NickListModelPrivate;
    QScopedPointer<NickListModelPrivate> d;
}; // End of class NickListModel.
} // End of namespace Aki.

#endif // NICKLISTMODEL_H
