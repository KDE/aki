#include "sqlchannel.hpp"
#include "aki.hpp"
#include "private/sqlchannel_p.hpp"
#include "utils/sqlnetwork.hpp"
#include <QtCore/QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
using namespace Aki;

SqlChannel::SqlChannel(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::SqlChannelPrivate(this));
}

SqlChannel::~SqlChannel()
{
}

bool
SqlChannel::operator!=(const Aki::SqlChannel& rhs) const
{
    return !(*this == rhs);
}

bool
SqlChannel::operator==(const Aki::SqlChannel& rhs) const
{
    return (channel() == rhs.channel() &&
            id() == rhs.id() &&
            password() == rhs.password() &&
            channelServerId() == rhs.channelServerId());
}

Aki::SqlChannel::List
SqlChannel::channelListForServer(const Aki::SqlNetwork* server)
{
    Aki::SqlChannel::List list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Channel WHERE channelServer=?");
    query.addBindValue(server->id());

    if (!query.exec()) {
        Aki::SqlChannelPrivate::checkError(query.lastError());
        return list;
    }

    if (query.size() == 0) {
        return list;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf("id");
    const int channel = record.indexOf("channel");
    const int password = record.indexOf("password");
    const int channelServerId = record.indexOf("channelServer");

    while (query.next()) {
        Aki::SqlChannel* tmp = new Aki::SqlChannel;
        tmp->setId(query.value(id).toInt());
        tmp->setChannel(query.value(channel).toString());
        tmp->setPassword(query.value(password).toString());
        tmp->setChannelServerId(query.value(channelServerId).toInt());
        list.append(tmp);
    }

    return list;
}

Aki::SqlChannel*
SqlChannel::findChannel(const QString& name, const Aki::SqlNetwork* server)
{
    QScopedPointer<Aki::SqlChannel> tmp(new Aki::SqlChannel);
    QSqlQuery query;
    query.prepare("SELECT * FROM Channel WHERE channelServer=? AND channel=? LIMIT 1");
    query.addBindValue(server->id());
    query.addBindValue(name);

    if (!query.exec()) {
        Aki::SqlChannelPrivate::checkError(query.lastError());
        return 0;
    }

    if (query.size() == 0) {
        return 0;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf("id");
    const int channel = record.indexOf("channel");
    const int password = record.indexOf("password");
    const int channelServerId = record.indexOf("channelServer");

    while (query.next()) {
        tmp->setId(query.value(id).toInt());
        tmp->setChannel(query.value(channel).toString());
        tmp->setPassword(query.value(password).toString());
        tmp->setChannelServerId(query.value(channelServerId).toInt());
        return tmp.take();
    }

    return 0;
}

QString
SqlChannel::channel() const
{
    return boost::get<0>(_d->channel);
}

int
SqlChannel::id() const
{
    return _d->id;
}

Aki::SqlChannel*
SqlChannel::newChannel(const QString& name, const Aki::SqlNetwork* server)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Channel (channel, password, channelServer)\n"
                  " VALUES(?,?,?)");
    QScopedPointer<Aki::SqlChannel> channel(new Aki::SqlChannel);
    channel->setChannel(name);
    channel->setPassword(QString());
    channel->setChannelServerId(server->id());

    query.addBindValue(channel->channel());
    query.addBindValue(channel->password());
    query.addBindValue(channel->channelServerId());

    if (!query.exec()) {
        Aki::SqlChannelPrivate::checkError(query.lastError());
        return 0;
    }

    channel->setId(query.lastInsertId().toInt());

    return channel.take();
}

QString
SqlChannel::password() const
{
    return boost::get<1>(_d->channel);
}

int
SqlChannel::channelServerId() const
{
    return _d->serverId;
}

void
SqlChannel::setChannel(const QString& channel)
{
    boost::get<0>(_d->channel) = channel;
}

void
SqlChannel::setId(int id)
{
    _d->id = id;
}

void
SqlChannel::setPassword(const QString& password)
{
    boost::get<1>(_d->channel) = password;
}

void
SqlChannel::setChannelServerId(int id)
{
    _d->serverId = id;
}

bool
SqlChannel::save()
{
    QSqlQuery query;
    const QString str("UPDATE Channel SET channel=?, password=?, channelServer=? WHERE id=?");
    query.prepare(str);
    query.addBindValue(channel());
    query.addBindValue(password());
    query.addBindValue(channelServerId());
    query.addBindValue(id());

    if (!query.exec()) {
        Aki::SqlChannelPrivate::checkError(query.lastError());
        return false;
    }

    return true;
}

bool
SqlChannel::remove()
{
    QSqlQuery query;
    const QString str("DELETE FROM Channel WHERE channel=? AND id=?");
    query.prepare(str);
    query.addBindValue(channel());
    query.addBindValue(id());

    if (!query.exec()) {
        Aki::SqlChannelPrivate::checkError(query.lastError());
        return false;
    }

    return true;
}
