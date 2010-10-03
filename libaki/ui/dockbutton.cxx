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

#include "dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include "ui/private/dockbutton_p.hpp"
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionToolButton>
#include <QtGui/QStylePainter>
using namespace Aki;

DockButton::DockButton(QWidget* parent)
    : QToolButton(parent)
{
    _d.reset(new Aki::DockButtonPrivate(this));
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

DockButton::~DockButton()
{
}

Aki::DockWidget*
DockButton::dockWidget() const
{
    return _d->dockWidget;
}


bool
DockButton::isAutoHide() const
{
    return _d->isAutoHide;
}

Qt::Orientation
DockButton::orientation() const
{
    return _d->orientation;
}

void
DockButton::paintEvent(QPaintEvent* event)
{
    if (orientation() == Qt::Horizontal) {
        QToolButton::paintEvent(event);
        return;
    }

    QStyleOptionToolButton option;
    initStyleOption(&option);

    QTransform tf;

    if (orientation() == Qt::Vertical) {
        tf = tf.rotate(90);
    } else {
        tf = tf.rotate(-90);
    }

    option.icon = icon();
    option.rect.setSize(QSize(option.rect.height(), option.rect.width()));

    QPixmap pix(160, 160);

    QStylePainter painter(&pix, this);
    painter.fillRect(pix.rect(), option.palette.background());
    painter.drawComplexControl(QStyle::CC_ToolButton, option);
    painter.end();

    QPainter p(this);

    if (orientation() == Qt::Vertical) {
        p.translate(0, height());
        p.rotate(-90);
    } else {
        p.translate(width(), 0);
        p.rotate(90);
    }

    p.drawPixmap(0, 0, pix);
}

void
DockButton::setAutoHide(bool hideable)
{
    _d->isAutoHide = hideable;
}

void
DockButton::setDockWidget(Aki::DockWidget* dock)
{
    _d->dockWidget = dock;
    connect(dock, SIGNAL(mouseLeave()),
            SIGNAL(mouseLeave()));
    setIcon(dock->icon());
    setText(dock->title());
}

void
DockButton::setOrientation(Qt::Orientation orientation)
{
    _d->orientation = orientation;
}

QSize
DockButton::sizeHint() const
{
    ensurePolished();

    QStyleOptionToolButton option;
    initStyleOption(&option);

    QFontMetrics fm = fontMetrics();
    const int charWidth = fm.width(QLatin1Char('x'));

    QSize textSize;

    if (toolButtonStyle() != Qt::ToolButtonIconOnly) {
        textSize = fm.size(Qt::TextShowMnemonic, option.text);
        textSize.rwidth() += 2 * charWidth;
    }

    const int spacing = 2;
    int iconWidth = 0;
    int iconHeight = 0;

    if (toolButtonStyle() != Qt::ToolButtonTextOnly) {
        if (orientation() == Qt::Vertical) {
            iconWidth = option.iconSize.height();
            iconHeight = option.iconSize.width();
        } else {
            iconWidth = option.iconSize.width();
            iconHeight = option.iconSize.height();
        }
    }

    int width = 4 + textSize.width() + iconWidth + spacing;
    int height = 4 + qMax(textSize.height(), iconHeight) + spacing;

    if (orientation() == Qt::Vertical) {
        return QSize(height, width);
    }

    return QSize(width, height);
}
