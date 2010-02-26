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

#include "ircurl.h"
#include <Aki/Irc/Rfc2812>
using namespace Aki;

namespace Aki
{
class IrcUrlPrivate
{
public:
    IrcUrlPrivate()
    {
        url.setProtocol("irc");
    }

    KUrl url;
}; // End of class IrcUrlPrivate.
} // End of namespace Aki.

IrcUrl::IrcUrl()
{
    d.reset(new Aki::IrcUrlPrivate);
}

IrcUrl::IrcUrl(const Aki::IrcUrl &copy)
{
    Q_UNUSED(copy)
    d.reset(new Aki::IrcUrlPrivate);
}

IrcUrl::~IrcUrl()
{
}

Aki::IrcUrl&
IrcUrl::operator=(const Aki::IrcUrl &rhs)
{
    Q_UNUSED(rhs);
    d.reset(new Aki::IrcUrlPrivate);
    return *this;
}

void
IrcUrl::setProtocol(const QString &protocol)
{
    QString proto = protocol.toLower();

    if (proto == "irc" || proto == "irc6" || proto == "ircs" ||
        proto == "ircs6") {
        d->url.setProtocol(protocol);
    }
}

QString
IrcUrl::protocol() const
{
    return d->url.protocol();
}

void
IrcUrl::setPort(quint16 port)
{
    d->url.setPort(port);
}

quint16
IrcUrl::port() const
{
    return d->url.port();
}

void
IrcUrl::setChannel(const QString &channel)
{
    if (channel.isEmpty() || channel.isNull() ||
        channel.contains(' ')) {
        return;
    }

    d->url.setPath(channel);
}

QString
IrcUrl::channel() const
{
    return d->url.path(KUrl::RemoveTrailingSlash).remove('/');
}

bool
IrcUrl::hasHost() const
{
    return d->url.hasHost();
}

QString
IrcUrl::host() const
{
    return d->url.host();
}

void
IrcUrl::setHost(const QString &host)
{
    d->url.setHost(host);
}

bool
IrcUrl::hasUsername() const
{
    return d->url.hasUser();
}

void
IrcUrl::setUsername(const QString &username)
{
    d->url.setUserName(username);
}

QString
IrcUrl::username() const
{
    return d->url.userName();
}
