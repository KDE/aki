/*
    This file is part of Aki IRC.
    Copyright 2009 - 2010 Keith Rusler <xzekex@live.co.uk>

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

#ifndef AKIMODULE_H
#define AKIMODULE_H

#include <QObject>

namespace Aki
{
class MainInterface;
class ServerView;
class SettingsPageInterface;
namespace Irc
{
class Socket;
} // End of namespace Irc.
} // End of namespace Aki.

class QDockWidget;
class AkiModulePrivate;
class AkiModule : public QObject
{
    Q_OBJECT
public:
    explicit AkiModule(QObject *parent, Aki::MainInterface *interface);
    virtual ~AkiModule();
public:
    Q_INVOKABLE void addDock(QDockWidget *widget);
    Q_INVOKABLE void removeDock(QDockWidget *widget);
    Q_INVOKABLE void addSettingsPage(Aki::SettingsPageInterface *page);
    Q_INVOKABLE void removeSettingsPage(Aki::SettingsPageInterface *page);
    Q_INVOKABLE QObject* mainView();
    Q_INVOKABLE void executeCommand(const QString &command);
    Q_INVOKABLE void printText(const QString &str);
    Q_INVOKABLE QObject* socket();
private:
    friend class AkiModulePrivate;
    QScopedPointer<AkiModulePrivate> d;
}; // End of class AkiModule.

#endif // AKIMODULE_H
