#ifndef AKI_DOCKBAR_HPP
#define AKI_DOCKBAR_HPP

#include "aki.hpp"
#include <KDE/KToolBar>

class QAction;

namespace Aki
{
class DockButton;
class DockBarPrivate;
class DockWidget;
class LIBAKI_EXPORT DockBar : public QToolBar
{
    Q_OBJECT
public:
    explicit DockBar(QWidget* parent = 0);
    ~DockBar();
    void addDockWidget(Aki::DockWidget* dock);
    Aki::DockButton* buttonForAction(QAction* action);
    Aki::DockButton* buttonForDockWidget(Aki::DockWidget* dock);
    void setArea(Qt::ToolBarArea area);
    Qt::ToolBarArea area() const;
Q_SIGNALS:
    void dockAdded(Aki::DockWidget* dock);
    void dockHide(Aki::DockWidget* dock);
    void dockRemoved(Aki::DockWidget* dock);
    void dockShow(Aki::DockWidget* dock);
    void dockAutoHideStateChanged(Aki::DockWidget* dock, bool checked);
private:
    Q_PRIVATE_SLOT(_d, void buttonTriggered(QAction* action))
    Q_PRIVATE_SLOT(_d, void dockHide(QWidget* dock))
    Q_PRIVATE_SLOT(_d, void dockShow(QWidget* dock))
    Q_PRIVATE_SLOT(_d, void dockAutoHideStateChanged(Aki::DockWidget* dockWidget, bool checked))
private:
    AKI_DECLARE_PRIVATE(DockBar);
}; // End of class DockBar.
} // End of namespace Aki.

#endif // AKI_DOCKBAR_HPP
