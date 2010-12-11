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

#include "ibasewindow.hpp"
#include "private/ibasewindow_p.hpp"
using namespace Aki;

IBaseWindow::IBaseWindow(QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::IBaseWindowPrivate(this));
}

IBaseWindow::IBaseWindow(const QString& title, QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::IBaseWindowPrivate(this));
    setTitle(title);
}

IBaseWindow::IBaseWindow(const KIcon& icon, const QString& title, QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::IBaseWindowPrivate(this));
    setIcon(icon);
    setTitle(title);
}

IBaseWindow::IBaseWindow(const QString& title, Aki::IBaseWindow::WindowType type,
                         QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::IBaseWindowPrivate(this));
    setTitle(title);
    setWindowType(type);
}

IBaseWindow::IBaseWindow(const KIcon& icon, const QString& title,
                         Aki::IBaseWindow::WindowType type, QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::IBaseWindowPrivate(this));
    setIcon(icon);
    setTitle(title);
    setWindowType(type);
}

IBaseWindow::~IBaseWindow()
{
}

KIcon
IBaseWindow::icon() const
{
    return _d->icon;
}

void
IBaseWindow::setIcon(const KIcon& icon)
{
    if (icon.isNull()) {
        return;
    }

    _d->icon = icon;
}

void
IBaseWindow::setTitle(const QString& title)
{
    _d->title = title;
}

QString
IBaseWindow::title() const
{
    return _d->title;
}

void
IBaseWindow::setTabColor(Aki::IBaseWindow::TabColor color)
{
    Q_UNUSED(color);
}

void
IBaseWindow::setWindowType(Aki::IBaseWindow::WindowType type)
{
    _d->type = type;
}

Aki::IBaseWindow::TabColor
IBaseWindow::tabColor() const
{
    return _d->tabColor;
}

Aki::IBaseWindow::WindowType
IBaseWindow::type() const
{
    return _d->type;
}

#include "interfaces/ibasewindow.moc"
