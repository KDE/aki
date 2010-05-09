#include "mainwindow_p.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include "ui/mainwindow.hpp"
#include "ui/view.hpp"
#include "ui/viewtabbar.hpp"
#include <KDE/KDebug>
#include <KDE/KMenuBar>
using namespace Aki;

MainWindowPrivate::MainWindowPrivate(Aki::MainWindow* q)
    : QObject(),
    cornerWidget(0),
    _q(q)
{
}

void
MainWindowPrivate::addDockBar(Qt::ToolBarArea area)
{
    Aki::DockBar* dockBar = new Aki::DockBar(_q);
    _q->connect(dockBar, SIGNAL(dockAdded(Aki::DockWidget*)),
                this, SLOT(dockAdded(Aki::DockWidget*)));
    _q->connect(dockBar, SIGNAL(dockRemoved(Aki::DockWidget*)),
                this, SLOT(dockRemoved(Aki::DockWidget*)));
    _q->connect(dockBar, SIGNAL(dockShow(Aki::DockWidget*)),
                this, SLOT(dockShow(Aki::DockWidget*)));
    _q->connect(dockBar, SIGNAL(dockHide(Aki::DockWidget*)),
                this, SLOT(dockHide(Aki::DockWidget*)));
    _q->connect(dockBar, SIGNAL(dockAutoHideStateChanged(Aki::DockWidget*,bool)),
                this, SLOT(dockAutoHideStateChanged(Aki::DockWidget*,bool)));

    dockBar->setDockArea(area);
    _q->addToolBar(area, dockBar);
}

void
MainWindowPrivate::dockAdded(Aki::DockWidget* dock)
{
    Aki::DockBar* dockBar = qobject_cast<Aki::DockBar*>(sender());
    if (!dockBar) {
        return;
    }

    switch (dockBar->area()) {
    case Qt::LeftToolBarArea: {
        _q->addDockWidget(Qt::LeftDockWidgetArea, dock);
        return;
    }
    case Qt::RightToolBarArea: {
        _q->addDockWidget(Qt::RightDockWidgetArea, dock);
        return;
    }
    case Qt::TopToolBarArea: {
        _q->addDockWidget(Qt::TopDockWidgetArea, dock);
        return;
    }
    case Qt::BottomToolBarArea: {
        _q->addDockWidget(Qt::BottomDockWidgetArea, dock);
        return;
    }
    }
}

void
MainWindowPrivate::dockAutoHideStateChanged(Aki::DockWidget* dock, bool checked)
{
    if (!checked) {
        dock->show();
    }
}

void
MainWindowPrivate::dockHide(Aki::DockWidget* dock)
{
    if (!dock->isVisible()) {
        return;
    }

    dock->hide();
}

void
MainWindowPrivate::dockLocationChanged(Qt::DockWidgetArea area)
{
    // Get the dockwidget that called this.
    Aki::DockWidget* dockWidget = qobject_cast<Aki::DockWidget*>(_q->sender());
    if (!dockWidget) {
        return;
    }

    Aki::DockBar* oldDock = 0;

    // Convert Qt::DockWidgetArea to Qt::ToolBarArea.
    Qt::ToolBarArea newArea = toToolBarArea(area);

    if (newArea == Qt::NoToolBarArea) {
        return;
    }

    // Check which toolbar has this dock.
    QList<KToolBar*> tbl = _q->toolBars();
    foreach (KToolBar* bar, tbl) {
        Aki::DockBar* dockBar = qobject_cast<Aki::DockBar*>(bar);
        if (dockBar && dockBar->buttonForDock(dockWidget)) {
            oldDock = dockBar;
            break;
        }
    }

    /* Check and make sure that this new drop area is not the same
       as the previous one */
    if (newArea == oldDock->area()) {
        return;
    }

    // Delete the old button for that dock.
    delete oldDock->buttonForDock(dockWidget);

    // Loop the toolbars to locate the new one and add it.
    foreach (KToolBar* bar, tbl) {
        Aki::DockBar* dockBar = qobject_cast<Aki::DockBar*>(bar);
        if (dockBar->area() == newArea) {
            dockBar->addDock(dockWidget);
            return;
        }
    }
}

void
MainWindowPrivate::dockRemoved(Aki::DockWidget* dock)
{
    _q->removeDockWidget(dock);
}

void
MainWindowPrivate::dockShow(Aki::DockWidget* dock)
{
    if (!dock->isHidden()) {
        return;
    }

    dock->show();
}

void
MainWindowPrivate::setTopRightCorner(Aki::ViewTabBar* widget)
{
    if (cornerWidget != widget) {
        if (cornerWidget) {
            delete cornerWidget;
        }

        cornerWidget = widget;
        cornerWidget->setTabsClosable(true);
        cornerWidget->setMovable(true);
        cornerWidget->setContextMenuPolicy(Qt::CustomContextMenu);

        _q->menuBar()->setCornerWidget(cornerWidget);
    }
}

Qt::ToolBarArea
MainWindowPrivate::toToolBarArea(Qt::DockWidgetArea area)
{
    switch (area) {
    case Qt::LeftDockWidgetArea: {
        return Qt::LeftToolBarArea;
    }
    case Qt::RightDockWidgetArea: {
        return Qt::RightToolBarArea;
    }
    case Qt::TopDockWidgetArea: {
        return Qt::TopToolBarArea;
    }
    case Qt::BottomDockWidgetArea: {
        return Qt::BottomToolBarArea;
    }
    default: {
        return Qt::NoToolBarArea;
    }
    }
}
