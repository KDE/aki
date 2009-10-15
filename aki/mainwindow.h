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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "interfaces/maininterface.h"
#include <KXmlGuiWindow>

namespace Aki
{
class ServerView;
} // End of class Aki.

class DBus;
class AkiStatusBar;
class KAction;
class SettingsDialog;
class IdentityDialog;
class MainWindow : public KXmlGuiWindow,
                   public Aki::MainInterface
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual void addSettingsPage(Aki::SettingsPageInterface *page);
    virtual void removeSettingsPage(Aki::SettingsPageInterface* page);
    virtual void addGui(Aki::Plugin* plugin);
    virtual void removeGui(Aki::Plugin* plugin);
    void createMenus();
    AkiStatusBar* statusBar();
    Aki::ServerView* mainView();
    virtual void addDock(QDockWidget *dock, Qt::DockWidgetArea area,
                         Qt::Orientation orientation);
    virtual void removeDock(QDockWidget *dock);
    void loadPlugins();
public Q_SLOTS:
    void openConfigureNotifications();
    virtual void quitApplication();
protected:
    virtual void closeEvent(QCloseEvent *event);
private Q_SLOTS:
    void slotIdentityDialog();
    void slotPreferences();
    void slotCharSelectorDialog();
    void slotQuickConnection();
    void slotReconnectTriggered();
    void slotDisconnectTriggered();
    void slotCharSelected(const QChar &ch);
private:
    KAction *m_akiNewServerTabAction;
    KAction *m_akiQuickConnectionAction;
    KAction *m_akiReconnectAction;
    KAction *m_akiDisconnectAction;
    KAction *m_akiQuitAction;
    KAction *m_windowCharSelectorAction;
    KAction *m_windowIdentityAction;
    KAction *m_windowUrlWatcherAction;
    KAction *m_windowClearAllWindowsAction;
    KAction *m_windowMessageLogAction;
    AkiStatusBar *m_statusBar;
    IdentityDialog *m_identityDialog;
    SettingsDialog *m_settingsDialog;
    Aki::ServerView *m_view;
    DBus *m_dbus;
};

#endif // MAINWINDOW_H
