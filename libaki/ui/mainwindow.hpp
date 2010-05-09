#ifndef AKI_MAINWINDOW_HPP
#define AKI_MAINWINDOW_HPP

#include "../libaki_export.hpp"
#include <KDE/KParts/MainWindow>

class KTabBar;
class QMenu;

namespace Aki
{
class DockWidget;
class View;
class ViewTabBar;
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
     * Gets the top right corner widget.
     *
     * @return Corner widget.
     */
    Aki::ViewTabBar* topRightCorner();
    /**
     * Gets the top right corner widget.
     *
     * @return Corner widget.
     */
    Aki::ViewTabBar* topRightCorner() const;
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
    /**
     * Sets the @p view widget that is used for viewing servers or
     * custom pages created by the user.
     *
     * @param view Main view widget.
     */
    void setView(Aki::View* view);
    Aki::View* view();
    Aki::View* view() const;
    
protected:
    virtual QMenu* createPopupMenu();
    /**
     * Sets the top right corner's @p widget tab bar.
     */
    void setTopRightCorner(Aki::ViewTabBar* widget);
private:
    friend class View;
    friend class MainWindowPrivate;
    QScopedPointer<MainWindowPrivate> _d;
}; // End of class MainWindow.
} // End of namespace Aki.

#endif // AKI_MAINWINDOW_HPP
