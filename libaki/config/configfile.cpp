/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "configfile.h"
using namespace Aki;

ConfigFile::ConfigFile(const QString &name, QObject *parent)
    : QObject(parent),
    m_configPtr(KSharedConfig::openConfig(name, KConfig::NoGlobals))
{
}

ConfigFile::~ConfigFile()
{
}

void
ConfigFile::createGroup(const QString &name)
{
    if (!m_configPtr->hasGroup(name)) {
        m_currentGroup = KConfigGroup(m_configPtr, name);
    }
}

KConfigGroup
ConfigFile::currentGroup()
{
    return m_currentGroup;
}

void
ConfigFile::deleteGroup(const QString &name)
{
    if (m_configPtr->hasGroup(name)) {
        m_configPtr->deleteGroup(name);
        setCurrentGroup("");
    }
}

QStringList
ConfigFile::groupList()
{
    return m_configPtr->groupList();
}

void
ConfigFile::save()
{
    m_configPtr->reparseConfiguration();
}

void
ConfigFile::setCurrentGroup(const QString &name)
{
    m_currentGroup = m_configPtr->group(name);
}
