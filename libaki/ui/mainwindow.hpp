#ifndef AKI_MAINWINDOW_HPP
#define AKI_MAINWINDOW_HPP

#include "aki.hpp"
#include <KDE/KParts/MainWindow>

class KTabBar;
class QMenu;

namespace Aki
{
class DockBar;
class DockWidget;
class View;
class MainWindowPrivate;
/**
 * Reimplementation of a MainWindow that has auto hiding docks.
 *
 * Inherits KParts::MainWindow for easy implementation of plugins
 * to merge menus and/or ToolBars of the plugins in to the MainWindow.
 */
class LIBAKI_EXPORT MainWindow : public KParts::MainWindow
{
    Q_OBJECT
public:
    /**
     * Creates a new MainWindow object.
     */
    MainWindow();
    /**
     * Destroys the object.
     */
    virtual ~MainWindow();
    /**
     * Adds a @p dock to a specific @p area in the MainWindow.
     *
     * @param area Location to place the dock.
     * @param dock DockWidget to be added.
     */
    void addDock(Qt::ToolBarArea area, Aki::DockWidget* dock);
    /**
     * Removes a @p dock from the MainWindow.
     * @note You are responsible for deleting the dock.
     *
     * @param dock DockWidget to be removes.
     */
    void removeDock(Aki::DockWidget* dock);
    QList<Aki::DockBar*> dockToolBars() const;
protected:
    virtual QMenu* createPopupMenu();
private:
    Q_PRIVATE_SLOT(_d, void dockAdded(Aki::DockWidget* dockWidget))
    Q_PRIVATE_SLOT(_d, void dockRemoved(Aki::DockWidget* dockWidget))
    Q_PRIVATE_SLOT(_d, void dockShow(Aki::DockWidget* dockWidget))
    Q_PRIVATE_SLOT(_d, void dockHide(Aki::DockWidget* dockWidget))
    Q_PRIVATE_SLOT(_d, void dockAutoHideStateChanged(Aki::DockWidget* dockWidget,
                                                     bool checked))
private:
    friend class View;
    AKI_DECLARE_PRIVATE(MainWindow)
}; // End of class MainWindow.
} // End of namespace Aki.

#endif // AKI_MAINWINDOW_HPP
