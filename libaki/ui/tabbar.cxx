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

#include "tabbar.hpp"
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QMimeData>
#include <QtGui/QDrag>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
using namespace Aki;

TabBar::TabBar(QWidget* parent)
    : KTabBar(parent)
{
    setSelectionBehaviorOnRemove(KTabBar::SelectLeftTab);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setTabsClosable(true);
    setMovable(true);
}

TabBar::~TabBar()
{
}

void TabBar::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MidButton) {
        QDrag* drag = new QDrag(this);
        QMimeData* data = new QMimeData;

        QByteArray encoded;
        QDataStream stream(&encoded, QIODevice::WriteOnly);

        data->setData("application/aki-tab", encoded);
        drag->setMimeData(data);
        drag->start();
        return;
    }
    KTabBar::mousePressEvent(event);
}

void TabBar::wheelEvent(QWheelEvent* event)
{
    if (count() < 2) {
        return;
    }

    int page = currentIndex();
    if (event->delta() < 0) {
        page = (page + 1) % count();
    } else {
        page--;
        if (page < 0) {
            page = count() - 1;
        }
    }
    setCurrentIndex(page);
    //KTabBar::wheelEvent(event);
}
