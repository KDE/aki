#include "ibasewindow_p.hpp"
#include "interfaces/ibasewindow.hpp"
using namespace Aki;

IBaseWindowPrivate::IBaseWindowPrivate(IBaseWindow* qq)
    : icon(KIcon()),
    title(QString()),
    tabColor(Aki::IBaseWindow::Default),
    type(Aki::IBaseWindow::OtherWindow),
    _q(qq)
{
}
