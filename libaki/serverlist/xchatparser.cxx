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

#include "xchatparser.hpp"
#include "debughelper.hpp"
#include "private/xchatparser_p.hpp"
using namespace Aki;

XChatParser::XChatParser(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::XChatParserPrivate(this));
}

XChatParser::~XChatParser()
{
}

bool
XChatParser::read(const QString& file)
{
    DEBUG_FUNC_NAME;

    if (file.isEmpty()) {
        DEBUG_TEXT("File name is empty")
        return false;
    }

    _d->file.setFileName(file);
    if (!_d->file.open(QIODevice::ReadOnly)) {
        DEBUG_TEXT2("Unable to open file: %1", file)
        DEBUG_TEXT2("Error: %1", _d->file.errorString());
        return false;
    }

    Aki::XChatServer* server = 0;

    QTextStream stream(&_d->file);
    while (!stream.atEnd()) {
        QString temp = stream.readLine().trimmed();

        if (temp.isEmpty()) {
            continue;
        }

        const QChar c = temp.at(0);
        temp.remove(0, 2);

        if (c == 'N') {
            server = new Aki::XChatServer;
            _d->serverList.append(server);

            server->name = temp;
        } else if (c == 'P') {
            server->serverPassword = temp;
        } else if (c == 'J') {
            QStringList channels;
            QStringList keys;

            if (temp.contains(' ')) {
                const QStringList split = temp.split(' ', QString::SkipEmptyParts);
                channels = split.at(0).split(',', QString::SkipEmptyParts);
                keys = split.at(1).split(',');

                for (int i = 0, c = channels.count(); i < c; ++i) {
                    if (!channels.at(i).isEmpty()) {
                        server->channels.insert(channels.value(i), keys.value(i));
                    }
                }
            } else {
                channels = temp.split(',', QString::SkipEmptyParts);

                foreach (const QString& channel, channels) {
                    server->channels.insert(channel, QString());
                }
            }
        } else if (c == 'B') {
            server->nickServPassword = temp;
        } else if (c == 'E') {
            server->encoding = temp;
        } else if (c == 'C') {
            
        } else if (c == 'F') {
            server->flags = temp.toInt();
        } else if (c == 'D') {
        } else if (c == 'S') {
            server->addresses.append(temp);
        }
    }
    _d->file.close();

    QList<Aki::Sql::Server*> serverList;
    QList<Aki::Sql::Channel*> channelList;
    QList<Aki::Sql::Address*> addressList;
    foreach (const Aki::XChatServer* server, _d->serverList) {
        if (!server) {
            continue;
        }

        Aki::Sql::Server* sqlServer = new Aki::Sql::Server;
        sqlServer->setName(server->name);
        sqlServer->setServicePassword(server->nickServPassword);

        if (server->flags & Aki::XChatParser::AllowInvalidSsl) {
            //sqlServer->setAllowInvalidSsl(true);
        } else if (server->flags & Aki::XChatParser::AutoConnectOnStartup) {
            sqlServer->setConnectOnStartup(true);
        }

        sqlServer->setServerIdentity(_d->identity->id());
        serverList.append(sqlServer);
    }

    if (_d->database->transaction()) {
        foreach (Aki::Sql::Server* server, serverList) {
            _d->database->add(server);
        }
        if (!_d->database->commit()) {
            DEBUG_TEXT("Unable to commit servers to database")
            qDeleteAll(serverList);
            return false;
        } else {
            DEBUG_TEXT("Successfully committed servers to database")
        }
    } else {
        DEBUG_TEXT("Unable to start transaction for servers")
        qDeleteAll(serverList);
        return false;
    }

    foreach (Aki::XChatServer* server, _d->serverList) {
        foreach (const Aki::Sql::Server* sqlServer, serverList) {
            if (server->name == sqlServer->name()) {
                QHashIterator<QString, QString> channelsIter(server->channels);
                while (channelsIter.hasNext()) {
                    channelsIter.next();

                    Aki::Sql::Channel* sqlChannel = new Aki::Sql::Channel;
                    sqlChannel->setName(channelsIter.key());
                    sqlChannel->setPassword(channelsIter.value());
                    sqlChannel->setChannelServer(sqlServer->id());
                    channelList.append(sqlChannel);
                }
            }
        }
    }

    foreach (Aki::XChatServer* server, _d->serverList) {
        foreach (Aki::Sql::Server* sqlServer, serverList) {
            if (server->name == sqlServer->name()) {
                foreach (const QString& address, server->addresses) {
                    Aki::Sql::Address* sqlAddress = new Aki::Sql::Address;
                    if (address.contains('/')) {
                        const QStringList split = address.split('/', QString::SkipEmptyParts);
                        sqlAddress->setAddress(split.at(0));
                        sqlAddress->setAddressServer(sqlServer->id());

                        QString port = split.at(1);

                        if (port.contains('+')) {
                            sqlAddress->setPort(port.remove('+').toInt());
                            sqlAddress->setSsl(true);
                        } else {
                            sqlAddress->setPort(6667);
                        }
                    } else {
                        sqlAddress->setAddress(address);
                        sqlAddress->setAddressServer(sqlServer->id());
                        sqlAddress->setPort(6667);
                    }

                    if (server->flags & Aki::XChatParser::UseSsl) {
                        sqlAddress->setSsl(true);
                    }

                    addressList.append(sqlAddress);
                }
            }
        }
    }

    if (_d->database->transaction()) {
        foreach (Aki::Sql::Channel* channel, channelList) {
            _d->database->add(channel);
        }
        foreach (Aki::Sql::Address* address, addressList) {
            _d->database->add(address);
        }

        if (!_d->database->commit()) {
            DEBUG_TEXT("Unable to commit channels to database")
            DEBUG_TEXT("Unable to commit addresses to database")
            qDeleteAll(serverList);
            qDeleteAll(channelList);
            qDeleteAll(addressList);
            return false;
        } else {
            DEBUG_TEXT("Successfully committed channels to database")
            DEBUG_TEXT("Successfully committed addresses to database")
            qDeleteAll(serverList);
            qDeleteAll(channelList);
            qDeleteAll(addressList);
            return true;
        }
    } else {
        DEBUG_TEXT("Unable to start transaction for channels")
        DEBUG_TEXT("Unable to start transaction for addresses")
        qDeleteAll(serverList);
        qDeleteAll(channelList);
        qDeleteAll(addressList);
        return false;
    }
    return true;
}

void
XChatParser::setDatabase(Aki::Sql::Database* database)
{
    Q_ASSERT(database);
    Q_ASSERT(database->isOpen());
    _d->database = database;
}

void
XChatParser::setIdentity(Aki::Sql::Identity* identity)
{
    Q_ASSERT(identity);
    Q_ASSERT(identity->id() != -1);
    _d->identity = identity;
}
