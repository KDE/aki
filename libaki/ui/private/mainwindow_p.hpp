/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
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

#ifndef AKI_MAINWINDOWPRIVATE_HPP
#define AKI_MAINWINDOWPRIVATE_HPP

#include <KDE/KTabBar>
#include <QtCore/QObject>

namespace Aki
{
class DockBar;
class DockWidget;
class MainWindow;
class View;
class MainWindowPrivate : public QObject
{
    Q_OBJECT
public:
    explicit MainWindowPrivate(Aki::MainWindow* mainWindow);
    void addDockBar(Qt::ToolBarArea area);
    void dockAdded(Aki::DockWidget* dockWidget);
    void dockAutoHideStateChanged(Aki::DockWidget* dockWidget, bool checked);
    void dockRemoved(Aki::DockWidget* dockWidget);
    void dockHide(Aki::DockWidget* dockWidget);
    void dockShow(Aki::DockWidget* dockWidget);
public:
    QList<Aki::DockBar*> dockBars;
    Aki::View* view;
private:
    Aki::MainWindow* _q;
}; // End of class MainWindowPrivate;
} // End of namespace Aki.

#endif // AKI_MAINWINDOWPRIVATE_HPP
