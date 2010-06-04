#include "pluginmanager.hpp"
#include "aki.hpp"
#include "interfaces/imaincontroller.hpp"
#include "plugin/plugin.hpp"
#include <KDE/KGlobal>
#include <KDE/KServiceTypeTrader>
#include <QtCore/QMutableHashIterator>
using namespace Aki;

namespace Aki
{
class PluginManagerPrivate
{
public:
    Aki::PluginManager instance;
}; // End of class PluginManagerPrivate.
} // End of namespace Aki.

K_GLOBAL_STATIC(Aki::PluginManagerPrivate, pluginManager)

PluginManager::PluginManager()
    : _mainController(0)
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
    Q_ASSERT(controller);
    _mainController = controller;

    // Search for all services matching Aki/Plugin.
    setPlugins(KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Aki/Plugin")));

    if (_plugins.isEmpty()) {
        kDebug() << "No plugins found";
        return;
    }

    kDebug() << _plugins.count() << " plugins found";

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
    // Get the service by the plugin info.
    KService::Ptr ptr = info.service();

    // Try and create the plugin.
    Aki::Plugin* plugin = ptr->createInstance<Aki::Plugin>(0, this);
    if (!plugin) {
        return;
    }

    if (!plugin->checkVersion(AKI_VERSION_STR)) {
        kError() << QString("Plugin %1's version does not match Aki's version.").arg(ptr->name());
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

Aki::PluginManager*
PluginManager::self()
{
    return &pluginManager->instance;
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
