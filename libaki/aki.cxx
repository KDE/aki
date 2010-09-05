#include "aki.hpp"
#include <KDE/KGlobal>
#include <KDE/KStandardDirs>
#include <QDir>

QString
Aki::databaseFile()
{
    KStandardDirs dirs;
    QString str = dirs.saveLocation("data", "aki");
    return str + "akidatabase.db";
}

QStringList
Aki::themeDirectory()
{
    return KGlobal::dirs()->findDirs("appdata", "themes/");
}

QStringList
Aki::themeDirectoryByName(const QString& themeName)
{
    const QString directory = QString("themes/%1").arg(themeName);
    return KGlobal::dirs()->findDirs("appdata", directory);
}
