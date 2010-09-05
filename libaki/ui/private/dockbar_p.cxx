#include "dockbar_p.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include <KDE/KIcon>
#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
using namespace Aki;

DockBarPrivate::DockBarPrivate(DockBar* qq)
    : hideDockMapper(0),
    showDockMapper(0),
    _q(qq)
{
}

void
DockBarPrivate::buttonTriggered(QAction* action)
{
    Aki::DockButton* button = _q->buttonForAction(action);
    if (button && button->dockWidget()->isVisible()) {
        return;
    }

    emit _q->dockShow(button->dockWidget());
}

void
DockBarPrivate::createAction(Aki::DockButton* button)
{
    QAction* action = _q->addWidget(button);
    button->setDefaultAction(action);
    action->setText(button->dockWidget()->title());
    action->setIcon(button->dockWidget()->icon());
}

void
DockBarPrivate::dockAutoHideStateChanged(Aki::DockWidget* dock, bool checked)
{
    kDebug() << "heheheeh";
    QList<QAction*> actionList = _q->actions();
    foreach (QAction* action, actionList) {
        Aki::DockButton* dockButton = _q->buttonForAction(action);
        if (dockButton && dockButton->dockWidget() == dock) {
            action->setVisible(checked);
            dockButton->setAutoHide(checked);

            emit _q->dockAutoHideStateChanged(dock, checked);
            return;
        }
    }
}

void
DockBarPrivate::dockHide(QWidget* dock)
{
    Aki::DockWidget* dockWidget = qobject_cast<Aki::DockWidget*>(dock);
    if (dockWidget) {
        emit _q->dockHide(dockWidget);
    }
}

void
DockBarPrivate::dockShow(QWidget* dock)
{
    Aki::DockWidget* dockWidget = qobject_cast<Aki::DockWidget*>(dock);
    if (dockWidget) {
        emit _q->dockShow(dockWidget);
    }
}
