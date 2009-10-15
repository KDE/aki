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

#include "networkimporter.h"
#include "server.h"
#include <KDebug>
#include <KLocale>
using namespace Aki;

namespace Aki
{
class NetworkImporterPrivate
{
public:
    NetworkImporterPrivate(Aki::NetworkImporter *qq)
        : q(qq)
    {
    }

    void readServer()
    {
        Q_ASSERT(q->isStartElement() && q->name() == "servers");

        while (!q->atEnd()) {
            q->readNext();

            if (q->isEndElement()) {
                break;
            }

            if (q->isStartElement()) {
                if (q->name() == "network") {
                    readNetwork();
                }
            }
        }
    }

    void readNetwork()
    {
        Q_ASSERT(q->isStartElement() && q->name() == "network");

        Aki::ServerPointer server(new Aki::Server);

        while (!q->atEnd()) {
            q->readNext();

            if (q->isEndElement()) {
                serverList << server;
                break;
            }

            if (q->isStartElement()) {
                if (q->name() == "name") {
                    readNetworkName(server);
                } else if (q->name() == "reconnection") {
                    readReconnection(server);
                } else if (q->name() == "retryInterval") {
                    readRetryInterval(server);
                } else if (q->name() == "retryAttempts") {
                    readRetryAttempts(server);
                } else if (q->name() == "connectionOptions") {
                    readConnectionOptions(server);
                } else if (q->name() == "servers") {
                    readServers(server);
                } else if (q->name() == "channels") {
                    readChannels(server);
                } else if (q->name() == "autoJoinChannels") {
                    readAutoJoinChannels(server);
                } else if (q->name() == "encoding") {
                    readEncoding(server);
                } else if (q->name() == "defaultEncoding") {
                    readDefaultEncoding(server);
                } else if (q->name() == "identity") {
                    readIdentity(server);
                }
            }
        }
    }

    void readNetworkName(Aki::ServerPointer server)
    {
        Q_ASSERT(q->isStartElement() && q->name() == "name");
        server->setName(q->readElementText());
    }

    void readReconnection(Aki::ServerPointer server)
    {
        Q_ASSERT(q->isStartElement() && q->name() == "reconnection");
        bool enabled = q->attributes().value("enable") == "1";
        server->setAutoReconnection(enabled);
        q->readNext();
    }

    void readRetryInterval(Aki::ServerPointer server)
    {
        Q_ASSERT(q->isStartElement() && q->name() == "retryInterval");
        server->setRetryInterval(q->readElementText().toInt());
    }

    void readRetryAttempts(Aki::ServerPointer server)
    {
        Q_ASSERT(q->isStartElement() && q->name() == "retryAttempts");
        server->setRetryAttempts(q->readElementText().toInt());
    }

    void readConnectionOptions(Aki::ServerPointer server)
    {
        Q_ASSERT(q->isStartElement() && q->name() == "connectionOptions");

        while (!q->atEnd()) {
            q->readNext();

            if (q->isEndElement()) {
                break;
            }

            if (q->isStartElement()) {
                if (q->name() == "connectOnStartup") {
                    bool enabled = q->attributes().value("enable") == "1";
                    server->setConnectOnStartup(enabled);
                    q->readNext();
                } else if (q->name() == "ssl") {
                    bool enabled = q->attributes().value("enable") == "1";
                    server->setSsl(enabled);
                    q->readNext();
                }
            }
        }
    }

    void readChannels(Aki::ServerPointer server)
    {
        Q_ASSERT(q->isStartElement() && q->name() == "channels");
        QStringList channels;

        while (!q->atEnd()) {
            q->readNext();

            if (q->isEndElement()) {
                break;
            }

            if (q->isStartElement()) {
                if (q->name() == "channel") {
                    channels << q->readElementText();
                }
            }
        }

        if (!channels.isEmpty()) {
            server->setChannelList(channels);
        }
    }

    void readServers(Aki::ServerPointer server)
    {
        Q_ASSERT(q->isStartElement() && q->name() == "servers");
        QStringList addresses;

        while (!q->atEnd()) {
            q->readNext();

            if (q->isEndElement()) {
                break;
            }

            if (q->isStartElement()) {
                if (q->name() == "server") {
                    QString address = q->attributes().value("address").toString();
                    QString port = q->attributes().value("port").toString();
                    addresses << (address + "/" + port);
                    q->readNext();
                } else if (q->name() == "connectToRandomServer") {
                    bool enabled = q->attributes().value("enable") == "1";
                    server->setConnectToRandomServer(enabled);
                    q->readNext();
                }
            }
        }

        foreach (QString add, addresses) {
            kDebug() << add;
        }

        if (!addresses.isEmpty()) {
            server->setAddressList(addresses);
        }
    }

    void readAutoJoinChannels(Aki::ServerPointer server)
    {
        bool enabled = q->attributes().value("enable") == "1";
        server->setAutoJoinChannels(enabled);
        q->readNext();
    }

    void readEncoding(Aki::ServerPointer server)
    {
        bool enabled = q->attributes().value("enable") == "1";
        server->setDefaultEncoding(enabled);
        q->readNext();
    }

    void readDefaultEncoding(Aki::ServerPointer server)
    {
        Q_ASSERT(q->isStartElement() && q->name() == "defaultEncoding");
        server->setEncoding(q->readElementText());
    }

    void readIdentity(Aki::ServerPointer server)
    {
        Q_ASSERT(q->isStartElement() && q->name() == "identity");

        bool enabled = q->attributes().value("enable") == "1";
        server->setAutoIdentify(enabled);

        while (!q->atEnd()) {
            q->readNext();

            if (q->isEndElement()) {
                break;
            }

            if (q->isStartElement()) {
                if (q->name() == "serviceName") {
                    server->setServiceName(q->readElementText());
                } else if (q->name() == "servicePassword") {
                    server->setServicePassword(q->readElementText());
                }
            }
        }
    }

    Aki::NetworkImporter *q;
    Aki::ServerList serverList;
}; // End of class NetworkImporterPrivate.
} // End of namespace Aki.

NetworkImporter::NetworkImporter()
    : QXmlStreamReader(),
    d(new NetworkImporterPrivate(this))
{
}

NetworkImporter::~NetworkImporter()
{
    delete d;
}

bool
NetworkImporter::read(QIODevice *device)
{
    if (!device) {
        return false;
    }

    setDevice(device);

    while (!atEnd()) {
        readNext();

        if (isStartElement()) {
            if (name() == "servers") {
                d->readServer();
            }
        }
    }

    if (d->serverList.isEmpty()) {
        return false;
    }

    return true;
}

Aki::ServerList
NetworkImporter::serverList() const
{
    return d->serverList;
}
