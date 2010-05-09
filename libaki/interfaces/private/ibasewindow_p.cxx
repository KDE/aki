#include "ibasewindow_p.hpp"
#include "interfaces/ibasewindow.hpp"
using namespace Aki;

IBaseWindowPrivate::IBaseWindowPrivate(IBaseWindow* qq)
    : QObject(),
    icon(KIcon()),
    title(QString()),
    tabColor(Aki::IBaseWindow::Default),
    type(Aki::IBaseWindow::OtherWindow),
    _q(qq)
{
}
