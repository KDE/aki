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

#ifndef AKI_MAINWINDOW_HPP
#define AKI_MAINWINDOW_HPP

#include "aki.hpp"
#include "interfaces/imaincontroller.hpp"
#include <KDE/KParts/MainWindow>

class KTabBar;
class QMenu;

namespace Aki
{
class DockBar;
class DockWidget;
class View;
class MainWindowPrivate;
/**
 * Reimplementation of a MainWindow that has auto hiding docks.
 *
 * Inherits KParts::MainWindow for easy implementation of plugins
 * to merge menus and/or ToolBars of the plugins in to the MainWindow.
 */
class LIBAKI_EXPORT MainWindow
    : public KParts::MainWindow,
      public Aki::IMainController
{
    Q_OBJECT
public:
    /**
     * Creates a new MainWindow object.
     */
    MainWindow();
    /**
     * Destroys the object.
     */
    virtual ~MainWindow();
    /**
     * Adds a @p dock to a specific @p area in the MainWindow.
     * @param dockWidget DockWidget to be added.
     * @param area Location to place the dock.
     */
    virtual void addDock(Aki::DockWidget* dockWidget, Qt::DockWidgetArea area);

    Aki::DockBar* findDockBar(Aki::DockWidget* dock);
    /**
     * Removes a @p dock from the MainWindow.
     * @note You are responsible for deleting the dock.
     *
     * @param dockWidget DockWidget to be removes.
     */
    virtual void removeDock(Aki::DockWidget* dockWidget);
    QList<Aki::DockBar*> dockToolBars() const;
protected:
    virtual QMenu* createPopupMenu();
private:
    Q_PRIVATE_SLOT(_d, void dockAdded(Aki::DockWidget* dockWidget))
    Q_PRIVATE_SLOT(_d, void dockRemoved(Aki::DockWidget* dockWidget))
    Q_PRIVATE_SLOT(_d, void dockShow(Aki::DockWidget* dockWidget))
    Q_PRIVATE_SLOT(_d, void dockHide(Aki::DockWidget* dockWidget))
    Q_PRIVATE_SLOT(_d, void dockAutoHideStateChanged(Aki::DockWidget* dockWidget,
                                                     bool checked))
private:
    friend class View;
    AKI_DECLARE_PRIVATE(MainWindow)
}; // End of class MainWindow.
} // End of namespace Aki.

#endif // AKI_MAINWINDOW_HPP
