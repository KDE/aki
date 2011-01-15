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

#ifndef AKI_SCRIPTMODULE_HPP
#define AKI_SCRIPTMODULE_HPP

#include <aki.hpp>
#include <QtCore/QObject>

namespace Aki
{
class IMainController;
class ISettingsPage;
class ScriptModule : public QObject
{
    Q_OBJECT
public:
    explicit ScriptModule(QObject* parent, Aki::IMainController* controller);
    ~ScriptModule();
    Q_INVOKABLE void addSettingsPage(Aki::ISettingsPage* page);
    Q_INVOKABLE void executeCommand(const QString& command);
    Q_INVOKABLE QObject* mainView() const;
    Q_INVOKABLE void printText(const QString& text);
    Q_INVOKABLE void removeSettingsPage(Aki::ISettingsPage* page);
private:
    Aki::IMainController* _mainController;
}; // End of class ScriptModule.
} // End of namespace Aki.

#endif // AKI_SCRIPTMODULE_HPP
