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

#include "xmlparser.h"
#include <KDebug>
#include <QTextCodec>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
using namespace Aki;

namespace Aki
{
class XmlParserPrivate
{
public:
    XmlParserPrivate(Aki::XmlParser *qq)
        : q(qq)
    {
        servers.clear();
    }

    void readServer()
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "servers");

        while (reader.readNextStartElement()) {
            if (reader.name() == "network") {
                readNetwork();
            }
        }
    }

    void readNetwork()
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "network");

        Aki::ServerPointer server;

        while (reader.readNextStartElement()) {
            if (reader.name() == "name") {
                readNetworkName(server);
            } else if (reader.name() == "reconnection") {
                readReconnection(server);
            } else if (reader.name() == "retryInterval") {
                readRetryInterval(server);
            } else if (reader.name() == "retryAttempts") {
                readRetryAttempts(server);
            } else if (reader.name() == "connectionOptions") {
                readConnectionOptions(server);
            } else if (reader.name() == "servers") {
                readServers(server);
            } else if (reader.name() == "channels") {
                readChannels(server);
            } else if (reader.name() == "autoJoinChannels") {
                readAutoJoinChannels(server);
            } else if (reader.name() == "encoding") {
                readEncoding(server);
            } else if (reader.name() == "defaultEncoding") {
                readDefaultEncoding(server);
            } else if (reader.name() == "identity") {
                readIdentity(server);
            }

            servers << server;
        }
    }

    void readNetworkName(Aki::ServerPointer server)
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "name");
        server->setName(reader.readElementText());
    }

    void readReconnection(Aki::ServerPointer server)
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "reconnection");
        bool enabled = reader.attributes().value("enabled") == "1";
        server->setAutoReconnection(enabled);
    }

    void readRetryInterval(Aki::ServerPointer server)
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "retryInterval");
        server->setRetryInterval(reader.readElementText().toInt());
    }

    void readRetryAttempts(Aki::ServerPointer server)
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "retryAttempts");
        server->setRetryAttempts(reader.readElementText().toInt());
    }

    void readConnectionOptions(Aki::ServerPointer server)
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "connectionOptions");

        while (reader.readNextStartElement()) {
            if (reader.name() == "connectOnStartup") {
                bool enabled = reader.attributes().value("enable") == "1";
                server->setConnectOnStartup(enabled);
            } else if (reader.name() == "ssl") {
                bool enabled = reader.attributes().value("enable") == "1";
                server->setSsl(enabled);
            }
        }
    }

    void readServers(Aki::ServerPointer server)
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "servers");
        QStringList addresses;

        while (reader.readNextStartElement()) {
            if (reader.name() == "server") {
                QString address = reader.attributes().value("address").toString();
                QString port = reader.attributes().value("port").toString();
                addresses << (address + '/' + port);
            } else if (reader.name() == "connectToRandomServer") {
                bool enabled = reader.attributes().value("enable") == "1";
                server->setConnectToRandomServer(enabled);
            }
        }

        if (!addresses.isEmpty()) {
            server->setAddressList(addresses);
        }
    }

    void readChannels(Aki::ServerPointer server)
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "channels");
        QStringList channels;

        while (reader.readNextStartElement()) {
            if (reader.name() == "channel") {
                channels << reader.readElementText();
            }
        }

        if (!channels.isEmpty()) {
            server->setChannelList(channels);
        }
    }

    void readAutoJoinChannels(Aki::ServerPointer server)
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "autoJoinChannels");
        bool enabled = reader.attributes().value("enable") == "1";
        server->setAutoJoinChannels(enabled);
    }

    void readEncoding(Aki::ServerPointer server)
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "encoding");
        bool enabled = reader.attributes().value("enable") == "1";
        server->setDefaultEncoding(enabled);
    }

    void readDefaultEncoding(Aki::ServerPointer server)
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "defaultEncoding");
        server->setEncoding(reader.readElementText());
    }

    void readIdentity(Aki::ServerPointer server)
    {
        Q_ASSERT(reader.isStartElement() && reader.name() == "identity");

        bool enabled = reader.attributes().value("enable") == "1";
        server->setAutoIdentify(enabled);

        while (reader.readNextStartElement()) {
            if (reader.name() == "serviceName") {
                server->setServiceName(reader.readElementText());
            } else if (reader.name() == "servicePassword") {
                server->setServicePassword(reader.readElementText());
            }
        }
    }

    Aki::XmlParser *q;
    QXmlStreamReader reader;
    QXmlStreamWriter writer;
    Aki::ServerList servers;
    QByteArray data;
}; // End of class XmlParserPrivate.
} // End of namespace Aki.

XmlParser::XmlParser(QObject *parent)
    : QObject(parent)
{
    d.reset(new XmlParserPrivate(this));
}

