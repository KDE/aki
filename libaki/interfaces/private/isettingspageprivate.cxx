#include "isettingspageprivate.hpp"
#include <KDE/KConfigSkeleton>
using namespace Aki;

ISettingsPagePrivate::ISettingsPagePrivate(Aki::ISettingsPage* qq)
    : QObject(),
    name(QString()),
    icon(QString()),
    header(QString()),
    config(0),
    _q(qq)
{
}
