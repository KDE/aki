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

#ifndef AKI_SERVERVIEWTABPRIVATE_HPP
#define AKI_SERVERVIEWTABPRIVATE_HPP

#include <QtCore/QObject>

class QSplitter;

namespace Aki
{
namespace Irc
{
class Socket;
} // End of namespace Irc.
class ChannelContainer;
class ServerViewTab;
class ServerViewTabPrivate
{
public:
    explicit ServerViewTabPrivate(Aki::ServerViewTab* qq);
public:
    Aki::Irc::Socket* socket;
    Aki::ChannelContainer* channelContainer;
    QSplitter* verticalSplitter;
private:
    Aki::ServerViewTab* _q;
}; // End of class ServerViewTabPrivate.
} // End of namespace Aki.

#endif // AKI_SERVERVIEWTABPRIVATE_HPP
