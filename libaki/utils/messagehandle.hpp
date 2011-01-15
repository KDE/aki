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

#ifndef AKI_MESSAGEHANDLE_HPP
#define AKI_MESSAGEHANDLE_HPP

#include "irc/replycodes.hpp"
#include <KDE/KAction>

namespace Aki
{
class MessageHandlePrivate;
class MessageHandle
    : public KAction
{
    Q_OBJECT
public:
    enum Type
    {
        System,
        Plugin
    }; // End of enum Type.
    explicit MessageHandle(QObject* parent, Aki::Irc::ReplyCodes replyCode, const char* slot,
                           const QString& help = QString(), Aki::MessageHandle::Type type = Aki::MessageHandle::System);
    virtual ~MessageHandle();
    Aki::Irc::ReplyCodes replyCode();
    QString help() const;
    Aki::MessageHandle::Type type() const;
private:
    QString _help;
    Aki::Irc::ReplyCodes _replyCode;
    Aki::MessageHandle::Type _type;
}; // End of class MessageHandle.
} // End of namespace Aki.

#endif // AKI_MESSAGEHANDLE_HPP
