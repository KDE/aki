/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
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

#ifndef AKI_HPP
#define AKI_HPP

#include "libaki_export.hpp"
#include "akiversion.hpp"
#include <KDE/KDebug>
#include <KDE/KLocale>

#define AKI_DECLARE_PRIVATE(klass) \
private: \
    friend class klass##Private; \
    QScopedPointer<klass##Private> _d;

#define AKI_DECLARE_PUBLIC(klass) \
private: \
    klass* _q;

namespace Aki
{
    template<bool>
    struct StaticAssert;

    template<>
    struct StaticAssert<true>
    {
    };

#define AKI_STATIC_ASSERT(e) (StaticAssert<((e) != 0)>())
    

    LIBAKI_EXPORT QString databaseFile();
    LIBAKI_EXPORT QStringList themeDirectory();
    LIBAKI_EXPORT QStringList themeDirectoryByName(const QString& themeName);
} // End of namespace Aki.

#endif // AKI_HPP
