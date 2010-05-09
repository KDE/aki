#include "mainwindow.hpp"
#include "private/mainwindow_p.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/view.hpp"
#include "ui/viewtabbar.hpp"
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
    _d->addDockBar(Qt::BottomToolBarArea);
    _d->addDockBar(Qt::RightToolBarArea);
}

MainWindow::~MainWindow()
{
}

void
MainWindow::addDock(Qt::ToolBarArea area, Aki::DockWidget* dock)
{
    Q_ASSERT(dock);

    if (area == Qt::NoToolBarArea) {
        return;
    }

    QList<KToolBar*> tbl = toolBars();
    foreach (KToolBar* bar, tbl) {
        if (toolBarArea(bar) == area) {
            qobject_cast<Aki::DockBar*>(bar)->addDock(dock);
        }
    }
}

QMenu*
MainWindow::createPopupMenu()
{
    return 0;
}

void
MainWindow::removeDock(Aki::DockWidget* dock)
{
    // Loop through all the toolbars.
    QList<KToolBar*> tbl = toolBars();
    foreach (KToolBar* bar, tbl) {
        // Attempt at casting to the DockBar.
        Aki::DockBar* dockBar = qobject_cast<Aki::DockBar*>(bar);
        /* Check and see if it is valid because if the user added more dockbars of their own
           hopefully it will fail here. */
        if (dockBar) {
            // So we got a valid DockBar and now we loop through all the actions.
            QList<QAction*> al = dockBar->actions();
            foreach (QAction* action, al) {
                // Now cast for the DockButton which holds the dock.
                Aki::DockButton* widget =
                    qobject_cast<Aki::DockButton*>(dockBar->widgetForAction(action));
                // Check and see if the widget is valid.
                if (widget) {
                    // Check and see if the dock is part of this action button.
                    if (widget->dock() == dock) {
                        dockBar->removeAction(action);
                        return;
                    }
                }
            }
        }
    }
}

void
MainWindow::setTopRightCorner(Aki::ViewTabBar* widget)
{
    Q_ASSERT(widget);
    _d->setTopRightCorner(widget);
}

Aki::ViewTabBar*
MainWindow::topRightCorner()
{
    return _d->cornerWidget;
}

Aki::ViewTabBar*
MainWindow::topRightCorner() const
{
    return _d->cornerWidget;
}

void
MainWindow::setView(Aki::View* view)
{
    _d->view = view;
    setCentralWidget(_d->view);

    connect(_d->cornerWidget, SIGNAL(moveTab(int,int)),
            _d->view, SLOT(viewPositionChanged(int,int)));
    connect(_d->cornerWidget, SIGNAL(tabCloseRequested(int)),
            _d->view, SLOT(viewPositionClose(int)));
    connect(_d->cornerWidget, SIGNAL(currentChanged(int)),
            _d->view, SLOT(viewCurrentChanged(int)));
}

Aki::View* MainWindow::view()
{
    return _d->view;
}

Aki::View*
MainWindow::view() const
{
    return _d->view;
}
