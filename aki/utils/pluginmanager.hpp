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
