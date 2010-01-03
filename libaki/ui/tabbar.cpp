/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "tabbar.h"
#include "ui/channelview.h"
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
using namespace Aki;

namespace Aki
{
class TabBarPrivate
{
public:
    TabBarPrivate()
        : dragEnable(true)
    {
    }

    bool dragEnable;
}; // End of class TabBarPrivate.
} // End of namespace Aki.

TabBar::TabBar(QWidget *parent)
    : KTabBar(parent),
    d(new TabBarPrivate)
{
    setAcceptDrops(true);
}

TabBar::~TabBar()
{
    delete d;
}

void
TabBar::mousePressEvent(QMouseEvent *event)
{
    if (d->dragEnable) {
        if (event->button() == Qt::MidButton) {
            QDrag *drag = new QDrag(this);
            QMimeData *data = new QMimeData();

            QByteArray encoded;
            QDataStream stream(&encoded, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_4_6);

            Aki::ChannelView *view = qobject_cast<Aki::ChannelView*>(parentWidget());
            stream << reinterpret_cast<quint64>(view->widget(tabAt(event->pos())));
            data->setData("application/aki-tab", encoded);
            drag->setMimeData(data);
            drag->start();
            return;
        }
    }

    KTabBar::mousePressEvent(event);
}

void
TabBar::wheelEvent(QWheelEvent *event)
{
    int current = currentIndex();
    const int cnt = count() - 1;

    if (event->delta() > 0) {
        if (current <= 0) {
            current = cnt;
            setCurrentIndex(current);
        } else {
            --current;
            setCurrentIndex(current);
        }
    } else if (event->delta() < 0) {
        if (current >= cnt) {
            current = 0;
            setCurrentIndex(0);
        } else {
            ++current;
            setCurrentIndex(current);
        }
    }
    KTabBar::wheelEvent(event);
}

void
TabBar::setDragMode(bool state)
{
    d->dragEnable = state;
}
