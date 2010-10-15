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

#include "mainwindow.hpp"
#include "private/mainwindow_p.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include <KDE/KDebug>
#include <KDE/KToolBar>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
using namespace Aki;

MainWindow::MainWindow()
    : KParts::MainWindow()
{
    _d.reset(new MainWindowPrivate(this));

    _d->addDockBar(Qt::TopToolBarArea);
    _d->addDockBar(Qt::LeftToolBarArea);
    _d->addDockBar(Qt::RightToolBarArea);
    _d->addDockBar(Qt::BottomToolBarArea);
}

MainWindow::~MainWindow()
{
}

QMenu*
MainWindow::createPopupMenu()
{
    return 0;
}

void
MainWindow::addDock(DockWidget* dock, Qt::DockWidgetArea area)
{
    QList<Aki::DockBar*> tbl = dockToolBars();
    foreach (Aki::DockBar* bar, tbl) {
        if (bar->area() == static_cast<Qt::ToolBarArea>(area)) {
            qobject_cast<Aki::DockBar*>(bar)->addDockWidget(dock);
            break;
        }
    }
}

Aki::DockBar*
MainWindow::findDockBar(Aki::DockWidget* dock)
{
    QList<Aki::DockBar*> dockBarList = _d->dockBars;
    foreach (Aki::DockBar* dockBar, dockBarList) {
        if (dockBar) {
            QList<QAction*> actionList = dockBar->actions();
            foreach (QAction* action, actionList) {
                Aki::DockButton* dockButton = qobject_cast<Aki::DockButton*>(dockBar->widgetForAction(action));
                if (dockButton && dockButton->dockWidget() == dock) {
                    return dockBar;
                }
            }
        }
    }

    return 0;
}

QList<Aki::DockBar*>
MainWindow::dockToolBars() const
{
    return _d->dockBars;
}

void
MainWindow::removeDock(Aki::DockWidget* dock)
{
    QList<Aki::DockBar*> tbl = dockToolBars();
    foreach (Aki::DockBar* dockBar, tbl) {
        if (dockBar) {
            QList<QAction*> al = dockBar->actions();
            foreach (QAction* action, al) {
                Aki::DockButton* bar =
                    qobject_cast<Aki::DockButton*>(dockBar->widgetForAction(action));
                if (bar && bar->dockWidget() == dock) {
                    dockBar->removeAction(action);
                    return;
                }
            }
        }
    }
}

#include "ui/mainwindow.moc"
