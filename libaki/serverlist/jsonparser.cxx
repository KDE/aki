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

#include "jsonparser.hpp"
#include "debughelper.hpp"
#include "private/jsonparser_p.hpp"
using namespace Aki;
using namespace Sql;

JsonParser::JsonParser(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::JsonParserPrivate(this));
}

JsonParser::~JsonParser()
{
}

bool
JsonParser::read(const QString& file)
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

    const QByteArray jsonData = _d->file.readAll();
    if (jsonData.isEmpty()) {
        DEBUG_TEXT("Unable to parse empty file")
        _d->file.close();
        return false;
    }

    bool ok = false;
    const QVariantMap serverMap = _d->parser.parse(jsonData, &ok).toMap();

    if (!ok) {
        DEBUG_TEXT("Unable to parse json file")
        DEBUG_TEXT2("Error: %1", _d->parser.errorString());
        _d->file.close();
        return false;
    }

    DEBUG_TEXT2("Version: %1", serverMap["version"].toString());

    QList<Aki::Sql::Server*> serverList;
    QList<Aki::Sql::Channel*> channelList;
    QList<Aki::Sql::Address*> addressList;

    foreach (const QVariant& var, serverMap) {
        const QVariantMap map = var.toMap();

        if (!map["name"].toString().isEmpty()) {
            serverList.append(_d->toServer(map));
        }
    }

    if (_d->database->transaction()) {
        foreach (Aki::Sql::Server* server, serverList) {
            _d->database->add(server);
        }
        if (!_d->database->commit()) {
            DEBUG_TEXT("Unable to commit servers to database")
            qDeleteAll(serverList);
            _d->file.close();
            return false;
        } else {
            DEBUG_TEXT("Successfully committed servers to database")
        }
    } else {
        DEBUG_TEXT("Unable to start transaction for servers")
        qDeleteAll(serverList);
        _d->file.close();
        return false;
    }

    foreach (const QVariant& var, serverMap) {
        const QVariantMap map = var.toMap();
        const QString name = map["name"].toString();

        if (!name.isEmpty()) {
            foreach (const Aki::Sql::Server* server, serverList) {
                if (name == server->name()) {
                    const QVariantMap channelMap = map["channels"].toMap();
                    foreach (const QVariant& var, channelMap) {
                        channelList.append(_d->toChannel(var.toMap(), server));
                    }

                    const QVariantMap addressMap = map["addresses"].toMap();
                    foreach (const QVariant& var, addressMap) {
                        addressList.append(_d->toAddress(var.toMap(), server));
                    }
                }
            }
        }
    }
    _d->file.close();

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
JsonParser::setDatabase(Aki::Sql::Database* database)
{
    Q_ASSERT(database);
    Q_ASSERT(database->isOpen());
    _d->database = database;
}

void
JsonParser::setIdentity(Aki::Sql::Identity* identity)
{
    Q_ASSERT(identity);
    Q_ASSERT(identity->id() != -1);
    _d->identity = identity;
}

bool
JsonParser::write(const QString& file)
{
    DEBUG_FUNC_NAME;

    if (file.isEmpty()) {
        DEBUG_TEXT("File name is empty")
        return false;
    }

    _d->file.setFileName(file);
    if (!_d->file.open(QIODevice::WriteOnly)) {
        DEBUG_TEXT2("Unable to open file: %1", file)
        DEBUG_TEXT2("Error: %1", _d->file.errorString());
        return false;
    }

    const QList<Aki::Sql::Server*> serverList = _d->database->find<Aki::Sql::Server>()
                                                    .where("serverIdentity = :serverIdentity").bind(":serverIdentity", _d->identity->id())
                                                    .result();
    if (serverList.isEmpty()) {
        DEBUG_TEXT("Unable to fetch servers. No servers found or an error has occurred")
        return false;
    }

    QVariantMap jsonData;
    jsonData["version"] = AKI_VERSION_STR;

    for (int i = serverList.count(); i > 0; --i) {
        const Aki::Sql::Server* server = serverList.at(i - 1);
        QVariantMap serverMap = _d->toMap(server);

        const QList<Aki::Sql::Address*> addressList = _d->database->find<Aki::Sql::Address>()
                                                        .where("addressServer = :addressServer").bind(":addressServer", server->id())
                                                        .result();
        QVariantMap addressMap;
        for (int i = addressList.count(); i > 0; --i) {
            const Aki::Sql::Address* address = addressList.at(i - 1);
            addressMap.insertMulti("address", _d->toMap(address));
        }
        qDeleteAll(addressList);

        if (!addressMap.isEmpty()) {
            serverMap.insert("addresses", addressMap);
        }

        const QList<Aki::Sql::Channel*> channelList = _d->database->find<Aki::Sql::Channel>()
                                                        .where("channelServer = :channelServer").bind(":channelServer", server->id())
                                                        .result();
        QVariantMap channelMap;
        for (int i = channelList.count(); i > 0; --i) {
            const Aki::Sql::Channel* channel = channelList.at(i - 1);
            channelMap.insertMulti("channels", _d->toMap(channel));
        }
        qDeleteAll(channelList);

        if (!channelMap.isEmpty()) {
            serverMap.insert("channels", channelMap);
        }
        jsonData.insertMulti("server", serverMap);
    }

    qDeleteAll(serverList);

    _d->serializer.setIndentMode(QJson::IndentFull);
    _d->serializer.allowSpecialNumbers(true);

    const QByteArray data = _d->serializer.serialize(jsonData);
    if (data.isEmpty()) {
        DEBUG_TEXT("Unable to serialise objects")
        return false;
    }

    _d->file.write(data);
    _d->file.close();
    return true;
}

#include "serverlist/jsonparser.moc"
