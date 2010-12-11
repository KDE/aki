#ifndef AKI_ISETTINGSPAGEPRIVATE_HPP
#define AKI_ISETTINGSPAGEPRIVATE_HPP

#include <QtCore/QObject>

class KConfigSkeleton;
namespace Aki
{
class ISettingsPage;
class ISettingsPagePrivate
{
public:
    ISettingsPagePrivate(Aki::ISettingsPage* qq);
public:
    QString name;
    QString icon;
    QString header;
    KConfigSkeleton* config;
private:
    Aki::ISettingsPage* _q;
}; // End of class ISettingsPagePrivate.
} // End of namespace Aki.

#endif // AKI_ISETTINGSPAGEPRIVATE_HPP
