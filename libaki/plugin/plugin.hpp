#ifndef AKI_PLUGIN_HPP
#define AKI_PLUGIN_HPP

#include "libaki_export.hpp"
#include <KDE/KXMLGUIClient>
#include <QtCore/QObject>

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
