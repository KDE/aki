/*
    This file is part of Aki IRC.
    Copyright 2009 - 2010 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef EVENTITEM_H
#define EVENTITEM_H

#include <KDateTime>
#include <QMetaType>
#include <QScopedPointer>
#include <QString>

namespace Aki
{
namespace Irc
{
class NickInfo;
} // End of namespace Irc.
} // End of namespace Aki.

class EventItemPrivate;
class EventItem
{
public:
    enum EventType {
        UnknownEvent = 0,
        ConnectEvent,
        DisconnectEvent,
        KickedEvent,
        NoticeEvent,
        PrivmsgEvent,
        CtcpPingEvent,
        CtcpVersionEvent,
        CtcpClientInfoEvent,
        CtcpUserInfoEvent,
        CtcpDccEvent,
        CtcpSourceEvent,
        CtcpTimeEvent,
        HighlightEvent,
        WallOpsEvent
    }; // End of enum EventType.

    EventItem();
    EventItem(const EventItem &copy);
    ~EventItem();
    EventItem& operator=(const EventItem &rhs);
    void setEvent(EventItem::EventType type);
    EventItem::EventType event() const;
    void setServer(const QString &server);
    QString server() const;
    void setChannel(const QString &channel);
    QString channel() const;
    void setFrom(const Aki::Irc::NickInfo &from);
    Aki::Irc::NickInfo from() const;
    KDateTime time() const;
    void setMessage(const QString &message);
    QString message() const;
private:
    friend class EventItemPrivate;
    QScopedPointer<EventItemPrivate> d;
}; // End of class EventItem.

Q_DECLARE_METATYPE(EventItem)

#endif // EVENTITEM_H
