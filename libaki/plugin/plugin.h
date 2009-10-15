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

#ifndef PLUGIN_H
#define PLUGIN_H

#include "libaki_export.h"
#include <KXMLGUIClient>

namespace Aki
{
class MainInterface;
/**
 * Base class for all Aki plugins.
 *
 * Plugin is a component which is loaded in to Aki shell at startup or by request.
 * Each plugin should have a corresponding .desktop file with a description. The .desktop
 * file template looks like:
 *
 * @code
 * [Desktop Entry]
 * Type=Service
 * Name=
 * Comment=
 * GenericName=
 * Comment=
 * Icon=
 * ServiceTypes=Aki/Plugin
 * X-KDE-Library=
 * X-KDE-PluginInfo-Author=
 * X-KDE-PluginInfo-Email=
 * X-KDE-PluginInfo-Name=
 * X-KDE-PluginInfo-Version=
 * X-KDE-PluginInfo-Website=
 * X-KDE-PluginInfo-Category=
 * X-KDE-PluginInfo-Depends=
 * X-KDE-PluginInfo-License=
 * X-KDE-PluginInfo-EnabledByDefault=
 * @endcode
 * <b>Name</b> is a non-translatable name of a plugin, it is used in KTrader queries to search for a plugin (required);<br>
 * <b>GenericName</b> is a translatable name of a plugin, it is used to show plugin names in GUI (required);<br>
 * <b>Comment</b> is a short description about the plugin (optional);<br>
 * <b>Icon</b> is a plugin icon (preferred);<br>
 * <b>ServiceTypes</b> should stay as Aki/Plugin so we know it's meant for Aki.(required)<br>
 * <b>X-KDE-Library</b> is a name of library which contains the plugin (required);<br>
 * <b>X-KDE-PluginInfo-Author</b> is your full name.(required)<br>
 * <b>X-KDE-PluginInfo-Email</b> is an email address to contact you (optional);<br>
 * <b>X-KDE-PluginInfo-Name</b> is a name of the shared object (unix) or dll (windows) without the extension (required):<br>
 * <b>X-KDE-PluginInfo-Version</b> is a version of a plugin (optional);<br>
 * <b>X-KDE-PluginInfo-Website</b> is a home page of a plugin (optional);<br>
 * <b>X-KDE-PluginInfo-Category</b> is the category this plugin belongs to (optional):<br>
 * <b>X-KDE-PluginInfo-Depends</b> is what this plugin depends on other plugin (optional):<br>
 * <b>X-KDE-PluginInfo-License</b> is a license (optional). can be: GPL, LGPL, BSD, Artistic, QPL or Custom. If this property is not set, license is considered as unknown;<br>
 * <b>X-KDE-PluginInfo-EnabledByDefault</b> is whether this plugin loads by default on the first time or not (required)<br>
 */
class LIBAKI_EXPORT Plugin : public QObject,
                             public KXMLGUIClient
{
    Q_OBJECT
public:
    /**
     * Creates the plugin.
     * @param parent Parent of the object.
     */
    Plugin(QObject *parent);
    /**
     * Destroys the object.
     */
    virtual ~Plugin();
    /**
     * Called when the plugin is loaded in to Aki.
     */
    virtual void load() = 0;
    /**
     * Called when the plugin is unloaded from Aki.
     */
    virtual void unload() = 0;
    /**
     * Checks the version the plugin was compiled with Aki against
     * the current version of Aki.
     * @param version Current Aki version.
     */
    virtual bool checkVersion(const QString &version) = 0;
    /**
     * Adds the main interface from the application to gain access to it.
     * @param main Main interface for Aki.
     * @internal
     */
    virtual void setMainInterface(Aki::MainInterface *main);
    /**
     * Gets the main interface from the application. You should use this
     * to gain access of the application.
     * @return Main interface to Aki.
     */
    virtual Aki::MainInterface* mainInterface();
private:
    Aki::MainInterface *m_mainInterface;
}; // End of class Plugin.
} // End of namespace Aki.

#endif // PLUGIN_H
