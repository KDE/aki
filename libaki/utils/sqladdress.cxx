#include "sqladdress.hpp"
#include "aki.hpp"
#include "private/sqladdress_p.hpp"
#include "utils/sqlnetwork.hpp"
#include <QtCore/QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
using namespace Aki;

SqlAddress::SqlAddress(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::SqlAddressPrivate(this));
}

SqlAddress::~SqlAddress()
{
}

QList<Aki::SqlAddress*>
SqlAddress::addressListForServer(const Aki::SqlNetwork* server)
{
    QList<Aki::SqlAddress*> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Address WHERE addressServer=?");
    query.addBindValue(server->id());

    if (!query.exec()) {
        Aki::SqlAddressPrivate::checkError(query.lastError());
        return list;
    }

    if (query.size() == 0) {
        return list;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf("id");
    const int address = record.indexOf("address");
    const int port = record.indexOf("port");
    const int password = record.indexOf("password");
    const int ssl = record.indexOf("ssl");
    const int addressServer = record.indexOf("addressServer");

    while (query.next()) {
        Aki::SqlAddress* tmp = new Aki::SqlAddress;
        tmp->setAddress(query.value(address).toString());
        tmp->setAddressServerId(query.value(addressServer).toInt());
        tmp->setId(query.value(id).toInt());
        tmp->setPassword(query.value(password).toString());
        tmp->setPort(query.value(port).toInt());
        tmp->setSsl(query.value(ssl).toBool());
        list.append(tmp);
    }

    return list;
}

Aki::SqlAddress*
SqlAddress::findAddress(const QString& address, const Aki::SqlNetwork* server)
{
    QScopedPointer<Aki::SqlAddress> tmp(new Aki::SqlAddress);
    if (address.isEmpty() || address.isNull()) {
        return 0;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM Address WHERE addressServer=? AND address=? LIMIT 1");
    query.addBindValue(server->id());
    query.addBindValue(address);

    if (!query.exec()) {
        Aki::SqlAddressPrivate::checkError(query.lastError());
        return 0;
    }

    if (query.size() == 0) {
        return 0;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf("id");
    const int addressIndex = record.indexOf("address");
    const int port = record.indexOf("port");
    const int password = record.indexOf("password");
    const int ssl = record.indexOf("ssl");
    const int addressServer = record.indexOf("addressServer");

    while (query.next()) {
        tmp->setAddress(query.value(addressIndex).toString());
        tmp->setAddressServerId(query.value(addressServer).toInt());
        tmp->setId(query.value(id).toInt());
        tmp->setPassword(query.value(password).toString());
        tmp->setPort(query.value(port).toInt());
        tmp->setSsl(query.value(ssl).toBool());
        return tmp.take();
    }

    return 0;
}

Aki::SqlAddress*
SqlAddress::newAddress(const QString& address, const Aki::SqlNetwork* server)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Address (address,port,password,ssl,addressServer)"
                  " VALUES(?,?,?,?,?)");
    QScopedPointer<Aki::SqlAddress> tmp(new Aki::SqlAddress);
    tmp->setAddress(address);
    tmp->setAddressServerId(server->id());
    tmp->setPassword(QString());
    tmp->setPort(6667);
    tmp->setSsl(false);

    query.addBindValue(tmp->address());
    query.addBindValue(tmp->port());
    query.addBindValue(tmp->password());
    query.addBindValue(tmp->isSslEnabled());
    query.addBindValue(tmp->addressServerId());

    if (!query.exec()) {
        Aki::SqlAddressPrivate::checkError(query.lastError());
        return 0;
    }

    tmp->setId(query.lastInsertId().toInt());

    return tmp.take();
}

bool
SqlAddress::operator!=(const Aki::SqlAddress& rhs) const
{
    return !(*this == rhs);
}

bool
SqlAddress::operator==(const Aki::SqlAddress& rhs) const
{
    return (address() == rhs.address() &&
            addressServerId() == rhs.addressServerId() &&
            id() == rhs.id() &&
            isSslEnabled() == rhs.isSslEnabled() &&
            password() == rhs.password() &&
            port() == rhs.port());
}

QString
SqlAddress::address() const
{
    return boost::get<0>(_d->address);
}

int
SqlAddress::addressServerId() const
{
    return _d->serverId;
}

int
SqlAddress::id() const
{
    return _d->id;
}

bool
SqlAddress::isSslEnabled() const
{
    return boost::get<3>(_d->address);
}

quint16
SqlAddress::port() const
{
    return boost::get<1>(_d->address);
}

void
SqlAddress::setAddress(const QString& address)
{
    boost::get<0>(_d->address) = address;
}

void
SqlAddress::setAddressServerId(int id)
{
    _d->serverId = id;
}

void
SqlAddress::setId(int id)
{
    _d->id = id;
}

void
SqlAddress::setPort(quint16 port)
{
    boost::get<1>(_d->address) = port;
}

void
SqlAddress::setSsl(bool enabled)
{
    boost::get<3>(_d->address) = enabled;
}

QString
SqlAddress::password() const
{
    return boost::get<2>(_d->address);
}

void
SqlAddress::setPassword(const QString& password)
{
    boost::get<2>(_d->address) = password;
}

bool
SqlAddress::save()
{
    QSqlQuery query;
    query.prepare("UPDATE Address SET address=?, port=?, password=?, ssl=?, addressServer=?"
                  " WHERE id=?");
    query.addBindValue(address());
    query.addBindValue(port());
    query.addBindValue(password());
    query.addBindValue(isSslEnabled());
    query.addBindValue(addressServerId());
    query.addBindValue(id());

    if (!query.exec()) {
        Aki::SqlAddressPrivate::checkError(query.lastError());
        return false;
    }

    return true;
}

bool
SqlAddress::remove()
{
    QSqlQuery query;
    const QString str = "DELETE FROM Address WHERE address=? AND addressServer=?";
    query.prepare(str);
    query.addBindValue(address());
    query.addBindValue(addressServerId());

    if (!query.exec()) {
        Aki::SqlAddressPrivate::checkError(query.lastError());
        return false;
    }

    return true;
}
