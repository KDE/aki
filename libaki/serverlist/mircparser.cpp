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

#include "mircparser.h"
#include <QTextStream>
using namespace Aki;

namespace Aki
{
class MIrcParserPrivate
{
public:
    MIrcParserPrivate(Aki::MIrcParser *qq)
        : q(qq)
    {
    }

    void readServerLine(const QString &str)
    {
        QString line = str;
        Aki::ServerPointer server(new Aki::Server);
        QStringList addresses;

        // Remove the nXX= part from the beginning of the string since
        // it is useless.
        QString tmp = line.left(line.indexOf('='));
        line.remove(0, tmp.length() + 1);

        // Split the string up
        // serverSplit[0] = "A5KNet: Random server"
        // serverSplit[1] = "irc.a5knet.com:6660-6669GROUP:1ran"
        QStringList serverSplit = line.split("SERVER:");

        // Now we split the serverSplit[1] to get the group and
        // the address and port(s).
        // groupSplit[0] = "irc.a5knet.com:6660-6669"
        // groupSplit[1] = "1ran"
        QStringList groupSplit = serverSplit[1].split("GROUP:");

        server->setName(serverSplit[0]);

        // Now we split it so we can get the address and port(s)
        QStringList addressPortSplit = groupSplit[0].split(':');

        // We need to check if there is a range of ports. Since
        // this is a limitation in Aki. As we don't support port
        // ranges we will have to add each address for each port.
        if (addressPortSplit[1].contains('-')) {
            // We get the port and split it in to a low and high port
            // range.
            QStringList highLowPort = addressPortSplit[1].split('-');
            // Get the low.
            int low = highLowPort[0].toInt();
            // Get the high and add 1 so we can get the last port.
            int high = highLowPort[1].toInt() + 1;

            for (int i = low; i < high; ++i) {
                // Append the new address to the list.
                // In Aki it is stores add <address>/<port>
                addresses << addressPortSplit[0] + '/' + QString::number(i);
            }

        } else if (addressPortSplit[1].contains(',')) {
            // Now we check if there is multiple ports and it's not a range.
            QStringList portSplit = addressPortSplit[1].split(',');

            // Iterate over the string list appending the address and port.
            QStringListIterator portIter(portSplit);
            while (portIter.hasNext()) {
                // Addresses are in <address>/<port> format.
                addresses << addressPortSplit[0] + '/' + portIter.next();
            }
        } else {
            // Apparently we just have 1 port.
            // Addresses are in <address>/<port> format.
            addresses << addressPortSplit[0] + '/' + addressPortSplit[1];
        }

        server->setAddressList(addresses);
        servers << server;
    }

    Aki::MIrcParser *q;
    Aki::ServerList servers;
}; // End of class MIrcParserPrivate.
} // End of namespace Aki.

MIrcParser::MIrcParser(QObject *parent)
    : QObject(parent)
{
    d.reset(new Aki::MIrcParserPrivate(this));
}

MIrcParser::~MIrcParser()
{
}

bool
MIrcParser::read(QIODevice *device)
{
    if (!device) {
        return false;
    }

    QString line;

    QTextStream in(device);
    while (!in.atEnd()) {
        line = in.readLine().trimmed();

        if (line[0] == 'n') {
            d->readServerLine(line);
        }
    }

    device->close();

    return true;
}

Aki::ServerList
MIrcParser::servers() const
{
    return d->servers;
}
