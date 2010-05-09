#include "aki.hpp"
#include <KDE/KStandardDirs>

QString
Aki::databaseFile()
{
    KStandardDirs dirs;
    QString str = dirs.saveLocation("data", QLatin1String("aki"));
    return str + QLatin1String("akidatabase.db");
}
