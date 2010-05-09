#include "view_p.hpp"
#include "ui/mainwindow.hpp"
#include "ui/view.hpp"
#include "ui/viewtabbar.hpp"
#include <KDE/KDebug>
#include <KDE/KLocale>
#include <KDE/KTabBar>
#include <QtGui/QStackedWidget>
using namespace Aki;

ViewPrivate::ViewPrivate(Aki::View* qq)
    : QObject(),
    widgetStack(0),
    _q(qq)
{
}

Aki::MainWindow*
ViewPrivate::mainWindow()
{
    return qobject_cast<Aki::MainWindow*>(_q->parentWidget());
}
