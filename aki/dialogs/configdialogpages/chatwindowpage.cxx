#include "chatwindowpage.hpp"
#include "aki.hpp"
#include "settings.h"
using namespace Aki;

ChatWindowPage::ChatWindowPage(QWidget* parent)
    : Aki::ISettingsPage(i18n("Chat Window"), Aki::Settings::self(),
                         "view-choose", i18n("Configure Chat Window Settings"),
                         parent)
{
    setupUi(this);
}

ChatWindowPage::~ChatWindowPage()
{
}

bool
ChatWindowPage::hasChanged() const
{
    return false;
}

void
ChatWindowPage::updateSettings()
{
}
