#ifndef DOCKBUTTON_P_HPP
#define DOCKBUTTON_P_HPP

#include <QtCore/QObject>

namespace Aki
{
class DockButton;
class DockWidget;
class DockButtonPrivate : public QObject
{
    Q_OBJECT
public:
    explicit DockButtonPrivate(Aki::DockButton* qq);
public:
    Aki::DockWidget* dock;
    Qt::Orientation orientation;
    bool hideable;
private:
    Aki::DockButton* _q;
}; // End of class DockButtonPrivate.
} // End of namespace Aki.

#endif // DOCKBUTTON_P_HPP
