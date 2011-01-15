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

#include "mainwindow_p.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include "ui/mainwindow.hpp"
#include <KDE/KDebug>
#include <KDE/KMenuBar>
using namespace Aki;

MainWindowPrivate::MainWindowPrivate(Aki::MainWindow* q)
    : _q(q)
{
}

void
MainWindowPrivate::addDockBar(Qt::ToolBarArea area)
{
    Aki::DockBar* dockBar = new Aki::DockBar(_q);

    _q->connect(dockBar, SIGNAL(dockAdded(Aki::DockWidget*)),
                SLOT(dockAdded(Aki::DockWidget*)));
    _q->connect(dockBar, SIGNAL(dockRemoved(Aki::DockWidget*)),
                SLOT(dockRemoved(Aki::DockWidget*)));
    _q->connect(dockBar, SIGNAL(dockShow(Aki::DockWidget*)),
                SLOT(dockShow(Aki::DockWidget*)));
    _q->connect(dockBar, SIGNAL(dockHide(Aki::DockWidget*)),
                SLOT(dockHide(Aki::DockWidget*)));
    _q->connect(dockBar, SIGNAL(dockAutoHideStateChanged(Aki::DockWidget*,bool)),
                SLOT(dockAutoHideStateChanged(Aki::DockWidget*,bool)));
    
    dockBar->setArea(area);
    _q->addToolBar(area, dockBar);
    dockBars.append(dockBar);
}

void
MainWindowPrivate::dockAdded(Aki::DockWidget* dockWidget)
{
    Aki::DockBar* dockBar = qobject_cast<Aki::DockBar*>(_q->sender());
    if (!dockBar) {
        return;
    }

    switch (dockBar->area()) {
    case Qt::TopToolBarArea: {
        _q->addDockWidget(Qt::TopDockWidgetArea, dockWidget);
        break;
    }
    case Qt::LeftToolBarArea: {
        _q->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
        break;
    }
    case Qt::RightToolBarArea: {
        _q->addDockWidget(Qt::RightDockWidgetArea, dockWidget);
        break;
    }
    case Qt::BottomToolBarArea: {
        _q->addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
        break;
    }
    default: {
        break;
    }
    }
}

void
MainWindowPrivate::dockAutoHideStateChanged(Aki::DockWidget* dockWidget, bool checked)
{
    if (!checked) {
        dockWidget->show();
    }
}

void
MainWindowPrivate::dockHide(Aki::DockWidget* dockWidget)
{
    if (dockWidget->isVisible()) {
        dockWidget->hide();
    }
}

void
MainWindowPrivate::dockRemoved(Aki::DockWidget* dockWidget)
{
    _q->removeDockWidget(dockWidget);
}

void
MainWindowPrivate::dockShow(Aki::DockWidget* dockWidget)
{
    if (dockWidget->isHidden()) {
        dockWidget->show();
    }
}
