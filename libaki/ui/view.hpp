#ifndef AKI_VIEW_HPP
#define AKI_VIEW_HPP

#include "libaki_export.hpp"
#include <KDE/KIcon>
#include <QtGui/QWidget>

namespace Aki
{
class IBaseWindow;
class ViewPrivate;
/**
 * Handles the displaying of the Servers in the MainWindow.
 */
class LIBAKI_EXPORT View : public QWidget
{
    Q_OBJECT
public:
    /**
     * Creates the View object.
     *
     * @param parent Parent of the object.
     */
    explicit View(QWidget* parent = 0);
    /**
     * Destroys the object.
     */
    virtual ~View();
    /**
     * Adds the given @p window to the View. Ownership is transferred.
     *
     * If the View is empty before this function is called it will be set
     * as the current window.
     *
     * @param window Window to be added.
     */
    void addWindow(Aki::IBaseWindow* window);
    /**
     * Inserts the @p window in to the View, this transfer ownership of the @p window
     * to the View. If the @p index is out of range it will be append.
     *
     * If the View was empty before this function is called, the @p window becomes the current
     * widget.
     *
     * @param index Index to be inserted at.
     * @param window Window to be inserted.
     */
    void insertWindow(int index, Aki::IBaseWindow* window);
    /**
     * Removes the @p window from the View.
     * @note The ownership of @p window remains the same. The widget
     * is not deleted, but simple removed from the view, causing it to hide.
     *
     * @param window Window to be removed.
     */
    void removeWindow(Aki::IBaseWindow* window);
    /**
     * Gets the window at the specified @p index
     *
     * @param index Window at the current index; 0 if there is no such Window.
     */
    Aki::IBaseWindow* window(int index) const;
    /**
     * Get the number of Windows in this view.
     *
     * @return Number of views.
     */
    int count() const;
    /**
     * Gets the current Window.
     *
     * @returns The current Window; 0 if there are no windows.
     */
    Aki::IBaseWindow* currentWindow() const;
    /**
     * Sets the current index to the specified @p index.
     *
     * @param index Index to change the view to.
     */
    void setCurrentIndex(int index);
    /**
     * Get the index of the current @p window.
     *
     * @param window IBaseWindow to get index of.
     * @return index if window is apart of View, or -1 otherwise.
     */
    int indexOf(Aki::IBaseWindow* window);
public Q_SLOTS:
    /**
     * Changes the position of the views object to match the
     * top right corner tabbar.
     *
     * @param oldIndex Old index the view was in.
     * @param newIndex New index the view will be.
     */
    virtual void viewPositionChanged(int oldIndex, int newIndex);
    /**
     * View that is being removed and no longer of any use.
     *
     * @param index Index of the view to be removed.
     */
    virtual void viewPositionClose(int index);
    /**
     * View that is being requested to be displayed.
     *
     * @param index Index of the view to be displayed.
     */
    virtual void viewCurrentChanged(int index);
    /**
     *
     */
    void setCurrentWindow(Aki::IBaseWindow* window);
private:
    friend class ViewPrivate;
    QScopedPointer<ViewPrivate> _d;
}; // End of class View.
} // End of namespace View.

#endif // AKI_VIEW_HPP
