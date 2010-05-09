#ifndef AKI_DOCKBUTTON_H
#define AKI_DOCKBUTTON_H

#include "../libaki_export.hpp"
#include <QtGui/QToolButton>

class QPaintEvent;
namespace Aki
{
class DockWidget;
class DockButtonPrivate;
/**
 * Adds functionality for holding of the dock and also allows
 * rotation of the button.
 */
class LIBAKI_EXPORT DockButton : public QToolButton
{
    Q_OBJECT
public:
    /**
     * Creates a DockButton object.
     *
     * @param parent Paren of the object.
     */
    explicit DockButton(QWidget *parent = 0);
    /**
     * Destroys the object.
     */
    virtual ~DockButton();
    /**
     * Sets the orientation of the button.
     *
     * @param orientation DockButton's orientation.
     */
    void setOrientation(Qt::Orientation orientation);
    /**
     * Gets the orientation of the button.
     *
     * @return DockButton's orientation.
     */
    Qt::Orientation orientation() const;
    /**
     *
     */
    virtual QSize sizeHint() const;
    /**
     * Sets the state of the button if it will be visible if @p hideable
     * is set to true.
     *
     * @param hideable true if the button will be visible; false otherwise.
     */
    void setAutoHide(bool hideable);
    /**
     * Gets the state of the DockButton hideable state.
     *
     * @return true if the button is visible; false otherwise.
     */
    bool isAutoHide() const;
    /**
     * Sets the @p dock that this button will hold.
     *
     * @param dock DockWidget for this button.
     */
    void setDock(Aki::DockWidget* dock);
    /**
     * Gets the dock that this button contains.
     *
     * @return DockWidget object if there is a dock for this button; 0 if
     * there is no button.
     */
    Aki::DockWidget* dock();
    /**
     * Gets the dock that this button contains.
     *
     * @return DockWidget object if there is a dock for this button; 0 if
     * there is no button.
     */
    Aki::DockWidget* dock() const;
Q_SIGNALS:
    void mouseLeave();
protected:
    virtual void paintEvent(QPaintEvent* e);
private:
    friend class DockButtonPrivate;
    QScopedPointer<DockButtonPrivate> _d;
}; // End of class DockButton.
} // End of namespace Aki.

#endif // AKI_DOCKBUTTON_H
