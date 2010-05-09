#include "viewtabbar.hpp"
#include "interfaces/ibasewindow.hpp"
#include "private/viewtabbar_p.hpp"
#include <QtGui/QMenu>
using namespace Aki;

ViewTabBar::ViewTabBar(QWidget* parent)
    : KTabBar(parent)
{
    _d.reset(new Aki::ViewTabBarPrivate(this));
}

ViewTabBar::~ViewTabBar()
{
}

QMenu*
ViewTabBar::createPopupMenu()
{
    _d->contextMenu = new QMenu(this);
    _d->setupContextActions();
    return _d->contextMenu;
}

void
ViewTabBar::addTab(Aki::IBaseWindow* window)
{
    if (window->icon().isNull()) {
        KTabBar::addTab(window->title());
        return;
    }

    KTabBar::addTab(window->icon(), window->title());
}

void
ViewTabBar::insertTab(int index, Aki::IBaseWindow* window)
{
    if (window->icon().isNull()) {
        KTabBar::insertTab(index, window->title());
    }

    KTabBar::insertTab(index, window->icon(), window->title());
}

void
ViewTabBar::prependTab(IBaseWindow* window)
{
    insertTab(0, window);
}
