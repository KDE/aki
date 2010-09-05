#include "configurationdialog.hpp"
#include "aki.hpp"
#include "settings.h"
#include "configdialogpages/chatwindowpage.hpp"
#include "configdialogpages/generalpage.hpp"
#include "configdialogpages/pluginpage.hpp"
#include "interfaces/isettingspage.hpp"
using namespace Aki;

ConfigurationDialog::ConfigurationDialog(QWidget* parent)
    : KConfigDialog(parent, "settings", Aki::Settings::self())
{
    setButtons(KConfigDialog::Help | KConfigDialog::Ok |
               KConfigDialog::Cancel | KConfigDialog::Apply);
    setSizeGripEnabled(true);

    addPage(new Aki::GeneralPage);
    addPage(new Aki::ChatWindowPage, false);
    addPage(new Aki::PluginPage);
    resize(QSize(800, 600));
}

ConfigurationDialog::~ConfigurationDialog()
{
}

void
ConfigurationDialog::addPage(Aki::ISettingsPage* page, bool manage)
{
    connect(page, SIGNAL(widgetsModified()),
            SLOT(updateButtons()));

    KPageWidgetItem* item;
    if (manage) {
        item = KConfigDialog::addPage(page, page->config(), page->name(), page->icon(), page->header());
    } else {
        item = KConfigDialog::addPage(page, page->name(), page->icon(), page->header(), manage);
    }
    _pageList[page] = item;
}

bool
ConfigurationDialog::hasChanged()
{
    bool changed;
    foreach (const Aki::ISettingsPage* page, _pageList.keys()) {
        if (page->hasChanged()) {
            changed = true;
        }
    }

    return changed;
}

void
ConfigurationDialog::removePage(Aki::ISettingsPage* page)
{
    QMutableMapIterator<Aki::ISettingsPage*, KPageWidgetItem*> pageIter(_pageList);
    while (pageIter.hasNext()) {
        pageIter.next();

        if (pageIter.key() == page) {
            _pageList.remove(page);
            KConfigDialog::removePage(pageIter.value());
            return;
        }
    }
}

void
ConfigurationDialog::updateButtons()
{
    enableButtonApply(hasChanged());
}

void
ConfigurationDialog::updateSettings()
{
    foreach (Aki::ISettingsPage* page, _pageList.keys()) {
        page->updateSettings();
    }
}

void
ConfigurationDialog::updateWidgets()
{
    foreach (Aki::ISettingsPage* page, _pageList.keys()) {
        page->updateWidgets();
    }
}
