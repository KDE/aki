#ifndef AKI_HPP
#define AKI_HPP

#include "libaki_export.hpp"
#include "akiversion.hpp"
#include <KDE/KDebug>
#include <KDE/KLocale>
#include <QxtCore/QxtLogger>

#define AKI_DECLARE_PRIVATE(klass) \
    friend class klass##Private; \
    QScopedPointer<klass##Private> _d;

namespace Aki
{
    LIBAKI_EXPORT QString databaseFile();
    LIBAKI_EXPORT QStringList themeDirectory();
    LIBAKI_EXPORT QStringList themeDirectoryByName(const QString& themeName);
} // End of namespace Aki.

#endif // AKI_HPP
