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

#ifndef IMAINCONTROLLER_HPP
#define IMAINCONTROLLER_HPP

#include "aki.hpp"

namespace Aki
{
class DockWidget;
class ISettingsPage;
class Plugin;
/**
 * @brief Interface class for calling the functions of the MainWindow class.
 * Plugins use these functions to add docks, settings pages, etc to the
 * running applications.
 */
class LIBAKI_EXPORT IMainController
{
public:
    virtual ~IMainController();
    /**
     * Addes a new @p dockWidget the application in the @p area specified.
     *
     * @param dockWidget DockWidget to be added.
     * @param area Location of where the dockwidget should be added to.
     */
    virtual void addDock(Aki::DockWidget* dockWidget, Qt::DockWidgetArea area) = 0;
    /**
     * Added the @p plugin to the application.
     * \internal
     *
     * @param plugin Plugin to be added.
     */
    virtual void addGui(Aki::Plugin* plugin) = 0;
    /**
     * Registers the @p page to the configuration dialogue.
     *
     * @param page Settings page.
     */
    virtual void addSettingsPage(Aki::ISettingsPage* page) = 0;
    /**
     * Removes the @p dockWidget from the MainWindow.
     * @note You are required to delete the @p dockWidget after removal.
     *
     * @param dockWidget DockWidget to remove.
     */
    virtual void removeDock(Aki::DockWidget* dockWidget) = 0;
    /**
     * Removes the @p plugin from the application.
     * \internal
     *
     * @param plugin Plugin to be removed.
     */
    virtual void removeGui(Aki::Plugin* plugin) = 0;
    /**
     * Removes the @p page from the configuration dialogue.
     * @note You are required to delete the @p page after removal.
     *
     * @param page Settings page to be removed.
     */
    virtual void removeSettingsPage(Aki::ISettingsPage* page) = 0;
}; // End of class IMainController.
} // End of namespace Aki.

#endif // IMAINCONTROLLER_HPP
