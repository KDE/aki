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

#ifndef AKI_IRC_NICKINFO_HPP
#define AKI_IRC_NICKINFO_HPP

#include "aki.hpp"

namespace Aki
{
namespace Irc
{
class NickInfoPrivate;
class LIBAKI_EXPORT NickInfo
{
public:
    NickInfo();
    explicit NickInfo(const QString& hostmask);
    NickInfo(const Aki::Irc::NickInfo& other);
    ~NickInfo();
    Aki::Irc::NickInfo& operator=(const Aki::Irc::NickInfo& rhs);
    QString host() const;
    QString hostmask() const;
    QString nick() const;
    void setHostmask(const QString& hostmask);
    QString user() const;
private:
    QSharedDataPointer<Aki::Irc::NickInfoPrivate> _d;
}; // End of class NickInfo.
} // End of namespace Irc.
} // End of namespace Aki.

Q_DECLARE_METATYPE(Aki::Irc::NickInfo)

QDebug operator<<(QDebug dbg, const Aki::Irc::NickInfo& nickInfo);

#endif // AKI_IRC_NICKINFO_HPP