XmlParser::~XmlParser()
{
}

bool
XmlParser::read(QIODevice *device)
{
    if (!device) {
        kDebug() << "Device fulled";
        device->close();
        return false;
    }

    d->reader.setDevice(device);

    if (d->reader.readNextStartElement()) {
        if (d->reader.name() == "servers") {
            d->readServer();
        } else {
            d->reader.raiseError("Not a valid Aki server list file");
            device->close();
            return false;
        }
    }

    if (d->servers.isEmpty()) {
        kDebug() << "Servers is empty";
        device->close();
        return false;
    }

    kDebug() << "Returned Normal";
    device->close();
    return true;
}

bool
XmlParser::write(QIODevice *device)
{
    if (!device) {
        device->close();
        return false;
    }

    if (!device->isWritable()) {
        kWarning("Device is in reading mode. Make sure you didn't call read already.");
        return false;
    }

    if (d->servers.isEmpty()) {
        device->close();
        return false;
    }

    d->writer.setDevice(device);
    d->writer.setAutoFormattingIndent(4);
    d->writer.setCodec(QTextCodec::codecForName("UTF-8"));

    d->writer.writeStartDocument();
    d->writer.writeDTD("<!DOCTYPE network>");
    d->writer.writeStartElement("http://aki.kde.org/aki", "servers");
    d->writer.writeAttribute("xmlns:network", "http://aki.kde.org/aki/network");

    foreach (Aki::ServerPointer s, d->servers) {
        d->writer.writeStartElement("network");

        d->writer.writeTextElement("network:name", s->name());

        d->writer.writeStartElement("network:reconnection");
        d->writer.writeAttribute("enable", QString::number(s->isAutoReconnectionEnabled()));
        d->writer.writeEndElement(); // End of reconnection element.

        d->writer.writeTextElement("network:retryInterval", QString::number(s->retryInterval()));
        d->writer.writeTextElement("network:retryAttempts", QString::number(s->retryAttemptCount()));

        d->writer.writeStartElement("network:connectionOptions");
        d->writer.writeStartElement("network:connectOnStartup");
        d->writer.writeAttribute("enable", QString::number(s->isConnectOnStartupEnabled()));
        d->writer.writeEndElement(); // End of connectOnStartup element.

        d->writer.writeStartElement("network:ssl");
        d->writer.writeAttribute("enable", QString::number(s->isSslEnabled()));
        d->writer.writeEndElement(); // End of ssl element.
        d->writer.writeEndElement(); // End of connectionOptions element.

        d->writer.writeStartElement("network:servers");
        QStringListIterator serverIter(s->addressList());
        while (serverIter.hasNext()) {
            QStringList split = serverIter.next().split('/');

            d->writer.writeStartElement("network:server");
            d->writer.writeAttribute("address", split[0]);
            d->writer.writeAttribute("port", split[1]);
            d->writer.writeEndElement(); // End of server element.
        }

        d->writer.writeStartElement("network:connectToRandomServer");
        d->writer.writeAttribute("enable", QString::number(s->isConnectToRandomServerEnabled()));
        d->writer.writeEndElement(); // End of connectToRandomServer element.
        d->writer.writeEndElement(); // End of servers element.

        d->writer.writeStartElement("network:channels");
        QStringListIterator channelIter(s->channelList());
        while (channelIter.hasNext()) {
            d->writer.writeTextElement("network:channel", channelIter.next());
        }
        d->writer.writeEndElement(); // End of channels element.

        d->writer.writeStartElement("network:autoJoinChannels");
        d->writer.writeAttribute("enable", QString::number(s->isAutoJoinChannelsEnabled()));
        d->writer.writeEndElement(); // End of autoJoinChannels element.

        d->writer.writeStartElement("network:encoding");
        d->writer.writeAttribute("enable", QString::number(s->isDefaultEncodingEnabled()));
        d->writer.writeEndElement(); // End of encoding element.

        d->writer.writeTextElement("network:defaultEncoding", s->encoding());

        d->writer.writeStartElement("network:identity");
        d->writer.writeAttribute("enable", QString::number(s->isAutoIdentifyEnabled()));
        d->writer.writeTextElement("network:serviceName", s->serviceName());
        d->writer.writeTextElement("network:servicePassword", s->servicePassword());
        d->writer.writeEndElement(); // End of identity element.
        d->writer.writeEndElement(); // End of network element.
    }

    d->writer.writeEndElement(); // End of servers element.
    d->writer.writeEndDocument();

    device->close();
    return true;
}

Aki::ServerList
XmlParser::servers() const
{
    return d->servers;
}

void
XmlParser::setServerList(const Aki::ServerList &servers)
{
    d->servers = servers;
}
