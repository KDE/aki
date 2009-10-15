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

#include "networkexporter.h"
#include "server.h"
#include <QIODevice>
#include <QTextCodec>
#include <QXmlStreamWriter>
using namespace Aki;

namespace Aki
{
class NetworkExporterPrivate
{
public:
    NetworkExporterPrivate()
    {
    }

    QXmlStreamWriter stream;
    QList<QSharedPointer<Aki::Server> > serverList;
}; // End of class NetworkExporterPrivate.
} // End of namespace Aki.

NetworkExporter::NetworkExporter()
    : d(new NetworkExporterPrivate)
{
    d->stream.setAutoFormatting(true);
}

NetworkExporter::~NetworkExporter()
{
}

void
NetworkExporter::setServerList(const QList<QSharedPointer<Aki::Server> > &list)
{
    d->serverList = list;
}

bool
NetworkExporter::write(QIODevice *device)
{
    if (!device || d->serverList.isEmpty()) {
        return false;
    }

    d->stream.setDevice(device);
    d->stream.setCodec(QTextCodec::codecForName("UTF-8"));

    d->stream.writeStartDocument();
    d->stream.writeDTD("<!DOCTYPE network>");
    d->stream.writeStartElement("http://aki.kde.org/aki", "servers");
    d->stream.writeAttribute("xmlns:network", "http://aki.kde.org/aki/network");

    foreach (Aki::ServerPointer s, d->serverList) {
        d->stream.writeStartElement("network");

        d->stream.writeTextElement("network:name", s->name());

        d->stream.writeStartElement("network:reconnection");
        d->stream.writeAttribute("enable", QString::number(s->isAutoReconnectionEnabled()));
        d->stream.writeEndElement(); // End of reconnection element.

        d->stream.writeTextElement("network:retryInterval", QString::number(s->retryInterval()));
        d->stream.writeTextElement("network:retryAttempts", QString::number(s->retryAttemptCount()));

        d->stream.writeStartElement("network:connectionOptions");
        d->stream.writeStartElement("network:connectOnStartup");
        d->stream.writeAttribute("enable", QString::number(s->isConnectOnStartupEnabled()));
        d->stream.writeEndElement(); // End of connectOnStartup element.

        d->stream.writeStartElement("network:ssl");
        d->stream.writeAttribute("enable", QString::number(s->isSslEnabled()));
        d->stream.writeEndElement(); // End of ssl element.
        d->stream.writeEndElement(); // End of connectionOptions element.

        d->stream.writeStartElement("network:servers");
        QStringListIterator serverIter(s->addressList());
        while (serverIter.hasNext()) {
            QStringList split = serverIter.next().split('/');

            d->stream.writeStartElement("network:server");
            d->stream.writeAttribute("address", split[0]);
            d->stream.writeAttribute("port", split[1]);
            d->stream.writeEndElement(); // End of server element.
        }

        d->stream.writeStartElement("network:connectToRandomServer");
        d->stream.writeAttribute("enable", QString::number(s->isConnectToRandomServerEnabled()));
        d->stream.writeEndElement(); // End of connectToRandomServer element.
        d->stream.writeEndElement(); // End of servers element.

        d->stream.writeStartElement("network:channels");
        QStringListIterator channelIter(s->channelList());
        while (channelIter.hasNext()) {
            d->stream.writeTextElement("network:channel", channelIter.next());
        }
        d->stream.writeEndElement(); // End of channels element.

        d->stream.writeStartElement("network:autoJoinChannels");
        d->stream.writeAttribute("enable", QString::number(s->isAutoJoinChannelsEnabled()));
        d->stream.writeEndElement(); // End of autoJoinChannels element.

        d->stream.writeStartElement("network:encoding");
        d->stream.writeAttribute("enable", QString::number(s->isDefaultEncodingEnabled()));
        d->stream.writeEndElement(); // End of encoding element.

        d->stream.writeTextElement("network:defaultEncoding", s->encoding());

        d->stream.writeStartElement("network:identity");
        d->stream.writeAttribute("enable", QString::number(s->isAutoIdentifyEnabled()));
        d->stream.writeTextElement("network:serviceName", s->serviceName());
        d->stream.writeTextElement("network:servicePassword", s->servicePassword());
        d->stream.writeEndElement(); // End of identity element.
        d->stream.writeEndElement(); // End of network element.
    }

    d->stream.writeEndElement(); // End of servers element.
    d->stream.writeEndDocument();
    return true;
}
