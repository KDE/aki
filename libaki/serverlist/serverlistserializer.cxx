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

#include "serverlistserializer.hpp"
#include "private/serverlistserializer_p.hpp"
#include "sql/database.hpp"
#include "sql/identity.hpp"
using namespace Aki;

ServerListSerializer::ServerListSerializer(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::ServerListSerializerPrivate(this));
    _d->jsonParser = new Aki::JsonParser(this);
    _d->xchatParser = new Aki::XChatParser(this);
}

ServerListSerializer::~ServerListSerializer()
{
}

Aki::Sql::Identity*
ServerListSerializer::identity() const
{
    return _d->identity;
}

bool
ServerListSerializer::read(const QString& file)
{
    if (file.endsWith(".json", Qt::CaseInsensitive)) {
        return _d->jsonParser->read(file);
    } else if (file.endsWith(".conf")) {
        return _d->xchatParser->read(file);
    }
    return false;
}

void
ServerListSerializer::setDatabase(Aki::Sql::Database* database)
{
    _d->jsonParser->setDatabase(database);
    _d->xchatParser->setDatabase(database);
}

void
ServerListSerializer::setIdentity(Aki::Sql::Identity* identity)
{
    _d->identity = identity;
    _d->jsonParser->setIdentity(identity);
    _d->xchatParser->setIdentity(identity);
}

bool
ServerListSerializer::write(const QString& file)
{
    return _d->jsonParser->write(file);
}


#include "serverlist/serverlistserializer.moc"
