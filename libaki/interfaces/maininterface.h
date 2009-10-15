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

#ifndef AKI_MAININTERFACE_H
#define AKI_MAININTERFACE_H

#include "libaki_export.h"
#include "qnamespace.h"

class QDockWidget;
namespace Aki
{
class Plugin;
class SettingsPageInterface;
class ServerView;
/**
 * Interface for manipulating stuff from the plugin
 * from plugins.
 */
class LIBAKI_EXPORT MainInterface
{
public:
    /**
     * Destroys the object.
     */
    virtual ~MainInterface();
    /**
     * Adds a settings page @p page to the main configuration dialog.
     * @param page Settings page.
     */
    virtual void addSettingsPage(Aki::SettingsPageInterface *page) = 0;
    /**
     * Removes a settings page @p page from the main configuration dialog.
     * @param page Settings page.
     */
    virtual void removeSettingsPage(Aki::SettingsPageInterface *page) = 0;
    /**
     * Adds the plugin @p plugin to the gui.
     * @param plugin Plugin being added.
     */
    virtual void addGui(Aki::Plugin *plugin) = 0;
    /**
     * Removes the plugin @p plugin from the gui.
     * @param plugin Plugin being removed.
     */
    virtual void removeGui(Aki::Plugin *plugin) = 0;

    virtual Aki::ServerView* mainView() = 0;

    virtual void addDock(QDockWidget *dock, Qt::DockWidgetArea area,
                         Qt::Orientation orientation) = 0;

    virtual void removeDock(QDockWidget *dock) = 0;
}; // End of class MainInterface.
} // End of namespace Aki.

#endif // AKI_MAININTERFACE_H
