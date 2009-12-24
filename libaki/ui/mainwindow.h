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

#ifndef AKI_MAINWINDOW_H
#define AKI_MAINWINDOW_H

#include "libaki_export.h"
#include "interfaces/maininterface.h"
#include <KXmlGuiWindow>
#include <QScopedPointer>

class QDockWidget;

namespace Aki
{
class Plugin;
class ServerView;
class SettingsPageInterface;
class MainWindowPrivate;
class LIBAKI_EXPORT MainWindow : public KXmlGuiWindow,
                                 public Aki::MainInterface
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual void addSettingsPage(Aki::SettingsPageInterface *page);
    virtual void removeSettingsPage(Aki::SettingsPageInterface *page);
    virtual void addGui(Aki::Plugin *plugin);
    virtual void removeGui(Aki::Plugin *plugin);

    /**
     * Adds the @p dock to the main window.
     *
     * @param dock Dock widget to be added to the window.
     * @param area Area the dock widget will be docked to.
     * @param orientation Orientation of the title bar.
     */
    virtual void addDock(QDockWidget* dock, Qt::DockWidgetArea area,
                         Qt::Orientation orientation);

    /**
     * Removes the @p dock from the main window.
     * @note You have to delete the dock after calling this function.
     *
     * @param dock Dock widget to be removed from the window.
     */
    virtual void removeDock(QDockWidget* dock);
    virtual Aki::ServerView* mainView();

    /**
     * Restores the default window title.
     */
    void restoreAkiDefaultWindowTitle();

    /**
     * Sets the window title with the current @p serverName.
     * Aki IRC Client - Freenode.
     *
     * @param serverName Current server name.
     */
    void setAkiWindowTitle(const QString &serverName);

    /**
     * Sets the window title with the current @p serverName and the
     * current @p channelName.
     * Aki IRC Client - #aki (Freenode)
     *
     * @param serverName Current server name.
     * @param channelName Current channel name.
     */
    void setAkiWindowTitle(const QString &serverName, const QString &channelName);

    void loadPlugins();

    void loadCustomServer(const QString &nick, const QString &server = QString(), quint16 port = 6667,
                          const QString &serverPassword = QString(), bool ssl = false);
protected:
    virtual void closeEvent(QCloseEvent *event);
private:
    Q_PRIVATE_SLOT(d, void quickConnectionTriggered())
    Q_PRIVATE_SLOT(d, void reconnectionTriggered())
    Q_PRIVATE_SLOT(d, void disconnectionTriggered())
    Q_PRIVATE_SLOT(d, void quitTriggered())
    Q_PRIVATE_SLOT(d, void charSelectorTriggered())
    Q_PRIVATE_SLOT(d, void urlWatcherTriggered())
    Q_PRIVATE_SLOT(d, void clearAllWindowsTriggered())
    Q_PRIVATE_SLOT(d, void messageLogTriggered())
    Q_PRIVATE_SLOT(d, void preferencesTriggered())
    Q_PRIVATE_SLOT(d, void configureNotificationsTriggered())
    Q_PRIVATE_SLOT(d, void charSelected(const QChar &ch))
    Q_PRIVATE_SLOT(d, void identityTriggered())
    Q_PRIVATE_SLOT(d, void findTriggered())
    Q_PRIVATE_SLOT(d, void clearMarkerLineTriggered())
private:
    friend class MainWindowPrivate;
    QScopedPointer<MainWindowPrivate> d;
}; // End of class MainWindow.
} // End of namespace Aki.

#endif // AKI_MAINWINDOW_H
