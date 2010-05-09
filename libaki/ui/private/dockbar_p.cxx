#include "dockbar_p.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include <QtGui/QAction>
using namespace Aki;

DockBarPrivate::DockBarPrivate(Aki::DockBar* qq)
    : QObject(),
    showDockMapper(0),
    hideDockMapper(0),
    _q(qq)
{
}

void
DockBarPrivate::buttonTriggered(QAction* action)
{
    Aki::DockButton* button = _q->buttonForAction(action);
    if (button && button->dock()->isVisible()) {
        return;
    }

    emit _q->dockShow(button->dock());
}

void
DockBarPrivate::createAction(Aki::DockButton* button)
{
    QAction* action = _q->addWidget(button);
    button->setDefaultAction(action);
    action->setText(button->dock()->title());
    action->setIcon(button->dock()->icon());
}

void
DockBarPrivate::dockAutoHideStateChanged(bool checked)
{
    Aki::DockWidget* dock = qobject_cast<Aki::DockWidget*>(_q->sender());
    if (!dock) {
        return;
    }

    QList<QAction*> al = _q->actions();
    // Loop through all the actions and fine the action for this dock.
    foreach (QAction* action, al) {
        // Now that we got the action get the widget that is assocated with the action.
        Aki::DockButton* button = _q->buttonForAction(action);
        // Check that the button is valid.
        if (button && button->dock() == dock) {
            action->setVisible(checked);
            button->setAutoHide(checked);
            // Emit the signal so the main window knows of this too.
            emit _q->dockAutoHideStateChanged(dock, checked);
            return;
        }
    }
}

void
DockBarPrivate::dockHide(QWidget* dock)
{
    Aki::DockWidget* widget = qobject_cast<Aki::DockWidget*>(dock);
    emit _q->dockHide(widget);
}

void
DockBarPrivate::dockShow(QWidget* dock)
{
    // Cast this to a DockWidget.
    Aki::DockWidget* widget = qobject_cast<Aki::DockWidget*>(dock);
    emit _q->dockShow(widget);
}
