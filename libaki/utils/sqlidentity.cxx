/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
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

#include "sqlidentity.hpp"
#include "aki.hpp"
#include "identity.hpp"
#include "private/sqlidentity_p.hpp"
#include <KDE/KUser>
#include <QtCore/QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
using namespace Aki;

SqlIdentity::SqlIdentity(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::SqlIdentityPrivate(this));
    _d->identity = Aki::Identity::Ptr(new Aki::Identity);
}

SqlIdentity::~SqlIdentity()
{
}

bool
SqlIdentity::operator==(const Aki::SqlIdentity& rhs) const
{
    return (awayMessage() == rhs.awayMessage() &&
            awayNickname() == rhs.awayNickname() &&
            id() == rhs.id() &&
            isMarkLastPositionEnabled() == rhs.isMarkLastPositionEnabled() &&
            isMessagesEnabled() == rhs.isMessagesEnabled() &&
            kickMessage() == rhs.kickMessage() &&
            name() == rhs.name() &&
            partMessage() == rhs.partMessage() &&
            quitMessage() == rhs.quitMessage() &&
            realName() == rhs.realName() &&
            returnMessage() == rhs.returnMessage());
}

bool
SqlIdentity::operator!=(const Aki::SqlIdentity& rhs) const
{
    return !(*this == rhs);
}

QString
SqlIdentity::awayMessage() const
{
    return _d->identity->awayMessage();
}

QString
SqlIdentity::awayNickname() const
{
    return _d->identity->awayNickname();
}

Aki::SqlIdentity*
SqlIdentity::findIdentity(const QString& name)
{
    QScopedPointer<Aki::SqlIdentity> identity(new Aki::SqlIdentity);
    if (name.isEmpty() || name.isNull()) {
        return 0;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM Identity WHERE name=? LIMIT 1");
    query.addBindValue(name);

    if (!query.exec()) {
        Aki::SqlIdentityPrivate::checkError(query.lastError());
        return 0;
    }

    if (query.size() == 0) {
        return 0;
    }

    QSqlRecord record = query.record();
    const int id = record.indexOf("id");
    const int awayMessage = record.indexOf("awayMessage");
    const int awayNickname = record.indexOf("awayNickname");
    const int nameIndex = record.indexOf("name");
    const int kickMessage = record.indexOf("kickMessage");
    const int enableMessages = record.indexOf("enableMessages");
    const int markLastPosition = record.indexOf("markLastPosition");
    const int partMessage = record.indexOf("partMessage");
    const int quitMessage = record.indexOf("quitMessage");
    const int realName = record.indexOf("realName");
    const int returnMessage = record.indexOf("returnMessage");

    while (query.next()) {
        identity->setAwayMessage(query.value(awayMessage).toString());
        identity->setAwayNickname(query.value(awayNickname).toString());
        identity->setEnableMessages(query.value(enableMessages).toBool());
        identity->setKickMessage(query.value(kickMessage).toString());
        identity->setMarkLastPosition(query.value(markLastPosition).toBool());
        identity->setName(query.value(nameIndex).toString());
        identity->setPartMessage(query.value(partMessage).toString());
        identity->setQuitMessage(query.value(quitMessage).toString());
        identity->setRealName(query.value(realName).toString());
        identity->setReturnMessage(query.value(returnMessage).toString());
        identity->setId(query.value(id).toInt());
        return identity.take();
    }

    return 0;
}

int
SqlIdentity::id() const
{
    return _d->id;
}

QStringList
SqlIdentity::identityNames()
{
    QStringList names;
    QSqlQuery query;
    query.prepare("SELECT name FROM Identity");

    if (!query.exec()) {
        Aki::SqlIdentityPrivate::checkError(query.lastError());
        return names;
    }

    QSqlRecord record = query.record();
    const int index = record.indexOf("name");

    while (query.next()) {
        names.append(query.value(index).toString());
    }

    return names;
}

bool
SqlIdentity::isMarkLastPositionEnabled() const
{
    return _d->identity->isMarkLastPositionEnabled();
}

bool
SqlIdentity::isMessagesEnabled() const
{
    return _d->identity->isAwayMessagesEnabled();
}

QString
SqlIdentity::kickMessage() const
{
    return _d->identity->kickMessage();
}

QString
SqlIdentity::name() const
{
    return _d->identity->name();
}

Aki::SqlIdentity*
SqlIdentity::newIdentity(const QString& name)
{
    QString tmp;
    KUser user(KUser::UseRealUserID);

    if (user.property(KUser::FullName).isNull()) {
        tmp = "Aki";
    } else {
        tmp = user.property(KUser::FullName).toString();
    }

    QScopedPointer<Aki::SqlIdentity> identity(new Aki::SqlIdentity);
    identity->setAwayMessage("I'm now away");
    identity->setAwayNickname(user.loginName() + "|away");
    identity->setEnableMessages(false);
    identity->setKickMessage(QString());
    identity->setMarkLastPosition(false);
    identity->setName(name);
    identity->setPartMessage("Aki IRC Client %v");
    identity->setQuitMessage("Aki IRC Client %v");
    identity->setRealName(tmp);
    identity->setReturnMessage("I'm now back");

    QSqlQuery query;
    query.prepare("INSERT INTO Identity (awayMessage,awayNickname,name,kickMessage,enableMessages,\n"
                  "markLastPosition,partMessage,quitMessage,realName,returnMessage)\n"
                  "VALUES(?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(identity->awayMessage()));
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(identity->awayNickname()));
    query.addBindValue(identity->name());
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(identity->kickMessage()));
    query.addBindValue(identity->isMessagesEnabled());
    query.addBindValue(identity->isMarkLastPositionEnabled());
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(identity->partMessage()));
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(identity->quitMessage()));
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(identity->realName()));
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(identity->returnMessage()));

    if (!query.exec()) {
        Aki::SqlIdentityPrivate::checkError(query.lastError());
        return 0;
    }

    identity->setId(query.lastInsertId().toInt());

    return identity.take();
}

