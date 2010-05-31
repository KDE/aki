#ifndef AKI_PLUGIN_HPP
#define AKI_PLUGIN_HPP

#include "libaki_export.hpp"
#include <KDE/KXMLGUIClient>
#include <QtCore/QObject>

namespace Aki
{
class LIBAKI_EXPORT Plugin : public QObject,
                             public KXMLGUIClient
{
    Q_OBJECT
public:
    Plugin(QObject* parent);
    virtual ~Plugin();
    virtual void checkVersion(const QString& version) = 0;
protected:
    virtual void load() = 0;
    virtual void unload() = 0;
}; // End of class Plugin.
} // End of namespace Aki.

#endif // AKI_PLUGIN_HPP
