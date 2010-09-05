#include "dockwidget_p.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include <QtGui/QAction>
#include <QtGui/QMenu>
using namespace Aki;

DockWidgetPrivate::DockWidgetPrivate(Aki::DockWidget* qq)
    : dockBar(0),
    dockButton(0),
    icon(KIcon()),
    contextMenu(0),
    autoHideAction(0),
    _q(qq)
{
}

void
DockWidgetPrivate::autoHideToggled(bool state)
{
    emit _q->autoHideStateChanged(_q, state);
}

void
DockWidgetPrivate::initialise()
{
    _q->setContextMenuPolicy(Qt::CustomContextMenu);
    _q->setMouseTracking(true);
    setupActions();

    _q->connect(_q, SIGNAL(customContextMenuRequested(QPoint)),
                SLOT(dockCustomContextMenuRequested()));
}

void
DockWidgetPrivate::dockCustomContextMenuRequested()
{
    contextMenu->exec(QCursor::pos());
}

void
DockWidgetPrivate::setupActions()
{
    contextMenu = new QMenu(_q);

    autoHideAction = new QAction(i18n("Auto-Hide"), contextMenu);
    autoHideAction->setCheckable(true);
    autoHideAction->setChecked(true);
    _q->connect(autoHideAction, SIGNAL(toggled(bool)),
                _q, SLOT(autoHideToggled(bool)));

    contextMenu->addAction(autoHideAction);
}
