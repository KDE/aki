/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef AKI_DOCKBAR_HPP
#define AKI_DOCKBAR_HPP

#include "aki.hpp"
#include <KDE/KToolBar>

class QAction;

namespace Aki
{
class DockButton;
class DockBarPrivate;
class DockWidget;
class LIBAKI_EXPORT DockBar
    : public QToolBar
{
    Q_OBJECT
public:
    explicit DockBar(QWidget* parent = 0);
    ~DockBar();
    void addDockWidget(Aki::DockWidget* dock);
    Aki::DockButton* buttonForAction(QAction* action);
    Aki::DockButton* buttonForDockWidget(Aki::DockWidget* dock);
    void setArea(Qt::ToolBarArea area);
    Qt::ToolBarArea area() const;
Q_SIGNALS:
    void dockAdded(Aki::DockWidget* dock);
    void dockHide(Aki::DockWidget* dock);
    void dockRemoved(Aki::DockWidget* dock);
    void dockShow(Aki::DockWidget* dock);
    void dockAutoHideStateChanged(Aki::DockWidget* dock, bool checked);
private:
    Q_PRIVATE_SLOT(_d, void buttonTriggered(QAction* action))
    Q_PRIVATE_SLOT(_d, void dockHide(QWidget* dock))
    Q_PRIVATE_SLOT(_d, void dockShow(QWidget* dock))
    Q_PRIVATE_SLOT(_d, void dockAutoHideStateChanged(Aki::DockWidget* dockWidget, bool checked))
private:
    AKI_DECLARE_PRIVATE(DockBar)
}; // End of class DockBar.
} // End of namespace Aki.

#endif // AKI_DOCKBAR_HPP
