#include "view.hpp"
#include "interfaces/ibasewindow.hpp"
#include "private/view_p.hpp"
#include "ui/mainwindow.hpp"
#include "ui/viewtabbar.hpp"
#include <KDE/KDebug>
#include <QtGui/QStackedWidget>
using namespace Aki;

View::View(QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::ViewPrivate(this));
    _d->widgetStack = new QStackedWidget(this);
}

View::~View()
{
    const int count = _d->widgetStack->count();
    for (int i = 0; i < count; ++i) {
        _d->widgetStack->removeWidget(_d->widgetStack->widget(i));
    }
}

void
View::viewPositionChanged(int oldIndex, int newIndex)
{
    QWidget* old = _d->widgetStack->widget(oldIndex);
    _d->widgetStack->removeWidget(old);
    _d->widgetStack->insertWidget(newIndex, old);
}

void
View::viewPositionClose(int index)
{
    QWidget* widget = _d->widgetStack->widget(index);
    _d->widgetStack->removeWidget(widget);
}

void
View::viewCurrentChanged(int index)
{
    _d->widgetStack->setCurrentIndex(index);
}

void
View::addWindow(Aki::IBaseWindow* window)
{
    Q_ASSERT(window);
    _d->widgetStack->addWidget(window);
    _d->mainWindow()->topRightCorner()->addTab(window);
}

void
View::insertWindow(int index, Aki::IBaseWindow* window)
{
    Q_ASSERT(window);
    if (index > count()) {
        _d->widgetStack->addWidget(window);
        _d->mainWindow()->topRightCorner()->addTab(window);
    } else if (index < 0) {
        _d->widgetStack->insertWidget(0, window);
        _d->mainWindow()->topRightCorner()->prependTab(window);
    } else {
        _d->widgetStack->insertWidget(index, window);
        _d->mainWindow()->topRightCorner()->insertTab(index, window);
    }
}

void
View::removeWindow(Aki::IBaseWindow* window)
{
    _d->widgetStack->removeWidget(window);
}

Aki::IBaseWindow*
View::window(int index) const
{
    return qobject_cast<Aki::IBaseWindow*>(_d->widgetStack->widget(index));
}

int
View::count() const
{
    return _d->widgetStack->count();
}

Aki::IBaseWindow*
View::currentWindow() const
{
    return window(_d->widgetStack->currentIndex());
}

int
View::indexOf(IBaseWindow* window)
{
    return _d->widgetStack->indexOf(window);
}

void
View::setCurrentIndex(int index)
{
    _d->widgetStack->setCurrentIndex(index);
}

void
View::setCurrentWindow(Aki::IBaseWindow* window)
{
    _d->widgetStack->setCurrentWidget(window);
}
