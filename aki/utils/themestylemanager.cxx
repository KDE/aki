#include "themestylemanager.hpp"
#include "aki.hpp"
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
    // Search for the themes directory, if it is not found create it.
    KStandardDirs::locateLocal("appdata", "themes/");

    _watcher = new KDirLister(this);
    _watcher->setDirOnlyMode(true);

    const QStringList themeList = Aki::themeDirectory();
    foreach (const QString& theme, themeList) {
        _watcher->openUrl(KUrl(theme), KDirLister::Keep);
    }

    connect(_watcher, SIGNAL(itemsAdded(KUrl,KFileItemList)),
            SLOT(slotItemsAdded(KUrl,KFileItemList)));
    connect(_watcher, SIGNAL(completed(KUrl)),
            SLOT(slotCompleted(KUrl)));
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
ThemeStyleManager::slotCompleted(const KUrl&)
{
    emit themeUpdatesCompleted();
}

void
ThemeStyleManager::slotItemsAdded(const KUrl&, const KFileItemList& items)
{
    foreach (const KFileItem& item, items) {
        QString themeName = item.url().fileName().remove(".AdiumMessageStyle");
        if (_themeHash.contains(themeName)) {
            _themeHash.value(themeName)->reloadTheme();
        } else {
            Aki::ThemeStyle* theme = new Aki::ThemeStyle(item.url().fileName(), this);
            _themeHash.insert(themeName, theme);
            emit newThemeAvailable(themeName);
        }
    }
}

Aki::ThemeStyle*
ThemeStyleManager::themeForName(const QString& themeName) const
{
    return _themeHash[themeName];
}
