/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

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

#ifndef AKI_NETWORKSERIALIZER_H
#define AKI_NETWORKSERIALIZER_H

#include "server.h"
#include <QObject>
#include <QScopedPointer>

namespace Aki
{
class NetworkSerializerPrivate;
class NetworkSerializer : public QObject
{
    Q_OBJECT
public:
    NetworkSerializer(QObject *parent = 0);
    ~NetworkSerializer();
    bool read(const QString &fileName);
    bool write(const QString &fileName);
    Aki::ServerList servers() const;
    void setServerList(const Aki::ServerList &servers);
private:
    friend class NetworkSerializerPrivate;
    QScopedPointer<Aki::NetworkSerializerPrivate> d;
}; // End of class NetworkSerializer.
} // End of class NetworkSerializer.

#endif // AKI_NETWORKSERIALIZER_H
