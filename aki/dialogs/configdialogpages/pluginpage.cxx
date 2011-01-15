/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
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

#include "pluginpage.hpp"
#include "aki.hpp"
#include "settings.h"
#include "utils/pluginmanager.hpp"
#include <KDE/KPluginSelector>
#include <QtGui/QVBoxLayout>
using namespace Aki;

PluginPage::PluginPage(QWidget* parent)
    : Aki::ISettingsPage(i18n("Plugins"), Aki::Settings::self(),
                         "preferences-plugin", i18n("Configure Plugin Settings"),
                         parent),
    _pluginsChanged(false)
{
    _pluginSelector = new KPluginSelector(this);
    connect(_pluginSelector, SIGNAL(changed(bool)),
            SLOT(slotChanged(bool)));
    connect(_pluginSelector, SIGNAL(configCommitted(QByteArray)),
            SLOT(slotChanged(QByteArray)));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_pluginSelector);
    setLayout(layout);

    _pluginSelector->addPlugins(Aki::PluginManager::self()->pluginInfos(),
                                KPluginSelector::IgnoreConfigFile, i18n("Plugins"),
                                "Plugins");
    _pluginSelector->updatePluginsState();
}

PluginPage::~PluginPage()
{
}

bool
PluginPage::hasChanged() const
{
    return _pluginsChanged;
}

void
PluginPage::slotChanged(bool)
{
    if (_pluginsChanged) {
        _pluginSelector->updatePluginsState();
        Aki::PluginManager::self()->loadPlugins();
    }
    emit widgetsModified();
}

void
PluginPage::slotChanged(const QByteArray&)
{
    _pluginsChanged = true;
}

void
PluginPage::updateSettings()
{
    if (_pluginsChanged) {
        _pluginSelector->save();
    }
}
