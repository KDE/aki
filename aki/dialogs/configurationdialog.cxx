/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

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
