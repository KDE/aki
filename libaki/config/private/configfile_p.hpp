#ifndef AKI_CONFIGFILE_P_HPP
#define AKI_CONFIGFILE_P_HPP

#include <KConfigGroup>
#include <KSharedConfig>
#include <QtCore/QObject>

namespace Aki
{
class ConfigFile;
class ConfigFilePrivate : public QObject
{
    Q_OBJECT
public:
    explicit ConfigFilePrivate(Aki::ConfigFile* qq);
public:
    KSharedConfigPtr config;
    KConfigGroup currentGroup;
private:
    Aki::ConfigFile* _q;
}; // End of class ConfigFilePrivate.
} // End of namespace Aki.

#endif // AKI_CONFIGFILE_P_HPP
