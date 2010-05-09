#include "dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include "private/dockbar_p.hpp"
#include <QtGui/QAction>
#include <QtCore/QSignalMapper>
using namespace Aki;

DockBar::DockBar(QWidget* parent)
    : KToolBar(parent)
{
    _d.reset(new Aki::DockBarPrivate(this));
    _d->showDockMapper = new QSignalMapper(this);
    _d->hideDockMapper = new QSignalMapper(this);

     connect(_d->showDockMapper, SIGNAL(mapped(QWidget*)),
             _d.data(), SLOT(dockShow(QWidget*)));
     connect(_d->hideDockMapper, SIGNAL(mapped(QWidget*)),
             _d.data(), SLOT(dockHide(QWidget*)));

    setContextMenuPolicy(Qt::NoContextMenu);
    setToolBarsEditable(false);
    setToolBarsLocked(true);
    setFloatable(false);
    setMovable(false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

DockBar::~DockBar()
{
}

void
DockBar::addDock(Aki::DockWidget* dock)
{
    if (!dock) {
        return;
    }

    Aki::DockButton* button = new Aki::DockButton(this);
    button->setDock(dock);
    button->setAutoHide(true);
    dock->setDockBar(this);
    dock->hide();
    _d->createAction(button);

    connect(button, SIGNAL(triggered(QAction*)),
            _d.data(), SLOT(buttonTriggered(QAction*)));
    connect(button, SIGNAL(mouseLeave()),
            _d->hideDockMapper, SLOT(map()));
    connect(dock, SIGNAL(dockAutoHideStateChanged(bool)),
            _d.data(), SLOT(dockAutoHideStateChanged(bool)));

    _d->showDockMapper->setMapping(button, dock);
    _d->hideDockMapper->setMapping(button, dock);

    if (area() == Qt::LeftToolBarArea || area() == Qt::RightToolBarArea) {
        button->setOrientation(Qt::Vertical);
    } else {
        button->setOrientation(Qt::Horizontal);
    }

    emit dockAdded(button->dock());
}

Qt::ToolBarAreas
DockBar::area() const
{
    return allowedAreas();
}

Aki::DockButton*
DockBar::buttonForAction(QAction* action)
{
    return qobject_cast<Aki::DockButton*>(widgetForAction(action));
}

Aki::DockButton*
DockBar::buttonForDock(Aki::DockWidget* dock)
{
    QList<QAction*> al = actions();
    foreach (QAction* action, al) {
        Aki::DockButton* button = buttonForAction(action);
        if (button && button->dock() == dock) {
            return button;
        }
    }

    return 0;
}

void
DockBar::removeDock(Aki::DockWidget* dock)
{
    Q_UNUSED(dock);
}

void
DockBar::setDockArea(Qt::ToolBarArea area)
{
    setAllowedAreas(area);
}
