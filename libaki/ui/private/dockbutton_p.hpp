#ifndef AKI_DOCKBUTTON_P_HPP
#define AKI_DOCKBUTTON_P_HPP

#include <QtCore/QString>

namespace Aki
{
class DockButton;
class DockWidget;
class DockButtonPrivate
{
public:
    explicit DockButtonPrivate(Aki::DockButton* qq);
public:
    Aki::DockWidget* dockWidget;
    Qt::Orientation orientation;
    bool isAutoHide;
    bool isButtonClicked;
private:
    Aki::DockButton* _q;
}; // End of class DockButtonPrivate.
} // End of namespace Aki.

#endif // AKI_DOCKBUTTON_P_HPP
