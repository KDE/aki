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

#ifndef AKI_CONFIGFILE_HPP
#define AKI_CONFIGFILE_HPP

#include "aki.hpp"
#include <KDE/KConfigGroup>
#include <QtCore/QObject>

namespace Aki
{
class ConfigFilePrivate;
/**
 * Reads and writes to a configuration file.
 */
class LIBAKI_EXPORT ConfigFile
    : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates a new configuration if one isn't already created and
     * loads it.
     *
     * @param name Name of the configuration file.
     * @param parent Parent of the object.
     */
    explicit ConfigFile(const QString& name, QObject* parent = 0);
    /**
     * Destroys the object.
     */
    virtual ~ConfigFile();
    /**
     * Creates a new configuration group with the @p name
     * and sets it as the current group.
     *
     * @param name Name of the group.
     */
    virtual void createGroup(const QString& name);
    /**
     * Gets the current group.
     *
     * @return Current group.
     */
    KConfigGroup currentGroup();
    /**
     * Deletes a group if it exists. If the group doesn't
     * already exists this will do nothing.
     *
     * @param name Name of the group.
     */
    virtual void deleteGroup(const QString& name);
    /**
     * Gets the list of all the groups in the configuration file.
     *
     * @return List of groups in this configuration file. If no groups
     * are found it will return an empty list.
     */
    QStringList groupLists();
    /**
     * Saves the configuration file.
     */
    virtual void save();
    /**
     * Sets the current group.
     *
     * @param name Name of the group.
     */
    virtual void setCurrentGroup(const QString& name);
protected:
    AKI_DECLARE_PRIVATE(ConfigFile)
}; // End of class ConfigFile.
} // End of namespace Aki.

#endif // AKI_CONFIGFILE_HPP
