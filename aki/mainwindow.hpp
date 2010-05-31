#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "interfaces/imaincontroller.hpp"
#include "ui/mainwindow.hpp"

namespace Aki
{
class ISettingsPage;
class Plugin;
class ServerDialog;
class AkiWindow : public Aki::MainWindow,
                  public Aki::IMainController
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
    void identityListTriggered();
    void serverListTriggered();
    void quitTriggered();
private:
    void createMenus();
    void createDialogs();
}; // End of class AkiWindow.
} // End of namespace Aki.

#endif // MAINWINDOW_HPP
