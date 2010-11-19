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

#include "serverviewtab.hpp"
#include "private/serverviewtab_p.hpp"
#include "ui/channelcontainer.hpp"
#include "ui/channelview.hpp"
#include "ui/channelwidget.hpp"
#include "irc/socket.hpp"
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
using namespace Aki;
using namespace Aki::Irc;

ServerViewTab::ServerViewTab(const QString& name, Aki::Irc::Socket* socket, QWidget* parent)
    : Aki::IBaseWindow(name, Aki::IBaseWindow::ServerWindow, parent)
{
    _d.reset(new Aki::ServerViewTabPrivate(this));
    _d->socket = socket;

    _d->channelContainer = new Aki::ChannelContainer;

    Aki::ChannelView* view = new Aki::ChannelView;
    view->addTab(new Aki::ChannelWidget, i18n("test"));
    _d->channelContainer->addChannelView(view);

    Aki::ChannelView* view2 = new Aki::ChannelView;
    view2->addTab(new Aki::ChannelWidget, i18n("test2"));
    _d->channelContainer->addChannelView(view2);

    _d->verticalSplitter = new QSplitter(Qt::Vertical);
    _d->verticalSplitter->addWidget(_d->channelContainer);

    Aki::ChannelContainer* cont = new Aki::ChannelContainer;

    Aki::ChannelView* view3 = new Aki::ChannelView;
    view3->addTab(new Aki::ChannelWidget, i18n("test3"));
    cont->addChannelView(view3);

    Aki::ChannelView* view4 = new Aki::ChannelView;
    view4->addTab(new Aki::ChannelWidget, i18n("test4"));
    cont->addChannelView(view4);

    _d->verticalSplitter->addWidget(cont);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_d->verticalSplitter);
    setLayout(layout);
}

ServerViewTab::~ServerViewTab()
{
}

Aki::IBaseWindow*
ServerViewTab::currentFocusedChannel() const
{
    return 0;
}

Aki::IBaseWindow*
ServerViewTab::findChannel(const QString& name) const
{
    Q_UNUSED(name);
    for (int i = 0; i < _d->verticalSplitter->count(); ++i) {
        Aki::ChannelContainer* tmpContainer =
            qobject_cast<Aki::ChannelContainer*>(_d->verticalSplitter->widget(i));
        Q_UNUSED(tmpContainer);
    }

    return 0;
}

bool
ServerViewTab::hasInputFocus() const
{
    return false;
}

QStringList
ServerViewTab::rejoinChannelList() const
{
    return QStringList();
}

QWidget*
ServerViewTab::widget()
{
    return 0;
}
