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

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include "libaki_export.h"
#include <QObject>
#include <KConfigGroup>
#include <KSharedConfig>

class KConfigGroup;
namespace Aki
{
/**
 * Reads and writes to a configuration file.
 */
class LIBAKI_EXPORT ConfigFile : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates a new configuration if one isn't already created.
     *
     * @param name Name of the configuration file.
     * @param parent Parent of the object.
     */
    ConfigFile(const QString &name, QObject *parent = 0);
    /**
     * Destroys the object.
     */
    virtual ~ConfigFile();
    /**
     * Creates a new configuration group with the name @p name
     *
     * @param name Name of the group.
     */
    virtual void createGroup(const QString &name);
    /**
     * Gets the current group.
     */
    KConfigGroup currentGroup();
    /**
     * Deletes a group if it exists else this does nothing.
     *
     * @param name Name of the group.
     */
    virtual void deleteGroup(const QString &name);
    /**
     * Gets the list of all the groups in the configuration file.
     */
    QStringList groupList();
    /**
     * Saves the current group.
     */
    virtual void save();
    /**
     * Changes the current group to the @param name name given.
     */
    virtual void setCurrentGroup(const QString &name);
protected:
    KSharedConfigPtr m_configPtr;
    KConfigGroup m_currentGroup;
}; // End of class ConfigFile.
} // End of namespace Aki.

#endif // CONFIGFILE_H
