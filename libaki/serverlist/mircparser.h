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

#ifndef MIRCPARSER_H
#define MIRCPARSER_H

#include "server.h"
#include <QObject>
#include <QScopedPointer>

namespace Aki
{
class MIrcParserPrivate;
class MIrcParser : public QObject
{
    Q_OBJECT
public:
    MIrcParser(QObject *parent = 0);
    ~MIrcParser();
    bool read(QIODevice *device);
    Aki::ServerList servers() const;
private:
    friend class MIrcParserPrivate;
    QScopedPointer<MIrcParserPrivate> d;
}; // End of class MIrcParser.
} // End of namespace Aki.

#endif // MIRCPARSER_H
