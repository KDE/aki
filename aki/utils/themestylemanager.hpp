#ifndef AKI_THEMESTYLEMANAGER_HPP
#define AKI_THEMESTYLEMANAGER_HPP

#include "aki.hpp"
#include <KDE/KFileItemList>
#include <KDE/KUrl>
#include <QtCore/QHash>

class KArchive;
class KArchiveDirectory;
class KDirLister;

namespace Aki
{
class ThemeStyle;
class ThemeStyleManagerPrivate;
class ThemeStyleManager : public QObject
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
         * Theme archive unable to open.
         */
        UnableToOpenThemeArchive,
        /**
         * User doesn't have write permission to the theme folder.
         */
        NoPermissions,
        /**
         * No theme path available to install to.
         */
        InvalidThemePath,
        /**
         * No valid themes found.
         */
        NoThemesFound
    }; // End of enum Result.
    static ThemeStyleManager* self();
    Aki::ThemeStyleManager::Result installTheme(const QString& themePath);
    bool removeTheme(const QString& theme);
    Aki::ThemeStyle* themeForName(const QString& themeName) const;
    QStringList themeList() const;
Q_SIGNALS:
    void newThemeAvailable(const QString& name);
    void themeUpdatesCompleted();
private Q_SLOTS:
    void slotCompleted(const KUrl& directoryUrl);
    void slotItemsAdded(const KUrl& directoryUrl, const KFileItemList& items);
private:
    ThemeStyleManager();
    ~ThemeStyleManager();
    bool isValidArchive(const KArchiveDirectory* rootDirectory) const;
    KArchive* archiveType(const QString& archive) const;
    void loadThemes();
private:
    KDirLister* _watcher;
    QHash<QString, Aki::ThemeStyle*> _themeHash;
    AKI_DECLARE_PRIVATE(ThemeStyleManager);
}; // End of class ThemeStyleManager.
} // End of namespace Aki.

#endif // AKI_THEMESTYLEMANAGER_HPP
