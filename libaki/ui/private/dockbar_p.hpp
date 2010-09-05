#ifndef AKI_DOCKBAR_P_HPP
#define AKI_DOCKBAR_P_HPP

#include <QtGui/QWidget>

class QAction;
class QSignalMapper;

namespace Aki
{
class DockBar;
class DockButton;
class DockWidget;
class DockBarPrivate
{
public:
    explicit DockBarPrivate(Aki::DockBar* qq);
    void buttonTriggered(QAction* action);
    void createAction(Aki::DockButton* button);
    void dockAutoHideStateChanged(Aki::DockWidget* dock, bool checked);
    void dockHide(QWidget* dock);
    void dockShow(QWidget* dock);
public:
    QSignalMapper* hideDockMapper;
    QSignalMapper* showDockMapper;
    Qt::ToolBarArea area;
private:
    Aki::DockBar* _q;
}; // End of class DockBarPrivate.
} // End of namespace Aki.

#endif // AKI_DOCKBAR_P_HPP