QString
SqlIdentity::partMessage() const
{
    return _d->identity->partMessage();
}

QString
SqlIdentity::quitMessage() const
{
    return _d->identity->quitMessage();
}

QString
SqlIdentity::realName() const
{
    return _d->identity->realName();
}

QString
SqlIdentity::returnMessage() const
{
    return _d->identity->returnMessage();
}

void
SqlIdentity::setAwayMessage(const QString& message)
{
    _d->identity->setAwayMessage(message);
}

void
SqlIdentity::setAwayNickname(const QString& nickname)
{
    _d->identity->setAwayNickname(nickname);
}

void
SqlIdentity::setEnableMessages(bool enabled)
{
    _d->identity->setUseAwayMessages(enabled);
}

void
SqlIdentity::setKickMessage(const QString& message)
{
    _d->identity->setKickMessage(message);
}

void
SqlIdentity::setMarkLastPosition(bool enabled)
{
    _d->identity->setMarkLastPosition(enabled);
}

void
SqlIdentity::setName(const QString& name)
{
    _d->identity->setName(name);
}

void
SqlIdentity::setPartMessage(const QString& message)
{
    _d->identity->setPartMessage(message);
}

void
SqlIdentity::setQuitMessage(const QString& message)
{
    _d->identity->setQuitMessage(message);
}

void
SqlIdentity::setRealName(const QString& name)
{
    _d->identity->setRealName(name);
}

void
SqlIdentity::setReturnMessage(const QString& message)
{
    _d->identity->setReturnMessage(message);
}

bool
SqlIdentity::save()
{
    QSqlQuery query;
    const QString str("UPDATE Identity SET awayMessage=?, awayNickname=?, name=?, kickMessage=?, enableMessages=?"
                      ", markLastPosition=?, partMessage=?, quitMessage=?, realName=?, returnMessage=? WHERE id=?");

    query.prepare(str);
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(awayMessage()));
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(awayNickname()));
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(name()));
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(kickMessage()));
    query.addBindValue(isMessagesEnabled());
    query.addBindValue(isMarkLastPositionEnabled());
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(partMessage()));
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(quitMessage()));
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(realName()));
    query.addBindValue(Aki::SqlIdentityPrivate::checkString(returnMessage()));
    query.addBindValue(id());

    if (!query.exec()) {
        Aki::SqlIdentityPrivate::checkError(query.lastError());
        return false;
    }

    return true;
}

void
SqlIdentity::setId(int id)
{
    _d->id = id;
}

bool
SqlIdentity::remove()
{
    QSqlQuery query;
    const QString str("DELETE FROM Identity WHERE name=? AND id=?");
    query.prepare(str);
    query.addBindValue(name());
    query.addBindValue(id());

    if (!query.exec()) {
        Aki::SqlIdentityPrivate::checkError(query.lastError());
        return false;
    }

    return true;
}
