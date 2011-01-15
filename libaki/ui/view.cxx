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

#include "view.hpp"
#include "interfaces/ibasewindow.hpp"
#include "private/view_p.hpp"
#include "ui/mainwindow.hpp"
#include "ui/tabbar.hpp"
#include <KDE/KMenuBar>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
using namespace Aki;

View::View(Aki::MainWindow* window, QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::ViewPrivate(this));

    _d->mainWindow = window;
    _d->tabWidget = new QStackedWidget(this);

    _d->tabBar = new Aki::TabBar(this);

    connect(_d->tabBar, SIGNAL(tabCloseRequested(int)),
            SLOT(tabCloseRequested(int)));
    connect(_d->tabBar, SIGNAL(moveTab(int,int)),
            SLOT(moveTab(int, int)));
    connect(_d->tabBar, SIGNAL(currentChanged(int)),
            SLOT(currentChanged(int)));
    connect(_d->tabBar, SIGNAL(wheelDelta(int)),
            SLOT(wheelDelta(int)));

    _d->layout = new QVBoxLayout(this);
#ifdef Q_OS_MAC
    _d->layout->addWidget(_d->tabBar);
#else
    _d->mainWindow->menuBar()->setCornerWidget(_d->tabBar);
#endif

    _d->layout->addWidget(_d->tabWidget);
    setLayout(_d->layout);
}

View::~View()
{
    const int count = _d->tabWidget->count();
    for (int i = 0; i < count; ++i) {
        QWidget* widget = _d->tabWidget->widget(i);
        _d->tabWidget->removeWidget(widget);
        delete widget;
    }
}

void
View::addWindow(Aki::IBaseWindow* window)
{
    Q_ASSERT(window);

    _d->tabWidget->addWidget(window);
    _d->tabBar->addTab(window->icon(), window->title());

    if (_d->tabWidget->count() == 1) {
        _d->tabWidget->setCurrentWidget(window);
    }
}

int
View::count() const
{
    return _d->tabWidget->count();
}

int
View::currentIndex() const
{
    return _d->tabWidget->currentIndex();
}

Aki::IBaseWindow*
View::currentWindow() const
{
    return static_cast<Aki::IBaseWindow*>(_d->tabWidget->currentWidget());
}

Aki::IBaseWindow*
View::findWindow(const QString& name) const
{
    if (name.isEmpty() || name.isNull()) {
        return 0;
    }

    const int count = _d->tabWidget->count();
    for (int i = 0; i < count; ++i) {
        Aki::IBaseWindow* window =
            static_cast<Aki::IBaseWindow*>(_d->tabWidget->widget(i));
        if (window->title().toLower() == name.toLower()) {
            return window;
        }
    }

    return 0;
}

int
View::indexOf(Aki::IBaseWindow* window) const
{
    return _d->tabWidget->indexOf(window);
}

void
View::insertWindow(int index, Aki::IBaseWindow* window)
{
    Q_ASSERT(window);

    if (index < 0) {
        prependWindow(window);
        return;
    } else if (index > _d->tabWidget->count()) {
        addWindow(window);
        return;
    }

    _d->tabWidget->insertWidget(index, window);
    _d->tabBar->insertTab(index, window->icon(), window->title());

    if (_d->tabWidget->count() == 1) {
        _d->tabWidget->setCurrentWidget(window);
    }
}

void
View::prependWindow(Aki::IBaseWindow* window)
{
    Q_ASSERT(window);

    _d->tabWidget->insertWidget(0, window);
    _d->tabBar->insertTab(0, window->icon(), window->title());

    if (_d->tabWidget->count() == 1) {
        _d->tabWidget->setCurrentWidget(window);
    }
}

void
View::removeWindow(Aki::IBaseWindow* window)
{
    _d->tabWidget->removeWidget(window);
    _d->tabBar->removeTab(indexOf(window));
    delete window;
}

void
View::removeWindow(int index)
{
    Aki::IBaseWindow* window = static_cast<Aki::IBaseWindow*>(_d->tabWidget->widget(index));
    _d->tabWidget->removeWidget(window);
    _d->tabBar->removeTab(index);
    delete window;
}

KTabBar*
View::tabBar() const
{
    return _d->tabBar;
}

Aki::IBaseWindow*
View::window(int index) const
{
    return static_cast<Aki::IBaseWindow*>(_d->tabWidget->widget(index));
}

#include "ui/view.moc"

