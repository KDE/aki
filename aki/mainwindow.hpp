#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui/mainwindow.hpp"

namespace Aki
{
class ServerDialog;
class AkiWindow : public Aki::MainWindow
{
    Q_OBJECT
public:
    AkiWindow();
    ~AkiWindow();
public Q_SLOTS:
    void serverListTriggered();
    void quitTriggered();
private:
    void createMenus();
    void createDialogs();
private:
    Aki::ServerDialog* _serverDialog;
}; // End of class AkiWindow.
} // End of namespace Aki.

#endif // MAINWINDOW_HPP
