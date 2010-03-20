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

#include "nicklistmodel.h"
#include <Aki/Irc/User>
#include <KIcon>
#include <KLocale>
using namespace Aki;

namespace Aki
{
class NickListModelPrivate
{
public:
    NickListModel::NickList nickList;
}; // End of class NickListModelPrivate.
} // End of namespace Aki.

NickListModel::NickListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    d.reset(new Aki::NickListModelPrivate);
}

NickListModel::~NickListModel()
{
    qDeleteAll(d->nickList);
    d->nickList.clear();
}

QVariant
NickListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() > d->nickList.count()) {
        return QVariant();
    }

    switch(role) {
    case Qt::ToolTipRole: {
        return toolTip(d->nickList.at(index.row()));
        break;
    }
    case Qt::DisplayRole: {
        return d->nickList.at(index.row())->nick();
        break;
    }
    case Qt::DecorationRole: {
        Aki::Irc::User *user = d->nickList.at(index.row());
        if (user->isAway()) {
            return KIcon("aki_away");
        } else if (user->isOp()) {
            return KIcon("aki_operator");
        } else if (user->isHalfOp()) {
            return KIcon("aki_halfop");
        } else if (user->isVoice()) {
            return KIcon("aki_voice");
        } else {
            return KIcon("aki_normal");
        }
        break;
    }
    case IrcUserAwayRole: {
        return d->nickList.at(index.row())->isAway();
        break;
    }
    case IrcUserRole: {
        return QVariant::fromValue(d->nickList.at(index.row()));
        break;
    }
    default:
        return QVariant();
    }

    return QVariant();
}

int
NickListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d->nickList.count();
}

bool
NickListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    if (role != IrcUserRole) {
        return false;
    }

    d->nickList[index.row()] = value.value<Aki::Irc::User*>();
    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags
NickListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QString
NickListModel::toolTip(Aki::Irc::User *user) const
{
    QString table;
    table += "<table border='0' cellspacing='0' cellpadding='0'>";
    table += "<tr><td><strong>Username:</strong></td>";
    table += QString("<td> %1</td></tr>").arg(user->realName());
    table += "<tr><td><strong>Hostname:</strong></td>";
    table += QString("<td> %1</td></tr>").arg(user->hostMask());

    QString tip;

    if (user->isAway()) {
        table += "<tr><td><strong>Away Message:</strong></td>";
        table += QString("<td> %1</td></tr>").arg(user->awayMessage());
    }

    QString status;

    if (!user->isNormal()) {
        table += "<tr><td><strong>Mode:</strong></td>";
        table += "<td> ";
        if (user->isOp() && !user->isHalfOp() && !user->isVoice()) {
            table += i18n("Operator");
        } else if (user->isOp() && user->isHalfOp() && !user->isVoice()) {
            table += i18n("Operator, Half-Operator");
        } else if (user->isOp() && user->isHalfOp() && user->isVoice()) {
            table += i18n("Operator, Half-Operator, Voice");
        } else if (!user->isOp() && user->isHalfOp() && !user->isVoice()) {
            table += i18n("Half-Operator");
        } else if (!user->isOp() && user->isHalfOp() && user->isVoice()) {
            table += i18n("Half-Operator, Voice");
        } else if (!user->isOp() && !user->isHalfOp() && user->isVoice()) {
            table += i18n("Voice");
        } else if (user->isOp() && !user->isHalfOp() && user->isVoice()) {
            table += i18n("Operator, Voice");
        }
        table += "</td></tr>";
    }

    if (user->idleTime().isValid()) {
        table += "<tr><td><strong>Last Message:</strong></td>";
        table += QString("<td> %1</td></tr>").arg(user->idleTime().toString("%B %d %Y %H:%M:%S"));
    }

    table += "</table>";

    return table;
}

NickListModel::NickList
NickListModel::users() const
{
    return d->nickList;
}

NickListModel::NickList
NickListModel::users()
{
    return d->nickList;
}

int
NickListModel::count() const
{
    return d->nickList.count();
}

void
NickListModel::addUser(Aki::Irc::User *user)
{
    if (d->nickList.contains(user)) {
        return;
    }

    int row = 0;
    if (d->nickList.count() == 0) {
        row = 0;
    } else {
        row = d->nickList.count();
    }

    beginInsertRows(QModelIndex(), row, row);
    d->nickList.insert(row, user);
    endInsertRows();
}

void
NickListModel::removeUser(Aki::Irc::User *user)
{
    const int row = d->nickList.indexOf(user);
    if (row == -1) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    d->nickList.removeAt(row);
    endRemoveRows();
}
