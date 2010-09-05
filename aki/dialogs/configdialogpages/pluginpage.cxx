#include "pluginpage.hpp"
#include "aki.hpp"
#include "settings.h"
#include "utils/pluginmanager.hpp"
#include <KDE/KPluginSelector>
#include <QtGui/QVBoxLayout>
using namespace Aki;

PluginPage::PluginPage(QWidget* parent)
    : Aki::ISettingsPage(i18n("Plugins"), Aki::Settings::self(),
                         "preferences-plugin", i18n("Configure Plugin Settings"),
                         parent),
    _pluginsChanged(false)
{
    _pluginSelector = new KPluginSelector(this);
    connect(_pluginSelector, SIGNAL(changed(bool)),
            SLOT(slotChanged(bool)));
    connect(_pluginSelector, SIGNAL(configCommitted(QByteArray)),
            SLOT(slotChanged(QByteArray)));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_pluginSelector);
    setLayout(layout);

    _pluginSelector->addPlugins(Aki::PluginManager::self()->pluginInfos(),
                                KPluginSelector::IgnoreConfigFile, i18n("Plugins"),
                                "Plugins");
    _pluginSelector->updatePluginsState();
}

PluginPage::~PluginPage()
{
}

bool
PluginPage::hasChanged() const
{
    return _pluginsChanged;
}

void
PluginPage::slotChanged(bool)
{
    if (_pluginsChanged) {
        _pluginSelector->updatePluginsState();
        Aki::PluginManager::self()->loadPlugins();
    }
    emit widgetsModified();
}

void
PluginPage::slotChanged(const QByteArray&)
{
    _pluginsChanged = true;
}

void
PluginPage::updateSettings()
{
    if (_pluginsChanged) {
        _pluginSelector->save();
    }
}
