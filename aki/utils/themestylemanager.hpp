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

#ifndef AKI_THEMESTYLEMANAGER_HPP
#define AKI_THEMESTYLEMANAGER_HPP

#include "aki.hpp"
#include "singleton.hpp"
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
class ThemeStyleManager : public Aki::Singleton<ThemeStyleManager>
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
    Aki::ThemeStyleManager::Result installTheme(const QString& themePath);
    bool removeTheme(const QString& theme);
    Aki::ThemeStyle* themeForName(const QString& themeName) const;
    QStringList themeList() const;
Q_SIGNALS:
    void newThemeAvailable(const QString& name);
    void themeUpdatesCompleted();
private Q_SLOTS:
    void slotCompleted();
    void slotItemsAdded(const KFileItemList& items);
private:
    friend class Aki::Singleton<ThemeStyleManager>;

    ThemeStyleManager();
    ~ThemeStyleManager();
    bool isValidArchive(const KArchiveDirectory* rootDirectory) const;
    KArchive* archiveType(const QString& archive) const;
    void loadThemes();
private:
    KDirLister* _watcher;
    QHash<QString, Aki::ThemeStyle*> _themeHash;
}; // End of class ThemeStyleManager.
} // End of namespace Aki.

#endif // AKI_THEMESTYLEMANAGER_HPP
