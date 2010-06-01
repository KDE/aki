#include "generalpage.hpp"
#include "aki.hpp"
#include "settings.h"
using namespace Aki;

GeneralPage::GeneralPage(QWidget* parent)
    : Aki::ISettingsPage(i18nc("General options for configuration", "General"),
                         Aki::Settings::self(), "configure", i18n("Configure General Settings"),
                         parent)
{
    setupUi(this);
}

GeneralPage::~GeneralPage()
{
}

bool
GeneralPage::hasChanged() const
{
    return false;
}

void
GeneralPage::updateSettings()
{
}
