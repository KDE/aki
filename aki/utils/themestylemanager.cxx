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

#include "themestylemanager.hpp"
#include "aki.hpp"
#include "debughelper.hpp"
#include "utils/themearchive.hpp"
#include "utils/themestyle.hpp"
#include <KDE/KApplication>
#include <KDE/KDirLister>
#include <KDE/KGlobal>
#include <KDE/KIO/NetAccess>
#include <KDE/KMessageBox>
#include <KDE/KStandardDirs>
#include <QtCore/QFileInfo>
using namespace Aki;

const QDir::Filters DIR_FILTERS = QDir::AllDirs | QDir::NoDotAndDotDot;

namespace Aki
{
class ThemeStyleManagerPrivate
{
public:
    Aki::ThemeStyleManager instance;
}; // End of class ThemeStyleManagerPrivate.
} // End of namespace Aki.
K_GLOBAL_STATIC(Aki::ThemeStyleManagerPrivate, themeStyleManager)

ThemeStyleManager::ThemeStyleManager()
    : QObject()
{
    DEBUG_FUNC_NAME
    // Search for the themes directory, if it is not found create it.
    KStandardDirs::locateLocal("appdata", "themes/");

    _watcher = new KDirLister(this);
    _watcher->setDirOnlyMode(true);

    connect(_watcher, SIGNAL(newItems(KFileItemList)),
            SLOT(slotItemsAdded(KFileItemList)));
    connect(_watcher, SIGNAL(completed()),
            SLOT(slotCompleted()));

    const QStringList themeList = Aki::themeDirectory();
    foreach (const QString& theme, themeList) {
        DEBUG_TEXT2("Opening theme direction: %1", theme)
        _watcher->openUrl(KUrl(theme), KDirLister::Keep);
    }
}

ThemeStyleManager::~ThemeStyleManager()
{
    qRemovePostRoutine(themeStyleManager.destroy);
}

Aki::ThemeStyleManager::Result
ThemeStyleManager::installTheme(const QString& themePath)
{
    QString baseThemePath;
    const QStringList themeList = Aki::themeDirectory();

    if (themeList.isEmpty()) {
        return Aki::ThemeStyleManager::InvalidThemePath;
    }

    foreach (const QString& path, themeList) {
        if (QFileInfo(path).isWritable()) {
            baseThemePath = path;
            break;
        }
    }

    if (baseThemePath.isEmpty() || baseThemePath.isNull()) {
        return Aki::ThemeStyleManager::NoPermissions;
    }

    QScopedPointer<Aki::ThemeArchive> archive(new Aki::ThemeArchive(themePath));
    if (archive.isNull() || !archive->isOpen()) {
        return Aki::ThemeStyleManager::UnableToOpenThemeArchive;
    }

    const Aki::ThemeArchive::Result result = archive->installTheme(KUrl(baseThemePath));
    switch (result) {
    case Aki::ThemeArchive::NoThemesFound: {
        return Aki::ThemeStyleManager::NoThemesFound;
        break;
    }
    case Aki::ThemeArchive::ThemeAlreadyExist: {
        return Aki::ThemeStyleManager::ThemeAlreadyExist;
        break;
    }
    default: {
        break;
    }
    }

    return Aki::ThemeStyleManager::Success;
}

bool
ThemeStyleManager::removeTheme(const QString& theme)
{
    if (theme.isEmpty() || theme.isNull()) {
        return false;
    }

    if (!_themeHash.contains(theme)) {
        return false;
    }

    QScopedPointer<Aki::ThemeStyle> themeStyle(_themeHash.take(theme));
    if (theme.isNull()) {
        return false;
    }

    const QStringList themeDirectories = Aki::themeDirectoryByName(theme + ".AdiumMessageStyle");
    if (themeDirectories.isEmpty()) {
        return false;
    }

    bool state = false;
    foreach (const QString& dir, themeDirectories) {
        state = KIO::NetAccess::del(KUrl(dir), 0);
    }

    return state;
}

Aki::ThemeStyleManager*
ThemeStyleManager::self()
{
    return &themeStyleManager->instance;
}

void
ThemeStyleManager::slotCompleted()
{
    emit themeUpdatesCompleted();
}

void
ThemeStyleManager::slotItemsAdded(const KFileItemList& items)
{
    DEBUG_FUNC_NAME
    foreach (const KFileItem& item, items) {
        DEBUG_TEXT(item.url().url())
        QString themeName = item.url().fileName().remove(".AdiumMessageStyle");
        if (_themeHash.contains(themeName)) {
            _themeHash.value(themeName)->reloadTheme();
        } else {
            Aki::ThemeStyle* theme = new Aki::ThemeStyle(item.url().fileName(), this);
            _themeHash.insert(themeName, theme);
        }
    }
}

Aki::ThemeStyle*
ThemeStyleManager::themeForName(const QString& themeName) const
{
    return _themeHash[themeName];
}

QStringList
ThemeStyleManager::themeList() const
{
    return _themeHash.keys();
}
