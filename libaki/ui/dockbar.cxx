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

#include "dockbar.hpp"
#include "debughelper.hpp"
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
    DEBUG_FUNC_NAME
    _d->area = area;
    switch (area) {
    case Qt::TopToolBarArea: {
        DEBUG_TEXT("TopToolBarArea")
        setObjectName("TopToolBarArea");
        break;
    }
    case Qt::LeftToolBarArea: {
        DEBUG_TEXT("LeftToolBarArea")
        setObjectName("LeftToolBarArea");
        break;
    }
    case Qt::RightToolBarArea: {
        DEBUG_TEXT("RightToolBaeArea")
        setObjectName("RightToolBarArea");
        break;
    }
    case Qt::BottomToolBarArea: {
        DEBUG_TEXT("BottomToolBarArea")
        setObjectName("BottomToolBarArea");
        break;
    }
    default: {
        DEBUG_TEXT("UnknownToolBarArea")
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
