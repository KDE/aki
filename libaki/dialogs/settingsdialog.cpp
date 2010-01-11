/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "settingsdialog.h"
#include "colorpage.h"
#include "generalpage.h"
#include "interfaces/settingspageinterface.h"
#include "ircpage.h"
#include "logpage.h"
#include "pluginpage.h"
#include "proxypage.h"
#include "serverpage.h"
#include "settings.h"
#include <KDebug>
#include <KLocale>
using namespace Aki;

namespace Aki
{
class SettingsDialogPrivate
{
public:
    SettingsDialogPrivate(Aki::SettingsDialog *qq)
        : q(qq),
        generalPage(0),
        colorPage(0),
        pluginPage(0),
        serverPage(0),
        ircPage(0),
        logPage(0),
        proxyPage(0)
    {
    }

    Aki::SettingsDialog *q;
    SettingsDialog::PageList pageList;
    Aki::GeneralPage *generalPage;
    Aki::ColorPage *colorPage;
    Aki::PluginPage *pluginPage;
    Aki::ServerPage *serverPage;
    Aki::IrcPage *ircPage;
    Aki::LogPage *logPage;
    Aki::ProxyPage *proxyPage;
}; // End of class SettingsDialog.
} // End of namespace Aki.

SettingsDialog::SettingsDialog(QWidget *parent)
    : KConfigDialog(parent, "settings", Aki::Settings::self())
{
    d.reset(new Aki::SettingsDialogPrivate(this));
    setButtons(Help | Ok | Cancel | Apply);
    setSizeGripEnabled(true);

    d->generalPage = new Aki::GeneralPage(this);
    connect(d->generalPage, SIGNAL(serverTabPositionChanged(int)),
            SIGNAL(serverTabPositionChanged(int)));
    connect(d->generalPage, SIGNAL(channelTabPositionChanged(int)),
            SIGNAL(channelTabPositionChanged(int)));
    addPage(d->generalPage);

    d->colorPage = new Aki::ColorPage(this);
    addPage(d->colorPage);

    d->pluginPage = new Aki::PluginPage(this);
    addPage(d->pluginPage);

    d->serverPage = new Aki::ServerPage(this);
    addPage(d->serverPage);

    d->ircPage = new Aki::IrcPage(this);
    connect(d->ircPage, SIGNAL(showNickListToggled(bool)),
            SIGNAL(showNickListToggled(bool)));
    connect(d->ircPage, SIGNAL(showModeBarToggled(bool)),
            SIGNAL(showModeBarToggled(bool)));
    addPage(d->ircPage);

    d->logPage = new Aki::LogPage(this);
    addPage(d->logPage);

    d->proxyPage = new Aki::ProxyPage(this);
    addPage(d->proxyPage);
}

SettingsDialog::~SettingsDialog()
{
}

void
SettingsDialog::addPage(Aki::SettingsPageInterface *page)
{
    connect(page, SIGNAL(widgetsModified()), this, SLOT(updateButtons()));
    KPageWidgetItem *item = KConfigDialog::addPage(page, page->config(), page->name(), page->icon(), page->header());
    d->pageList.insert(page, item);
}

void
SettingsDialog::removePage(Aki::SettingsPageInterface *page)
{
    QMutableMapIterator<Aki::SettingsPageInterface*, KPageWidgetItem*> pageIter(d->pageList);
    while (pageIter.hasNext()) {
        pageIter.next();

        if (pageIter.key() == page) {
            d->pageList.remove(page);
            KConfigDialog::removePage(pageIter.value());
        }
    }
}

bool
SettingsDialog::hasChanged()
{
    bool changed = false;
    foreach (const Aki::SettingsPageInterface *page, d->pageList.keys()) {
        if (page->hasChanged()) {
            changed = true;
        }
    }

    return changed;
}

void
SettingsDialog::updateButtons()
{
    enableButtonApply(hasChanged());
}

void
SettingsDialog::updateSettings()
{
    foreach (Aki::SettingsPageInterface *page, d->pageList.keys()) {
        page->updateSettings();
    }

    KConfigDialog::updateSettings();
}

void
SettingsDialog::updateWidgets()
{
    foreach (Aki::SettingsPageInterface *page, d->pageList.keys()) {
        page->updateWidgets();
    }

    KConfigDialog::updateWidgets();
}

#include "settingsdialog.moc"
