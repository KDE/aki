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

#include "channelcontainer.hpp"
#include "private/channelcontainer_p.hpp"
#include "ui/channelview.hpp"
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
using namespace Aki;

ChannelContainer::ChannelContainer(QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::ChannelContainerPrivate(this));
    _d->horizontalSplitter = new QSplitter(Qt::Horizontal);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_d->horizontalSplitter);
    setLayout(layout);
}

ChannelContainer::~ChannelContainer()
{
}

void
ChannelContainer::addChannelView(Aki::ChannelView* view)
{
    Q_ASSERT(view);
    _d->horizontalSplitter->addWidget(view);
}

Aki::ChannelView*
ChannelContainer::channelView(int index) const
{
    return qobject_cast<Aki::ChannelView*>(_d->horizontalSplitter->widget(index));
}

int
ChannelContainer::count() const
{
    return _d->horizontalSplitter->count();
}

int
ChannelContainer::indexOf(Aki::ChannelView* view) const
{
    const int count = _d->horizontalSplitter->count();
    for (int i = 0; i < count; ++i) {
        Aki::ChannelView* tmp =
            qobject_cast<Aki::ChannelView*>(_d->horizontalSplitter->widget(i));
        if (tmp == view) {
            return i;
        }
    }

    return 0;
}

void
ChannelContainer::insertChannelView(int index, Aki::ChannelView* view)
{
    Q_ASSERT(view);

    if (index <= 0) {
        _d->horizontalSplitter->insertWidget(0, view);
        return;
    } else if (index >= _d->horizontalSplitter->count()) {
        _d->horizontalSplitter->addWidget(view);
        return;
    }

    _d->horizontalSplitter->insertWidget(index, view);
}
