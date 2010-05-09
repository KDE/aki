#ifndef AKI_DOCKWIDGET_HPP
#define AKI_DOCKWIDGET_HPP

#include "../libaki_export.hpp"
#include <KDE/KIcon>
#include <QtGui/QDockWidget>

namespace Aki
{
class DockBar;
class DockWidgetPrivate;
/**
 * Iconified DockWidget and auto hideable.
 */
class LIBAKI_EXPORT DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    /**
     * Creates a DockWidget object.
     *
     * @param parent Parent of the object.
     */
    explicit DockWidget(QWidget* parent = 0);
    /**
     * Creates a DockWidget object with a @p title and a @p icon.
     *
     * @param title Name of the dock.
     * @param icon Icon for the dock.
     * @param parent Parent of the object.
     */
    explicit DockWidget(const QString& title, const KIcon& icon = KIcon(),
                        QWidget* parent = 0);
    /**
     * Destroys the object.
     */
    virtual ~DockWidget();
    /**
     * Sets the @p title of the dock.
     *
     * @param title Title for the dock.
     */
    void setTitle(const QString& title);
    /**
     * Gets the title of the dock.
     *
     * @return DockWidget title.
     */
    QString title() const;
    /**
     * Sets the @p icon for the dock.
     *
     * @param icon Icon for the dock.
     */
    void setIcon(const KIcon& icon);
    /**
     * Gets the icon of the dock.
     *
     * @return DockWidget icon.
     */
    KIcon icon() const;
    /**
     * Sets the @p dockBar that owns this dock.
     *
     * @param dockBar DockWidgets dockbar.
     */
    void setDockBar(Aki::DockBar* dockBar);
    /**
     * Gets the dock's dockbar that this dock belongs to.
     *
     * @return DockWidget's dockbar.
     */
    Aki::DockBar* dockBar();
    /**
     * Gets the dock's dockbar that this dock belongs to.
     *
     * @return DockWidget's dockbar.
     */
    Aki::DockBar* dockBar() const;
protected:
    virtual void leaveEvent(QEvent* e);
Q_SIGNALS:
    /**
     * This signal is emitted when the dock @p title has been changed.
     *
     * @param title New title for this dock.
     */
    void titleChanged(const QString& title);
    /**
     * This signal is emitted when the dock's autohide state has been
     * changed.
     *
     * @param checked true if the dock is now hideable; false otherwise.
     */
    void dockAutoHideStateChanged(bool checked);
    /**
     * Thie signal is emitted when the mouse has left the DockWidget.
     */
    void mouseLeave();
private:
    friend class DockWidgetPrivate;
    QScopedPointer<DockWidgetPrivate> _d;
}; // End of class DockWidget.
} // End of namespace Aki.

#endif // AKI_DOCKWIDGET_HPP
