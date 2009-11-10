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
#include <KDebug>
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

        QStringList serversSplit = line.split("SERVER:", QString::SkipEmptyParts, Qt::CaseInsensitive);
        QString serverName = serversSplit[0];
        QString addressGroup = serversSplit[1];

        server->setName(serverName);

        QStringList groupSplit = addressGroup.split("GROUP:", QString::SkipEmptyParts, Qt::CaseInsensitive);
        QString addressPort = groupSplit[0];
        QStringList addressPortSplit = addressPort.split(':');

        QString address = addressPortSplit[0];
        QStringList portSplit;

        if (addressPortSplit[1].contains(',')) {
            portSplit = addressPortSplit[1].split(',');
        } else if (addressPortSplit[1].contains('.')) {
            portSplit = addressPortSplit[1].split('.');
        } else {
            return;
        }

        QStringListIterator portIter(portSplit);
        while (portIter.hasNext()) {
            QString ports = portIter.next().trimmed();
            if (ports.contains('-')) {
                QStringList lowHighSplit = ports.split('-');
                int low = lowHighSplit[0].toInt();
                int high = lowHighSplit[1].toInt();

                for (int i = low; i < high + 1; ++i) {
                    addresses << address + '/' + QString::number(i);
                }
            } else {
                addresses << address + '/' + ports;
            }
        }

        if (!addresses.isEmpty()) {
            server->setAddressList(addresses);
            servers << server;
        }
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
