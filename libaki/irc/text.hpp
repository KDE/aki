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

#ifndef AKI_IRC_TEXT_HPP
#define AKI_IRC_TEXT_HPP

#include "aki.hpp"

namespace Aki
{
namespace Irc
{
class LIBAKI_EXPORT Text
{
public:
    enum State {
        None = 0,
        Bold = 1,
        Colour = Bold << 1,
        Italic = Colour << 1,
        Strike = Italic << 1,
        Underline = Strike << 1,
        Inverse = Underline << 1
    }; // End of enum State.

    enum ColourCode {
        White = 0,
        Black,
        Blue,
        Green,
        Red,
        Brown,
        Purple,
        Orange,
        Yellow,
        LightGreen,
        Teal,
        LightCyan,
        LightBlue,
        Pink,
        Grey,
        LightGrey
    }; // End of enum Colour.

    static QString makeBold(const QString& text);
    static QString makeColour(const QString& text, Aki::Irc::Text::ColourCode foreground,
                              Aki::Irc::Text::ColourCode background = Aki::Irc::Text::White);
    static QString makeInverse(const QString& text);
    static QString makeItalic(const QString& text);
    static QString makeStrike(const QString& text);
    static QString makeUnderline(const QString& text);
}; // End of class Text.
} // End of enum Irc.
} // End of enum Aki.

#endif // AKI_IRC_TEXT_HPP
