#ifndef AKI_PLUGINMANAGER_HPP
#define AKI_PLUGINMANAGER_HPP

#include <KDE/KPluginInfo>
#include <QtCore/QHash>
#include <QtCore/QObject>

namespace Aki
{
class IMainController;
class Plugin;
class PluginManagerPrivate;
class PluginManager : public QObject
{
    Q_OBJECT
public:
    void initialize(Aki::IMainController* controller);
    void loadPlugins();
    KPluginInfo pluginInfo(const Aki::Plugin* plugin) const;
    KPluginInfo::List pluginInfos() const;
    static Aki::PluginManager* self();
    void unloadPlugins();
private:
    PluginManager();
    ~PluginManager();
    void loadPlugin(const KPluginInfo& info);
    void setPlugins(const KPluginInfo::List& info);
    void unloadPlugin(const KPluginInfo& info);
private:
    friend class PluginManagerPrivate;
    typedef QHash<KPluginInfo, Aki::Plugin*> PluginHash;
    KPluginInfo::List _plugins;
    Aki::PluginManager::PluginHash _loadedPlugins;
    Aki::IMainController* _mainController;
}; // End of class PluginManager.
} // End of namespace Aki.

#endif // AKI_PLUGINMANAGER_HPP
