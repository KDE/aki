#include "mainwindow.hpp"
#include "private/mainwindow_p.hpp"
#include "ui/dockbar.hpp"
#include <KDE/KDebug>
#include <KDE/KToolBar>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
using namespace Aki;

MainWindow::MainWindow()
    : KParts::MainWindow()
{
    _d.reset(new MainWindowPrivate(this));

    _d->addDockBar(Qt::TopToolBarArea);
    _d->addDockBar(Qt::LeftToolBarArea);
    _d->addDockBar(Qt::RightToolBarArea);
    _d->addDockBar(Qt::BottomToolBarArea);
}

MainWindow::~MainWindow()
{
}

QMenu*
MainWindow::createPopupMenu()
{
    return 0;
}

QList<Aki::DockBar*>
MainWindow::dockToolBars() const
{
    return _d->dockBars;
}

#include "ui/mainwindow.moc"
