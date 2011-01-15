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

#include "parser.hpp"
#include "debughelper.hpp"
#include "private/parser_p.hpp"
#include <QtCore/QFile>
#include <QtXml/QXmlStreamReader>
using namespace Aki;
using namespace Upnp;

Parser::Parser()
    : _d(new Aki::Upnp::ParserPrivate)
{
}

Parser::~Parser()
{
}

Aki::Upnp::Parser&
Parser::operator=(const Aki::Upnp::Parser& other)
{
    _d = other._d;
    return *this;
}

bool
Parser::parse(const QString& file, Aki::Upnp::Router* router)
{
    DEBUG_FUNC_NAME;
    Q_ASSERT(router);
    _d->router = router;

    if (file.isEmpty()) {
        DEBUG_TEXT("Unable to parse xml file: File name is empty");
        return false;
    }

    QFile f(file);
    if (!f.open(QIODevice::ReadOnly)) {
        DEBUG_TEXT2("Unable to open file: %1", file);
        DEBUG_TEXT2("Error: %1", f.errorString());
        return false;
    }

    _d->xml.setDevice(&f);

    if (_d->xml.hasError()) {
        DEBUG_TEXT2("Unable to parser xml file: %1", _d->xml.errorString());
        return false;
    }

    if (_d->xml.readNextStartElement()) {
        if (_d->xml.name() == "root") {
            return _d->readRoot();
        }
    }

    return false;
}

bool
Parser::parse(const QByteArray& data, Aki::Upnp::Router* router)
{
    DEBUG_FUNC_NAME;
    Q_ASSERT(router);
    _d->router = router;

    if (data.isEmpty()) {
        DEBUG_TEXT("Unable to parse xml file: Empty data");
        return false;
    }

    _d->xml.addData(data);

    if (_d->xml.hasError()) {
        DEBUG_TEXT2("Unable to parse xml file: %1", _d->xml.errorString());
        return false;
    }

    if (_d->xml.readNextStartElement()) {
        if (_d->xml.name() == "root") {
            return _d->readRoot();
        }
    }

    return false;
}
