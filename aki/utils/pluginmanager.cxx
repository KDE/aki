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

#include "pluginmanager.hpp"
#include "debughelper.hpp"
#include "interfaces/imaincontroller.hpp"
#include "plugin/plugin.hpp"
#include <KDE/KGlobal>
#include <KDE/KServiceTypeTrader>
#include <QtCore/QMutableHashIterator>
using namespace Aki;

template<> Aki::PluginManager* Aki::Singleton<Aki::PluginManager>::_instance = 0;

PluginManager::PluginManager()
    : Aki::Singleton<Aki::PluginManager>(),
    _mainController(0)
{
    _loadedPlugins.clear();
    _plugins.clear();
}

PluginManager::~PluginManager()
{
}

void
PluginManager::initialize(Aki::IMainController* controller)
{
    DEBUG_FUNC_NAME
    _mainController = controller;

    // Search for all services matching Aki/Plugin.
    setPlugins(KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Aki/Plugin")));

    if (_plugins.isEmpty()) {
        DEBUG_TEXT("No plugins found")
        return;
    }

    DEBUG_TEXT2("%1 plugins found", _plugins.count())

    KPluginInfo::List::Iterator begin = _plugins.begin();
    KPluginInfo::List::Iterator end = _plugins.end();

    // Load every plugin from the configuration file.
    for (; begin != end; ++begin) {
        KPluginInfo& pi = *begin;
        // Plugin config is stored by category.
        pi.setConfig(KGlobal::config()->group(pi.category()));
        // Load the service.
        pi.load();
    }
}

void
PluginManager::loadPlugin(const KPluginInfo& info)
{
    DEBUG_FUNC_NAME
    // Get the service by the plugin info.
    KService::Ptr ptr = info.service();

    // Try and create the plugin.
    Aki::Plugin* plugin = ptr->createInstance<Aki::Plugin>(0, this);
    if (!plugin) {
        return;
    }

    if (!plugin->checkVersion(AKI_VERSION_STR)) {
        DEBUG_TEXT2("Plugin %1's version does not match Aki's version.", ptr->name())
        delete plugin;
    } else {
        // Pass the main interface to the plugin.
        plugin->setMainController(_mainController);

        // We load the plugin since it was successful.
        plugin->load();

        // Add the plugin's gui to the application.
        _mainController->addGui(plugin);

        // Insert the plugin to the loaded hash.
        _loadedPlugins.insert(info, plugin);
    }
}

void
PluginManager::loadPlugins()
{
    KPluginInfo::List::Iterator begin = _plugins.begin();
    KPluginInfo::List::Iterator end = _plugins.end();
    // Loop through the plugin info list.
    for (; begin != end; ++begin) {
        KPluginInfo& pi = *begin;

        /* We check the state of the plugins. If the plugin is already loaded
         * but it is not enabled we need to unload it.
         */
        if (_loadedPlugins.contains(pi) && !pi.isPluginEnabled()) {
            // Unload the plugin and save it to the configuration file.
            unloadPlugin(pi);
            // Save it
            pi.save();
        } else if (!_loadedPlugins.contains(pi) && pi.isPluginEnabled()) {
            // Load the plugin and save it to the configuration file.
            loadPlugin(pi);
            pi.save();
        }
    }
}

KPluginInfo
PluginManager::pluginInfo(const Aki::Plugin* plugin) const
{
    // Get a plugin info from plugin
    KPluginInfo info = _loadedPlugins.key(const_cast<Aki::Plugin*>(plugin));

    if (!info.isValid()) {
        return KPluginInfo();
    }

    return info;
}

KPluginInfo::List
PluginManager::pluginInfos() const
{
    return _plugins;
}

void
PluginManager::setPlugins(const KPluginInfo::List& info)
{
    _plugins = info;
}

void
PluginManager::unloadPlugins()
{
    Aki::PluginManager::PluginHash::Iterator begin = _loadedPlugins.begin();
    Aki::PluginManager::PluginHash::Iterator end = _loadedPlugins.end();

    for (; begin != end; ++begin) {
        unloadPlugin(begin.key());
    }
}

void
PluginManager::unloadPlugin(const KPluginInfo& info)
{
    // Get a plugin by the plugin info.
    Aki::Plugin* plugin = _loadedPlugins.find(info).value();

    // If plugin doesn't exist no need to unload.
    if (!plugin) {
        return;
    }

    // Plugin was found and is valid so we start the unload process.
    plugin->unload();

    // Remove the plugin's gui from the application.
    _mainController->removeGui(plugin);

    delete _loadedPlugins.take(info);
}
