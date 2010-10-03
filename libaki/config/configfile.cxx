/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

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
