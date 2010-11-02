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

#ifndef AKI_IRC_MESSAGE_HPP
#define AKI_IRC_MESSAGE_HPP

#include "aki.hpp"
#include "nickinfo.hpp"
#include "replycodes.hpp"
#include <QtCore/QObject>
#include <QtCore/QStringList>

class QTextCodec;
namespace Aki
{
namespace Irc
{
class MessagePrivate;
class LIBAKI_EXPORT Message
{
public:
    Message();
    explicit Message(const QString& line);
    Message(const Aki::Irc::Message& message);
    Aki::Irc::Message& operator=(const Aki::Irc::Message& rhs);
    virtual ~Message();
    bool isNumeric() const;
    QString command() const;
    QString message() const;
    Aki::Irc::ReplyCodes numeric() const;
    QStringList params() const;
    void parse(const QString& str);
    Aki::Irc::NickInfo sender() const;
private:
    AKI_DECLARE_PRIVATE(Message)
}; // End of class Message.
} // End of namespace Irc.
} // End of namespace Aki.

Q_DECLARE_METATYPE(Aki::Irc::Message)

QDebug LIBAKI_EXPORT operator<<(QDebug dbg, const Aki::Irc::Message& message);

#endif // AKI_IRC_MESSAGE_HPP
