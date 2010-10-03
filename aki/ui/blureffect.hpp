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

#ifndef AKI_BLUREFFECT_HPP
#define AKI_BLUREFFECT_HPP

#include <QtGui/QColor>

namespace Aki
{
class WindowNotifier;
class BlurEffect
{
public:
    static QColor colorizationColor();
    static bool enableBlurBehindWindow(QWidget* widget, bool enable = true);
    static bool extendFrameIntoClientArea(QWidget* widget, int left = -1,
                                          int right = -1, int top = -1,
                                          int bottom = -1);
    static bool isCompositionEnabled();
private:
    static Aki::WindowNotifier* windowNotifier();
}; // End of class BlurEffect.
} // End of namespace Aki.

#endif // AKI_BLUREFFECT_HPP