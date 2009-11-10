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

#include "networkserializer.h"
#include "serverlist/mircparser.h"
#include "serverlist/xmlparser.h"
#include <KDebug>
#include <KIO/NetAccess>
#include <KMessageBox>
#include <KSaveFile>
using namespace Aki;

namespace Aki
{
class NetworkSerializerPrivate
{
public:
    NetworkSerializerPrivate(Aki::NetworkSerializer *qq)
        : q(qq),
        mirc(0),
        xml(0)
    {
    }

    Aki::NetworkSerializer *q;
    Aki::MIrcParser *mirc;
    Aki::XmlParser *xml;
    Aki::ServerList servers;
}; // End of class NetworkSerializerPrivate.
} // End of namespace Aki.

NetworkSerializer::NetworkSerializer(QObject *parent)
    :QObject(parent)
{
    d.reset(new Aki::NetworkSerializerPrivate(this));
}

NetworkSerializer::~NetworkSerializer()
{
}

bool
NetworkSerializer::read(const QString &fileName)
{
    bool state = false;
    QString tmpFile;

    if (fileName.isEmpty()) {
        kError() << "No Filename";
        return state;
    }

    if (KIO::NetAccess::download(fileName, tmpFile, 0)) {
        QFile file(tmpFile);
        if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
            kError() << ("Unable to open file");
            KIO::NetAccess::removeTempFile(tmpFile);
            return state;
        }

        QString ext = fileName.right(fileName.length() - (fileName.indexOf('.') + 1));

        if (ext.toLower() == "ini") {
            d->mirc = new Aki::MIrcParser(this);
            state = d->mirc->read(&file);
            d->servers = d->mirc->servers();
            KIO::NetAccess::removeTempFile(tmpFile);
        } else if (ext.toLower() == "xml") {
            d->xml = new Aki::XmlParser(this);
            state = d->xml->read(&file);
            d->servers = d->xml->servers();
            KIO::NetAccess::removeTempFile(tmpFile);
        } else {
            kError() << ("Unknown file extension");
            KIO::NetAccess::removeTempFile(tmpFile);
            return state;
        }
    }

    KIO::NetAccess::removeTempFile(tmpFile);
    return state;
}

bool
NetworkSerializer::write(const QString &fileName)
{
    KSaveFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QString ext = fileName.right(fileName.indexOf('.'));
    if (ext.toLower() == "ini") {
        kWarning() << "mIRC Server List writing is not supported. Please use xml";
        return false;
    } else if (ext.toLower() == "xml") {
        d->xml = new Aki::XmlParser(this);
        return d->xml->write(&file);
    } else {
        kWarning() << QString("%1 extension is not support. Please use xml").arg(ext.toLower());
        return false;
    }

    return false;
}

Aki::ServerList
NetworkSerializer::servers() const
{
    return d->servers;
}

void
NetworkSerializer::setServerList(const Aki::ServerList &servers)
{
    d->servers = servers;
}
