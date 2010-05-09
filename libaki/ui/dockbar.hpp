#ifndef AKI_DOCKBAR_HPP
#define AKI_DOCKBAR_HPP

#include "../libaki_export.hpp"
#include <KToolBar>

class QAction;
namespace Aki
{
class DockButton;
class DockWidget;
class DockBarPrivate;
/**
 * Dockable widgets for easy viewing and auto hiding of the docks.
 *
 * DockBar is just a QToolBar with added features of managing docks.
 */
class LIBAKI_EXPORT DockBar : public KToolBar
{
    Q_OBJECT
public:
    /**
     * Creates a new DockBar object.
     *
     * @param parent Parent of the object.
     */
    explicit DockBar(QWidget* parent = 0);
    /**
     * Destroys the object.
     */
    virtual ~DockBar();
    /**
     * Adds a @p dock to the DockBar.
     * @note Takes ownership of the dock.
     *
     * @param dock DockWidget.
     */
    void addDock(Aki::DockWidget* dock);
    /**
     * Removes a @p dock from the DockBar.
     * @note Takes ownership of the @p dock.
     *
     * @param dock Dock to be removes.
     */
    void removeDock(Aki::DockWidget* dock);
    /**
     * Set the area where the dock will be located.
     *
     * @param area Location of DockBar.
     */
    void setDockArea(Qt::ToolBarArea area);
    /**
     * Get the location of the DockBar.
     *
     * @return DockBar location.
     */
    Qt::ToolBarAreas area() const;
    /**
     * Gets the DockButton that holds this @p action.
     *
     * @param action Action to get DockButton for.
     *
     * @return DockButton for the action.
     * @return 0 if no button is found for this action.
     */
    Aki::DockButton* buttonForAction(QAction* action);
    /**
     * Gets the DockButton that holds this @p dock.
     *
     * @param dock DockWidget to get DockButton for.
     *
     * @return DockButton that holds this dock
     * @return 0 if no button is found for this dock.
     */
    Aki::DockButton* buttonForDock(Aki::DockWidget* dock);
Q_SIGNALS:
    /**
     * This signal is emitted when the @p dock is to be shown.
     *
     * @param dock DockWidget that needs to be shown.
     */
    void dockShow(Aki::DockWidget* dock);
    /**
     * This signal is emitted when the @p dock is the be hidden.
     *
     * @param dock DockWidget that needs to be hidden.
     */
    void dockHide(Aki::DockWidget* dock);
    /**
     * This signal is emitted when the @p dock is to be added
     * to the MainWindow.
     *
     * @param dock DockWidget that needs to be added to MainWindow.
     */
    void dockAdded(Aki::DockWidget* dock);
    /**
     * This signal is emitted when the @p dock is to be removed
     * from the MainWindow.
     *
     * @param dock DockWidget that needs to be removed from MainWindow.
     */
    void dockRemoved(Aki::DockWidget* dock);
    void dockAutoHideStateChanged(Aki::DockWidget*, bool checked);
private:
    friend class DockBarPrivate;
    QScopedPointer<DockBarPrivate> _d;
}; // End of class DockBar.
} // End of namespace Aki.

#endif // AKI_DOCKBAR_HPP
