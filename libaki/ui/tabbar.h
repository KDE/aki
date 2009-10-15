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

#ifndef AKI_TABBAR_H
#define AKI_TABBAR_H

#include "libaki_export.h"
#include <KTabBar>

namespace Aki
{
class TabBarPrivate;
/**
 * Extends KTabBar with a new feature to tear off the tab widget.
 */
class LIBAKI_EXPORT TabBar : public KTabBar
{
    Q_OBJECT
public:
    /**
     * Creates a new tab bar.
     */
    TabBar(QWidget *parent = 0);
    /**
     * Destroys the object.
     */
    ~TabBar();
    /**
     * @copydoc KTabBar::mousePressEvent
     */
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent* event);
    void setDragMode(bool state);
private:
    friend class TabBarPrivate;
    TabBarPrivate* const d;
}; // End of class TabBar.
} // End of namespace Aki.

#endif // AKI_TABBAR_H
