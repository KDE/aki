#include "dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include "ui/private/dockbar_p.hpp"
#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
using namespace Aki;

DockBar::DockBar(QWidget* parent)
    : QToolBar(parent)
{
    _d.reset(new Aki::DockBarPrivate(this));
    _d->hideDockMapper = new QSignalMapper(this);
    _d->showDockMapper = new QSignalMapper(this);

    connect(_d->hideDockMapper, SIGNAL(mapped(QWidget*)),
            SLOT(dockHide(QWidget*)));
    connect(_d->showDockMapper, SIGNAL(mapped(QWidget*)),
            SLOT(dockShow(QWidget*)));

    setContextMenuPolicy(Qt::NoContextMenu);
    //setToolBarsEditable(false);
    //setToolBarsLocked(true);
    setFloatable(false);
    setMovable(false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

DockBar::~DockBar()
{
}

void
DockBar::addDockWidget(Aki::DockWidget* dock)
{
    if (!dock) {
        return;
    }

    Aki::DockButton* dockButton = new Aki::DockButton(this);
    dockButton->setAutoHide(true);
    dockButton->setDockWidget(dock);

    dock->setDockBar(this);
    dock->hide();

    _d->createAction(dockButton);

    connect(dockButton, SIGNAL(triggered(QAction*)),
            SLOT(buttonTriggered(QAction*)));
    connect(dockButton, SIGNAL(mouseEnter()),
            _d->showDockMapper, SLOT(map()));
    connect(dockButton, SIGNAL(mouseLeave()),
            _d->hideDockMapper, SLOT(map()));
    connect(dock, SIGNAL(autoHideStateChanged(Aki::DockWidget*,bool)),
            SLOT(dockAutoHideStateChanged(Aki::DockWidget*,bool)));

    _d->hideDockMapper->setMapping(dockButton, dock);
    _d->showDockMapper->setMapping(dockButton, dock);

    if (area() == Qt::LeftToolBarArea || area() == Qt::RightToolBarArea) {
        dockButton->setOrientation(Qt::Vertical);
    } else {
        dockButton->setOrientation(Qt::Horizontal);
    }

    emit dockAdded(dockButton->dockWidget());
}

Aki::DockButton*
DockBar::buttonForAction(QAction* action)
{
    return qobject_cast<Aki::DockButton*>(widgetForAction(action));
}

Aki::DockButton*
DockBar::buttonForDockWidget(Aki::DockWidget* dock)
{
    QList<QAction*> actionList = actions();
    foreach (QAction* action, actionList) {
        Aki::DockButton* dockButton = buttonForAction(action);
        if (dockButton && dockButton->dockWidget() == dock) {
            return dockButton;
        }
    }

    return 0;
}

void
DockBar::setArea(Qt::ToolBarArea area)
{
    _d->area = area;
    switch (area) {
    case Qt::TopToolBarArea: {
        setObjectName("TopToolBarArea");
        break;
    }
    case Qt::LeftToolBarArea: {
        setObjectName("LeftToolBarArea");
        break;
    }
    case Qt::RightToolBarArea: {
        setObjectName("RightToolBarArea");
        break;
    }
    case Qt::BottomToolBarArea: {
        setObjectName("BottomToolBarArea");
        break;
    }
    default: {
        break;
    }
    }

    setAllowedAreas(area);
}

Qt::ToolBarArea
DockBar::area() const
{
    return _d->area;
}

#include "ui/dockbar.moc"
