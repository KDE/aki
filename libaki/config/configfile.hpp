#ifndef AKI_CONFIGFILE_HPP
#define AKI_CONFIGFILE_HPP

#include "../libaki_export.hpp"
#include <KDE/KConfigGroup>
#include <QtCore/QObject>

namespace Aki
{
class ConfigFilePrivate;
/**
 * Reads and writes to a configuration file.
 */
class LIBAKI_EXPORT ConfigFile : public QObject
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
    friend class ConfigFilePrivate;
    QScopedPointer<ConfigFilePrivate> _d;
}; // End of class ConfigFile.
} // End of namespace Aki.

#endif // AKI_CONFIGFILE_HPP
