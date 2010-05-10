#include "aki.hpp"
#include <KDE/KStandardDirs>

QString
Aki::databaseFile()
{
    KStandardDirs dirs;
    QString str = dirs.saveLocation("data", "aki");
    return str + "akidatabase.db";
}
