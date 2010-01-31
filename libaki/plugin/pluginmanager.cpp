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

#include "pluginmanager.h"
#include "akiversion.h"
#include "interfaces/maininterface.h"
#include "plugin.h"
#include <KDebug>
#include <kdemacros.h>
#include <KGlobal>
#include <QMutableMapIterator>
#include <KServiceTypeTrader>
using namespace Aki;

namespace Aki
{
class PluginManagerPrivate
{
public:
    PluginManager instance;
}; // End of namespace PluginManagerPrivate.
} // End of namespace Aki.

K_GLOBAL_STATIC(PluginManagerPrivate, pluginManager)

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
}

void
PluginManager::init(Aki::MainInterface *main)
{
    m_mainInterface = main;
    // Searches for all Services matching Aki/Plugin.
    setPlugins(KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Aki/Plugin")));

    // Load every plugin from the configuration file.
    for (KPluginInfo::List::Iterator i = m_plugins.begin(); i != m_plugins.end(); ++i) {
        KPluginInfo &pi = *i;
        // Plugin config is stored by category.
        pi.setConfig(KGlobal::config()->group(pi.category()));
        // Load the Service.
        pi.load();
    }
}

PluginManager*
PluginManager::self()
{
    return &pluginManager->instance;
}

void
PluginManager::loadPlugin(const KPluginInfo &info)
{
    // Get the service by the plugin info.
    KService::Ptr ptr = info.service();

    // Try and create the plugin.
    Aki::Plugin *plugin = ptr->createInstance<Aki::Plugin>(0, this);
    if (!plugin) {
        return;
    }

    if (!plugin->checkVersion(AKI_VERSION_STR)) {
        kError() << QString("Plugin %1's version does not match Aki's version.").arg(AKI_VERSION_STR);
        delete plugin;
    } else {
        // Pass the main interface to the plugin.
        plugin->setMainInterface(m_mainInterface);

        // We load the plugin since it was successful.
        plugin->load();

        // Add the plugin's gui to the application.
        m_mainInterface->addGui(plugin);

        m_loadedPlugins.insert(info, plugin);
    }
}

void
PluginManager::loadPlugins()
{
    // Loop through the plugin info list
    for (KPluginInfo::List::Iterator i = m_plugins.begin(); i != m_plugins.end(); ++i) {
        KPluginInfo &pi = *i;

        /* We check the state of the plugins. If the plugin is already loaded
         * but it is not enabled we need to unload it.
         */
        if (m_loadedPlugins.contains(pi) && !pi.isPluginEnabled()) {
            // Unload the plugin and save it to the configuration file.
            unloadPlugin(pi);
            pi.save();
            // If the plugin isn't load and is enabled we should load it.
        } else if (!m_loadedPlugins.contains(pi) && pi.isPluginEnabled()) {
            // Load the plugin and save it to the configuration file.
            loadPlugin(pi);
            pi.save();
        }
    }
}

KPluginInfo
PluginManager::pluginInfo(const Aki::Plugin *plugin) const
{
    /* We loop through the loaded plugins and search for this plugin.
     * If the plugin is loaded get the plugin info of it.
     */
    for (PluginMap::ConstIterator i = m_loadedPlugins.constBegin();
        i != m_loadedPlugins.constEnd(); ++i) {
        // Does the stored plugin match this plugin
        if (i.value() == plugin) {
            // It does so we return the plugin info of it.
            return i.key();
        }
    }

    // None was found so we return an empty one.
    return KPluginInfo();
}

KPluginInfo::List
PluginManager::pluginInfoList() const
{
    return m_plugins;
}

void
PluginManager::setPlugins(const KPluginInfo::List &pluginInfoList)
{
    m_plugins = pluginInfoList;
}

void
PluginManager::unloadPlugin(const KPluginInfo &info)
{
    // Get a plugin by the plugin info.
    Aki::Plugin *plugin = m_loadedPlugins.find(info).value();

    // If plugin doesn't exist no need to unload.
    if (!plugin) {
        return;
    }

    // Plugin was found and is valid so we start the unload process.
    plugin->unload();

    // Remove the plugin's gui from the application.
    m_mainInterface->removeGui(plugin);

    delete m_loadedPlugins.take(info);
}

void
PluginManager::unloadPlugins()
{
    PluginMap::Iterator begin = m_loadedPlugins.begin();
    PluginMap::Iterator end = m_loadedPlugins.end();

    while (begin != end) {
        unloadPlugin(begin.key());
        ++begin;
    }
}
