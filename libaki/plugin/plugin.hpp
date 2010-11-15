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

#ifndef AKI_PLUGIN_HPP
#define AKI_PLUGIN_HPP

#include "aki.hpp"
#include <KDE/KXMLGUIClient>
#include <QtCore/QObject>

#define AKI_REGISTER_PLUGIN(factoryName, name) \
    K_PLUGIN_FACTORY(factoryName##Factory, registerPlugin<factoryName##Plugin>();) \
    K_EXPORT_PLUGIN(factoryName##Factory(#name))

namespace Aki
{
class IMainController;
class PluginManager;
class LIBAKI_EXPORT Plugin
    : public QObject,
      public KXMLGUIClient
{
    Q_OBJECT
public:
    Plugin(QObject* parent);
    virtual ~Plugin();
    virtual bool checkVersion(const QString& version) = 0;
protected:
    virtual void load() = 0;
    Aki::IMainController* mainController() const;
    virtual void unload() = 0;
private:
    void setMainController(Aki::IMainController* controller);
private:
    friend class PluginManager;
    Aki::IMainController* _mainController;
}; // End of class Plugin.
} // End of namespace Aki.

#endif // AKI_PLUGIN_HPP
