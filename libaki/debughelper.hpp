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

#ifndef AKI_DEBUGHELPER_HPP
#define AKI_DEBUGHELPER_HPP

#include "aki.hpp"
#include <QtCore/QString>

#ifndef QT_NO_DEBUG
#   define DEBUG_FUNC_NAME Aki::DebugHelper dbgHelper(Q_FUNC_INFO);
#   define DEBUG_TEXT(A) dbgHelper.addText(A);
#   define DEBUG_TEXT2(A, B) dbgHelper.addText(QString(A).arg(B));
#   define DEBUG_TEXT3(A, B, C) dbgHelper.addText(QString(A).arg(B).arg(C));
#   define DEBUG_TEXT4(A, B, C, D) dbgHelper.addText(QString(A).arg(B).arg(C).arg(D));
#   define DEBUG_TEXT5(A, B, C, D, E) dbgHelper.addText(QString(A).arg(B).arg(C).arg(D).arg(E));
#   define DEBUG_TEXT6(A, B, C, D, E, F) dbgHelper.addText(QString(A).arg(B).arg(C).arg(D).arg(E).arg(F));
#else
#   define DEBUG_FUNC_NAME
#   define DEBUG_TEXT(A)
#   define DEBUG_TEXT2(A, B)
#   define DEBUG_TEXT3(A, B, C)
#   define DEBUG_TEXT4(A, B, C, D)
#   define DEBUG_TEXT5(A, B, C, D, E)
#   define DEBUG_TEXT6(A, B, C, D, E, F)
#endif

namespace Aki
{
class DebugHelperPrivate;
class LIBAKI_EXPORT DebugHelper
{
public:
    explicit DebugHelper(const QString &t);
    ~DebugHelper();
    void addText(const QString& text);
private:
    AKI_DECLARE_PRIVATE(DebugHelper)
}; // End of class DebugHelper.
} // End of namespace Aki.

#endif // AKI_DEBUGHELPER_HPP
