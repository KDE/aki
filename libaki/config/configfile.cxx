#include "configfile.hpp"
#include "private/configfile_p.hpp"
using namespace Aki;

ConfigFile::ConfigFile(const QString& name, QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::ConfigFilePrivate(this));
    _d->config = KSharedConfig::openConfig(name, KConfig::NoGlobals);
}

ConfigFile::~ConfigFile()
{
}

void
ConfigFile::createGroup(const QString& name)
{
    if (!_d->config->hasGroup(name)) {
        _d->currentGroup = KConfigGroup(_d->config, name);
    }
}

KConfigGroup
ConfigFile::currentGroup()
{
    return _d->currentGroup;
}

void
ConfigFile::deleteGroup(const QString& name)
{
    if (!_d->config->hasGroup(name)) {
        _d->config->deleteGroup(name);
        setCurrentGroup(QString());
    }
}

QStringList
ConfigFile::groupLists()
{
    return _d->config->groupList();
}

void
ConfigFile::save()
{
    _d->config->reparseConfiguration();
}

void
ConfigFile::setCurrentGroup(const QString& name)
{
    _d->currentGroup = _d->config->group(name);
}
