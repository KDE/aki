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

#include "jsonparser.hpp"
#include "aki.hpp"
#include "debughelper.hpp"
#include "private/jsonparser_p.hpp"
#include <KDE/KDebug>
#include <qjson/serializer.h>
#include <QtCore/QVariant>
using namespace Aki;

JsonParser::JsonParser(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::JsonParserPrivate(this));
}

JsonParser::~JsonParser()
{
}

bool
JsonParser::read(QIODevice* device, SqlIdentity* identity, bool url)
{
    DEBUG_FUNC_NAME;
    Q_UNUSED(url);
    Q_UNUSED(identity);
    if (!device) {
        DEBUG_TEXT("Device not valid for Json");
        return false;
    }

    bool ok;
    QVariantMap jsonData = _d->parser.parse(device, &ok).toMap();
    if (ok) {

    }

    return ok;
}

bool
JsonParser::write(QIODevice* device, Aki::SqlIdentity* identity)
{
    DEBUG_FUNC_NAME;
    if (!device) {
        DEBUG_TEXT("Device not valid for Json");
        return false;
    }

    bool ok;

    QVariantMap jsonData;
    jsonData["version"] = "0.0.3";

    _d->identity = identity;
    _d->writeServer(&jsonData);
    _d->serializer.serialize(jsonData, device, &ok);

    return ok;
}
