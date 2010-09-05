#ifndef AKI_SCRIPTINGPLUGIN_HPP
#define AKI_SCRIPTINGPLUGIN_HPP

#include "plugin/plugin.hpp"
#include <QtCore/QVariant>

namespace Aki
{
class ScriptingPlugin : public Aki::Plugin
{
    Q_OBJECT
public:
    ScriptingPlugin(QObject* parent, const QVariantList& args);
    ~ScriptingPlugin();
    virtual bool checkVersion(const QString& version);
protected:
    virtual void load();
    virtual void unload();
}; // End of class ScriptingPlugin.
} // End of namespace Aki.

#endif // AKI_SCRIPTINGPLUGIN_HPP
