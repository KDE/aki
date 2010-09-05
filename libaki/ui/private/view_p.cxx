#include "view_p.hpp"
#include "interfaces/ibasewindow.hpp"
#include "ui/mainwindow.hpp"
#include "ui/tabbar.hpp"
#include "ui/view.hpp"
#include <KDE/KMenuBar>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
using namespace Aki;

ViewPrivate::ViewPrivate(Aki::View* qq)
    : QObject(),
    tabWidget(0),
    tabBar(0),
    layout(0),
    mainWindow(0),
    _q(qq)
{
}

void
ViewPrivate::currentChanged(int index)
{
    tabWidget->setCurrentIndex(index);
}

void
ViewPrivate::moveTab(int oldIndex, int newIndex)
{
    QWidget* widget = tabWidget->widget(oldIndex);
    tabWidget->removeWidget(widget);
    tabWidget->insertWidget(newIndex, widget);
}

void
ViewPrivate::tabCloseRequested(int index)
{
    QWidget* currentWidget = tabWidget->widget(index);
    Aki::IBaseWindow* window = static_cast<Aki::IBaseWindow*>(currentWidget);
    if (window) {
        tabBar->removeTab(index);
        tabWidget->removeWidget(window);
        delete window;

        mainWindow->menuBar()->setCornerWidget(tabBar);
    }
}

void
ViewPrivate::wheelDelta(int delta)
{
    Q_UNUSED(delta);
}
