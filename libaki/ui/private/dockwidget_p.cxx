#include "dockwidget_p.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include <KDE/KLocale>
#include <QtGui/QAction>
#include <QtGui/QMenu>
using namespace Aki;

DockWidgetPrivate::DockWidgetPrivate(Aki::DockWidget* qq)
    : QObject(),
    dockBar(0),
    dockButton(0),
    icon(KIcon()),
    contextMenu(0),
    autoHideAction(0),
    _q(qq)
{
}

void
DockWidgetPrivate::autoHideToggled(bool checked)
{
    emit _q->dockAutoHideStateChanged(checked);
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
    connect(autoHideAction, SIGNAL(toggled(bool)),
            SLOT(autoHideToggled(bool)));
}
