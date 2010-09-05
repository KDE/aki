#ifndef AKI_MAINWINDOWPRIVATE_HPP
#define AKI_MAINWINDOWPRIVATE_HPP

#include <KDE/KTabBar>
#include <QtCore/QObject>

namespace Aki
{
class DockBar;
class DockWidget;
class MainWindow;
class View;
class MainWindowPrivate : public QObject
{
    Q_OBJECT
public:
    explicit MainWindowPrivate(Aki::MainWindow* mainWindow);
    void addDockBar(Qt::ToolBarArea area);
    void dockAdded(Aki::DockWidget* dockWidget);
    void dockAutoHideStateChanged(Aki::DockWidget* dockWidget, bool checked);
    void dockRemoved(Aki::DockWidget* dockWidget);
    void dockHide(Aki::DockWidget* dockWidget);
    void dockShow(Aki::DockWidget* dockWidget);
public:
    QList<Aki::DockBar*> dockBars;
    Aki::View* view;
private:
    Aki::MainWindow* _q;
}; // End of class MainWindowPrivate;
} // End of namespace Aki.

#endif // AKI_MAINWINDOWPRIVATE_HPP
