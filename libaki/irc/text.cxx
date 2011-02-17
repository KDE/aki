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

#include "text.hpp"
using namespace Aki;
using namespace Irc;

QString
Text::makeBold(const QString& text)
{
    //return QString("\u0002%1\u0002").arg(text);
    return QString();
}

QString
Text::makeColour(const QString& text, Aki::Irc::Text::ColourCode foreground,
                 Aki::Irc::Text::ColourCode background)
{
    if (background == Aki::Irc::Text::White) {
        //return QString("\u0003%1%2\u0003").arg(QString::number(foreground), text);
    }

    //return QString("\u0003%1,%2%3\u0003").arg(QString::number(foreground), QString::number(background), text);
    return QString();
}

QString
Text::makeInverse(const QString& text)
{
    //return QString("\u0016%1\u0016").arg(text);
    return QString();
}

QString
Text::makeItalic(const QString& text)
{
    Q_UNUSED(text)
    return QString();
}

QString
Text::makeStrike(const QString& text)
{
    Q_UNUSED(text)
    return QString();
}

QString
Text::makeUnderline(const QString& text)
{
    //return QString("\u001F%1\u001F").arg(text);
    return QString();
}
