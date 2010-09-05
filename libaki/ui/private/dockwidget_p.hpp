#ifndef AKI_DOCKWIDGET_P_HPP
#define AKI_DOCKWIDGET_P_HPP

#include <KDE/KIcon>

class QAction;
class QMenu;

namespace Aki
{
class DockBar;
class DockButton;
class DockWidget;
class DockWidgetPrivate
{
public:
    explicit DockWidgetPrivate(Aki::DockWidget* qq);
    void autoHideToggled(bool state);
    void dockCustomContextMenuRequested();
    void initialise();
public:
    Aki::DockBar* dockBar;
    Aki::DockButton* dockButton;
    KIcon icon;
    QMenu* contextMenu;
    QAction* autoHideAction;
private:
    void setupActions();
private:
    Aki::DockWidget* _q;
}; // End of class DockWidgetPrivate.
} // End of namespace Aki.

#endif // AKI_DOCKWIDGET_P_HPP
