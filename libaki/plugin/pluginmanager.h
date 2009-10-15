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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "libaki_export.h"
#include <QObject>
#include <KPluginInfo>

namespace Aki
{
class MainInterface;
class Plugin;
class PluginManagerPrivate;
/**
 * Plugin manager that handles all the loading and unloading
 * of the plugins. This class is a singleton.
 */
class LIBAKI_EXPORT PluginManager : public QObject
{
    Q_OBJECT
public:
    /**
     * Initalises the plugin manager with the plugins.
     */
    void init(Aki::MainInterface *main);
    /**
     * Instance of the plugin manager.
     * @return static instance of the PluginManager.
     */
    static PluginManager* self();
    /**
     * Gets the list of plugin info of available plugins.
     * @return Plugin info list of available plugins.
     */
    KPluginInfo::List pluginInfoList() const;
    /**
     * Gets the plugin info by the plugin @p plugin.
     * @param plugin Plugin.
     * @return Plugin info if found or if not found a empty plugin info.
     */
    KPluginInfo pluginInfo(const Aki::Plugin *plugin) const;
    /**
     * Loads all the plugins if any are found.
     */
    void loadPlugins();
    /**
     * Unloads all the plugins if any were loaded.
     */
    void unloadPlugins();
private:
    /**
     * Private constructor.
     */
    PluginManager();
    /**
     * Destroys the object.
     */
    ~PluginManager();
    /**
     * Loads a plugin by the plugin @param info info if
     * one is found.
     * @param info Plugin info.
     */
    void loadPlugin(const KPluginInfo &info);
    /**
     * Sets the list of plugin @param info info.
     * @param info Plugin info list.
     */
    void setPlugins(const KPluginInfo::List &info);
    /**
     * Unloads the plugin by the plugin @param info info if
     * one is found.
     * @param info Plugin info.
     */
    void unloadPlugin(const KPluginInfo &info);
private:
    KPluginInfo::List m_plugins;
    typedef QMap<KPluginInfo, Aki::Plugin*> PluginMap;
    PluginMap m_loadedPlugins;
    Aki::MainInterface *m_mainInterface;
    friend class PluginManagerPrivate;
}; // End of class PluginManager.
} // End of namespace Aki.

#endif // PLUGINMANAGER_H
