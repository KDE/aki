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
class ViewTabBar;
class MainWindowPrivate : public QObject
{
    Q_OBJECT
public:
    explicit MainWindowPrivate(Aki::MainWindow* mainWindow);
    void setTopRightCorner(Aki::ViewTabBar* widget);
    void addDockBar(Qt::ToolBarArea area);
    Qt::ToolBarArea toToolBarArea(Qt::DockWidgetArea area);
    void dockLocationChanged(Qt::DockWidgetArea area);
public Q_SLOTS:
    void dockAdded(Aki::DockWidget* dock);
    void dockRemoved(Aki::DockWidget* dock);
    void dockShow(Aki::DockWidget* dock);
    void dockHide(Aki::DockWidget* dock);
    void dockAutoHideStateChanged(Aki::DockWidget* dock, bool checked);
public:
    Aki::ViewTabBar* cornerWidget;
    Aki::View* view;
private:
    Aki::MainWindow* _q;
}; // End of class MainWindowPrivate;
} // End of namespace Aki.

#endif // AKI_MAINWINDOWPRIVATE_HPP
