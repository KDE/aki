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

#ifndef AKI_DOCKWIDGET_P_HPP
#define AKI_DOCKWIDGET_P_HPP

#include <KDE/KIcon>

class QAction;
class QMenu;

namespace Aki
{
class DockBar;
class DockButton;
class DockWidget;
class DockWidgetPrivate
{
public:
    explicit DockWidgetPrivate(Aki::DockWidget* qq);
    void autoHideToggled(bool state);
    void dockCustomContextMenuRequested();
    void initialise();
public:
    Aki::DockBar* dockBar;
    Aki::DockButton* dockButton;
    KIcon icon;
    QMenu* contextMenu;
    QAction* autoHideAction;
private:
    void setupActions();
private:
    Aki::DockWidget* _q;
}; // End of class DockWidgetPrivate.
} // End of namespace Aki.

#endif // AKI_DOCKWIDGET_P_HPP
