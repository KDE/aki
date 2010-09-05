#ifndef AKI_THEMEARCHIVE_HPP
#define AKI_THEMEARCHIVE_HPP

#include "aki.hpp"
#include <KDE/KArchive>
#include <KDE/KUrl>
#include <QtCore/QObject>

class KArchiveDeleter
{
public:
    static inline void cleanup(KArchive* pointer)
    {
        if (pointer) {
            pointer->close();
            delete pointer;
        }
    }
}; // End of class KArchiveDeleter.
typedef QScopedPointer<KArchive, KArchiveDeleter> KArchivePointer;

namespace Aki
{
/**
 * Archive class for handling theme archives. Supports adium theme
 * archives.
 */
class ThemeArchive : public QObject
{
    Q_OBJECT
public:
    /**
     * Status results when installTheme is called.
     */
    enum Result {
        /**
         * Theme was successfully installed.
         */
        Success,
        /**
         * Theme already exists.
         */
        ThemeAlreadyExist,
        /**
         * No valid themes found.
         */
        NoThemesFound
    }; // End of enum Result.

    /**
     * Create a ThemeArchive object.
     *
     * @p parent Parent of the object.
     */
    explicit ThemeArchive(QObject* parent = 0);
    /**
     * Create a ThemeArchive object with a @p path to the file and attempts opening.
     *
     * @p path Path to the archive.
     * @p parent Parent of the object.
     */
    explicit ThemeArchive(const QString& path, QObject* parent = 0);
    /**
     * Destroys the object.
     */
    virtual ~ThemeArchive();
    /**
     * Installs the theme in the directory if it contains a valid theme or themes.
     * 
     */
    Aki::ThemeArchive::Result installTheme(const KUrl& path);
    /**
     * Check to see if the archive is open or not.
     *
     * @return True if the archive is opened; false otherwise.
     */
    bool isOpen() const;
    /**
     * Checks to see if the archive contains a valid theme. Even if there is more
     * than one theme and if one is invalid and the other is valid, this will return true.
     *
     * @return True if the archive contains a valid theme; false otherwise.
     */
    bool isValidArchive();
    /**
     * Opens the archive that is at the given @p path. There is no need to call this
     * if you specify the path in the constructor, as the constructor automatically calls
     * this function.
     *
     * @p path Path to the archive.
     * @return True if the archive is open; false otherwise.
     */
    bool open(const QString& path);
private:
    const KArchiveDirectory* rootDirectory();
    bool isValidTheme(const KArchiveDirectory* directory);
    bool isTarArchive() const;
    bool isZipArchive() const;
private:
    QString _mimeType;
    KArchivePointer _archive;
}; // End of class ThemeArchive.
} // End of namespace Aki.

#endif // AKI_THEMEARCHIVE_HPP
