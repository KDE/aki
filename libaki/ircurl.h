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

#ifndef AKI_IRCURL_H
#define AKI_IRCURL_H

#include "libaki_export.h"
#include <KUrl>
#include <QScopedPointer>

namespace Aki
{
class IrcUrlPrivate;
class LIBAKI_EXPORT IrcUrl
{
public:
    IrcUrl();
    IrcUrl(const KUrl &url);
    IrcUrl(const IrcUrl &copy);
    ~IrcUrl();
    IrcUrl& operator=(const IrcUrl &rhs);
    void setProtocol(const QString &protocol);
    QString protocol() const;
    bool hasPort() const;
    void setPort(quint16 port);
    quint16 port() const;
    bool hasChannel() const;
    void setChannel(const QString &channel);
    QString channel() const;
    bool hasUsername() const;
    void setUsername(const QString &username);
    QString username() const;
    bool hasHost() const;
    void setHost(const QString &host);
    QString host() const;
private:
    friend class IrcUrlPrivate;
    QScopedPointer<IrcUrlPrivate> d;
}; // End of class IrcUrl.
} // End of namespace Aki.

#endif // AKI_IRCURL_H