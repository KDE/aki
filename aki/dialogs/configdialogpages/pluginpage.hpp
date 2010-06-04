#ifndef AKI_PLUGINPAGE_HPP
#define AKI_PLUGINPAGE_HPP

#include "interfaces/isettingspage.hpp"

class KPluginSelector;
namespace Aki
{
class PluginPage : public Aki::ISettingsPage
{
    Q_OBJECT
public:
    explicit PluginPage(QWidget* parent = 0);
    ~PluginPage();
    virtual bool hasChanged() const;
    virtual void updateSettings();
private Q_SLOTS:
    void slotChanged(bool hasChanged);
    void slotChanged(const QByteArray& name);
private:
    KPluginSelector* _pluginSelector;
    bool _pluginsChanged;
}; // End of class PluginPage.
} // End of namespace Aki.

#endif // AKI_PLUGINPAGE_HPP
