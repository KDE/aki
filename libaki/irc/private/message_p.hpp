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

#ifndef AKI_IRC_MESSAGE_P_HPP
#define AKI_IRC_MESSAGE_P_HPP

#include "irc/nickinfo.hpp"
#include "irc/replycodes.hpp"
#include <QtCore/QString>
#include <QtCore/QStringList>

namespace Aki
{
namespace Irc
{
class Message;
class MessagePrivate
{
public:
    explicit MessagePrivate(Aki::Irc::Message* qq);
    /**
     * Removes string from the @p line to starting at 0 index to the first whitespace.
     *
     * @param line Modifies the string after it is done.
     *
     * @return String that was taken.
     */
    QString removeStringToFirstWhitespace(QString* line);
    /**
     * Removes the string from the @p line starting at @p start and ending at @p stop.
     *
     * @param line Modifies to string after it is done.
     * @param start Start position index.
     * @param stop Stop position index.
     *
     * @return String that was taken.
     */
    QString removeStringToFirstWhitespace(QString* line, int start, int stop);
public:
    Aki::Irc::NickInfo sender;
    QString command;
    QString message;
    QStringList params;
    Aki::Irc::ReplyCodes numeric;
    bool isNumeric;
private:
    Aki::Irc::Message* _q;
}; // End of class MessagePrivate.
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_MESSAGE_P_HPP
