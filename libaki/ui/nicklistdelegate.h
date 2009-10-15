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

#ifndef NICKLISTDELEGATE_H
#define NICKLISTDELEGATE_H

#include "libaki_export.h"
#include <QStyledItemDelegate>

namespace Aki
{
class NickListDelegatePrivate;
/**
 * NickList delegate for controlling the online and away
 * colours.
 */
class LIBAKI_EXPORT NickListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    /**
     * Creates the NickList delegate.
     */
    NickListDelegate(QObject *parent = 0);
    /**
     * Destroys the delegate.
     */
    ~NickListDelegate();
    /**
     * Gets the away colour of when the user is away.
     * @return Away colour.
     */
    QColor awayColor() const;
    /**
     * Gets the online colour of when the user is online.
     * @return Online colour.
     */
    QColor onlineColor() const;
    /**
     * Sets the away colour @p color of when the user is away.
     * @param color Away colour.
     */
    void setAwayColor(const QColor &color);
    /**
     * Sets the online colour @p color of when the user is online.
     * @param color Online colour.
     */
    void setOnlineColor(const QColor &color);
protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const;
private:
    friend class NickListDelegatePrivate;
    NickListDelegatePrivate* const d;
}; // End of class NickListDelegate.
} // End of namespace Aki.

#endif // NICKLISTDELEGATE_H
