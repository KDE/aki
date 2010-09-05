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
