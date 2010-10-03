/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
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

#include "dockwidget.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/private/dockwidget_p.hpp"
#include <KDE/KIcon>
#include <QtCore/QEvent>
using namespace Aki;

DockWidget::DockWidget(QWidget* parent)
    : QDockWidget(parent)
{
    _d.reset(new Aki::DockWidgetPrivate(this));
    _d->initialise();
    setIcon(KIcon("aki")),
    setTitle(QString());
}

DockWidget::DockWidget(const QString& title, QWidget* parent)
    : QDockWidget(title, parent)
{
    _d.reset(new Aki::DockWidgetPrivate(this));
    _d->initialise();
    setIcon(KIcon("aki"));
    setTitle(title);
}

DockWidget::DockWidget(const QString& title, const KIcon& icon, QWidget* parent)
    : QDockWidget(title, parent)
{
    _d.reset(new Aki::DockWidgetPrivate(this));
    _d->initialise();
    setIcon(icon);
    setTitle(title);
}

DockWidget::~DockWidget()
{

}

Aki::DockBar*
DockWidget::dockBar() const
{
    return _d->dockBar;
}

KIcon
DockWidget::icon() const
{
    return _d->icon;
}

void
DockWidget::leaveEvent(QEvent*)
{
    if (!_d->dockButton) {
        _d->dockButton = _d->dockBar->buttonForDockWidget(this);
    }

    if (_d->dockButton && _d->dockButton->isAutoHide()) {
        emit mouseLeave();
    }
}

void
DockWidget::setDockBar(DockBar* dockBar)
{
    Q_ASSERT(dockBar);
    _d->dockBar = dockBar;
}

void
DockWidget::setIcon(const KIcon& icon)
{
    _d->icon = icon;
}

void
DockWidget::setTitle(const QString& title)
{
    setObjectName(title);
}

QString
DockWidget::title() const
{
    return objectName();
}

#include "ui/dockwidget.moc"
