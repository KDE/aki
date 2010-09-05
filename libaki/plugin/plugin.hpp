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
class LIBAKI_EXPORT Plugin : public QObject,
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
