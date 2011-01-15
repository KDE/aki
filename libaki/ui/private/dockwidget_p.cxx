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
