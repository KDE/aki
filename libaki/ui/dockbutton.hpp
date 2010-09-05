#ifndef AKI_DOCKBUTTON_HPP
#define AKI_DOCKBUTTON_HPP

#include "aki.hpp"
#include <QtGui/QToolButton>

class QPaintEvent;
namespace Aki
{
class DockButtonPrivate;
class DockWidget;
class LIBAKI_EXPORT DockButton : public QToolButton
{
    Q_OBJECT
public:
    explicit DockButton(QWidget* parent = 0);
    ~DockButton();
    Aki::DockWidget* dockWidget() const;
    bool isAutoHide() const;
    Qt::Orientation orientation() const;
    void setAutoHide(bool hideable);
    void setDockWidget(Aki::DockWidget* dock);
    void setOrientation(Qt::Orientation orientation);
    virtual QSize sizeHint() const;
Q_SIGNALS:
    void mouseEnter();
    void mouseLeave();
protected:
    virtual void paintEvent(QPaintEvent* event);
private:
    AKI_DECLARE_PRIVATE(DockButton);
}; // End of class DockButton.
} // End of namespace Aki.

#endif // AKI_DOCKBUTTON_HPP
