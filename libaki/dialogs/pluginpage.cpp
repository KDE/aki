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

#include "pluginpage.h"
#include "plugin/pluginmanager.h"
#include "settings.h"
#include <KLocale>
#include <KPluginSelector>
#include <QVBoxLayout>
using namespace Aki;

namespace Aki
{
class PluginPagePrivate
{
public:
    PluginPagePrivate(Aki::PluginPage *qq)
        : q(qq),
        pluginSelector(0),
        pluginsChanged(false)
    {
    }

    void slotChanged(bool hasChanged)
    {
        pluginsChanged = hasChanged;
        if (pluginsChanged) {
            pluginSelector->updatePluginsState();
            Aki::PluginManager::self()->loadPlugins();
        }
        emit q->widgetsModified();
    }

    void slotChanged(const QByteArray &name)
    {
        Q_UNUSED(name);
        pluginsChanged = true;
    }

    Aki::PluginPage *q;
    KPluginSelector *pluginSelector;
    bool pluginsChanged;
}; // End of class PluginPagePrivate.
} // End of namespace Aki.

PluginPage::PluginPage(QWidget *parent)
    : Aki::SettingsPageInterface(i18n("Plugins"), Aki::Settings::self(),
                                 "preferences-plugin", i18n("Configure Plugin Settings"),
                                 parent)
{
    d.reset(new PluginPagePrivate(this));
    d->pluginSelector = new KPluginSelector(this);

    connect(d->pluginSelector, SIGNAL(changed(bool)),
            SLOT(slotChanged(bool)));
    connect(d->pluginSelector, SIGNAL(configCommitted(QByteArray)),
            SLOT(slotChanged(QByteArray)));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(d->pluginSelector);
    setLayout(mainLayout);

    d->pluginSelector->addPlugins(Aki::PluginManager::self()->pluginInfoList(),
                                  KPluginSelector::IgnoreConfigFile, i18n("Plugins"),
                                  "Plugins");
    d->pluginSelector->updatePluginsState();
}

PluginPage::~PluginPage()
{
}

void
PluginPage::updateSettings()
{
    if (d->pluginsChanged) {
        d->pluginSelector->save();
    }
}

bool
PluginPage::hasChanged() const
{
    return d->pluginsChanged;
}

#include "pluginpage.moc"
