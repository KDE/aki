#include "dockbutton_p.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
using namespace Aki;

DockButtonPrivate::DockButtonPrivate(DockButton* qq)
    : QObject(),
    dock(0),
    orientation(Qt::Horizontal),
    hideable(false),
    _q(qq)
{
}
