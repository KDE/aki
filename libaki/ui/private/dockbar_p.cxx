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
