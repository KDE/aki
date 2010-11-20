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

#ifndef AKI_IRC_BASEREPLY_HPP
#define AKI_IRC_BASEREPLY_HPP

#include "aki.hpp"
#include "irc/nickinfo.hpp"
#include "irc/replycodes.hpp"
#include <QtCore/QSharedDataPointer>
#include <QtCore/QStringList>

namespace Aki
{
namespace Irc
{
class BaseReplyPrivate;
class LIBAKI_EXPORT BaseReply
{
public:
    BaseReply();
    BaseReply (const Aki::Irc::BaseReply& other);
    virtual ~BaseReply();
    virtual Aki::Irc::BaseReply& operator=(const Aki::Irc::BaseReply& other);
    virtual bool operator==(const Aki::Irc::BaseReply& other) const;
    bool isNumeric() const;
    QString command() const;
    QString message() const;
    QStringList params() const;
    static Aki::Irc::BaseReply parse(const QString& line);
    Aki::Irc::ReplyCodes replyCode() const;
    Aki::Irc::NickInfo sender() const;
private:
    QSharedDataPointer<Aki::Irc::BaseReplyPrivate> _d;
}; // End of class BaseReply.
} // End of namespace Irc.
} // End of namespace Aki.

QDebug LIBAKI_EXPORT operator<<(QDebug dbg, const Aki::Irc::BaseReply& reply);

#endif // AKI_IRC_BASEREPLY_HPP
