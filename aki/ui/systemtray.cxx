#include "systemtray.hpp"
#include "aki.hpp"
using namespace Aki;

SystemTray::SystemTray(QObject* parent)
    : KStatusNotifierItem(parent)
{
    setStatus(KStatusNotifierItem::Active);
    setIconByName("aki");
}

SystemTray::~SystemTray()
{
}
