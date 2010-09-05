#include "dockbutton_p.hpp"
#include "ui/dockbutton.hpp"
#include "ui/dockwidget.hpp"
using namespace Aki;

DockButtonPrivate::DockButtonPrivate(Aki::DockButton* qq)
    : dockWidget(0),
    orientation(Qt::Horizontal),
    isAutoHide(false),
    isButtonClicked(false),
    _q(qq)
{
}
