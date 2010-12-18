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

#ifndef AKI_IRC_REPLYINFO_HPP
#define AKI_IRC_REPLYINFO_HPP

#include "aki.hpp"
#include "irc/nickinfo.hpp"
#include "irc/replycodes.hpp"
#include <QtCore/QStringList>

namespace Aki
{
namespace Irc
{
class ReplyInfoPrivate;
class LIBAKI_EXPORT ReplyInfo
{
public:
    ReplyInfo();
    explicit ReplyInfo(const Aki::Irc::NickInfo& sender, const QString& command, const QString& message,
                       const QStringList& params = QStringList(), Aki::Irc::ReplyCodes replyCode = Aki::Irc::RPL_NULL);
    ReplyInfo(const Aki::Irc::ReplyInfo& other);
    ~ReplyInfo();
    Aki::Irc::ReplyInfo& operator=(const Aki::Irc::ReplyInfo& other);
    QString command() const;
    bool isNumeric() const;
    QString message() const;
    Aki::Irc::ReplyCodes numeric() const;
    QStringList params() const;
    Aki::Irc::NickInfo sender() const;
private:
    QSharedDataPointer<Aki::Irc::ReplyInfoPrivate> _d;
}; // End of class ReplyInfo.
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_REPLYINFO_HPP
