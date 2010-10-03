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

#include "scriptingplugin.hpp"
#include "scriptmodule.hpp"
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KPluginFactory>
#include <KDE/Kross/Manager>
#include <KDE/KStandardDirs>
using namespace Aki;

//K_PLUGIN_FACTORY(ScriptingFactory, registerPlugin<ScriptingPlugin>();)
//K_EXPORT_PLUGIN(ScriptingFactory("akiscriptingplugin"))
AKI_REGISTER_PLUGIN(Scripting, scripting)

ScriptingPlugin::ScriptingPlugin(QObject* parent, const QVariantList&)
    : Aki::Plugin(parent)
{
    setComponentData(ScriptingFactory::componentData());
}

ScriptingPlugin::~ScriptingPlugin()
{
}

bool
ScriptingPlugin::checkVersion(const QString& version)
{
    return AKI_VERSION_STR == version;
}

void
ScriptingPlugin::load()
{
    //setXMLFile("akiscriptingpluginui.rc");
    Kross::Manager::self().addQObject(new Aki::ScriptModule(this, mainController()));
}

void
ScriptingPlugin::unload()
{
    Kross::Manager::self().deleteModules();
}
