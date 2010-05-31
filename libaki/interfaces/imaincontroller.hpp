#ifndef IMAINCONTROLLER_HPP
#define IMAINCONTROLLER_HPP

#include "libaki_export.hpp"

namespace Aki
{
class ISettingsPage;
class Plugin;
class LIBAKI_EXPORT IMainController
{
public:
    virtual ~IMainController();
    virtual void addGui(Aki::Plugin* plugin) = 0;
    virtual void addSettingsPage(Aki::ISettingsPage* page) = 0;
    virtual void removeGui(Aki::Plugin* plugin) = 0;
    virtual void removeSettingsPage(Aki::ISettingsPage* page) = 0;
}; // End of class IMainController.
} // End of namespace Aki.

#endif // IMAINCONTROLLER_HPP
