#include "isettingspage.hpp"
#include "private/isettingspageprivate.hpp"
#include <KDE/KConfigSkeleton>
using namespace Aki;

ISettingsPage::ISettingsPage(const QString& name, KConfigSkeleton* config,
                             const QString& icon, const QString& header,
                             QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::ISettingsPagePrivate(this));
    _d->name = name;
    _d->config = config;
    _d->icon = icon;
    _d->header = header;
}

ISettingsPage::~ISettingsPage()
{
}

KConfigSkeleton*
ISettingsPage::config()
{
    return _d->config;
}

QString
ISettingsPage::header() const
{
    return _d->header;
}

QString
ISettingsPage::icon() const
{
    return _d->icon;
}

QString
ISettingsPage::name() const
{
    return _d->name;
}

void
ISettingsPage::updateWidgets()
{
}
