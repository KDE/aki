#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "interfaces/imaincontroller.hpp"
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
class AkiWindow : public Aki::MainWindow,
                  public Aki::IMainController
{
    Q_OBJECT
public:
    AkiWindow();
    ~AkiWindow();
    virtual void addDock(Aki::DockWidget* dockWidget, Qt::DockWidgetArea area);
    virtual void addGui(Aki::Plugin* plugin);
    virtual void addSettingsPage(Aki::ISettingsPage* page);
    virtual void removeDock(Aki::DockWidget* dockWidget);
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
    void createMenus();
    void createDialogs();
private:
    Aki::ConfigurationDialog* _configDialog;
    Aki::View* _view;
    Aki::SystemTray* _systemTray;
}; // End of class AkiWindow.
} // End of namespace Aki.

#endif // MAINWINDOW_HPP
