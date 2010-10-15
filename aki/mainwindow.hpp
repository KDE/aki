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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui/mainwindow.hpp"

namespace Aki
{
class ConfigurationDialog;
class DockWidget;
class ISettingsPage;
class Plugin;
class ServerDialog;
class SystemTray;
class View;
class AkiWindow : public Aki::MainWindow
{
    Q_OBJECT
public:
    AkiWindow();
    ~AkiWindow();
    virtual void addGui(Aki::Plugin* plugin);
    virtual void addSettingsPage(Aki::ISettingsPage* page);
    virtual void removeGui(Aki::Plugin* plugin);
    virtual void removeSettingsPage(Aki::ISettingsPage* page);
public Q_SLOTS:
    void slotConfigureNotifications();
    void slotIdentityListTriggered();
    void slotNetworkListTriggered();
    void slotPreferencesTriggered();
    void slotQuickConnectionTriggered();
    void slotQuitTriggered();
    void slotShowMenubar();
private:
    void createAction(const QString& actionName, const QString& description, const char* slot, const KIcon& icon = KIcon());
    void createMenus();
    void createDialogs();
private:
    Aki::ConfigurationDialog* _configDialog;
    Aki::View* _view;
    Aki::SystemTray* _systemTray;
}; // End of class AkiWindow.
} // End of namespace Aki.

#endif // MAINWINDOW_HPP
