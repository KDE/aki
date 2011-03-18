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

#include "konversationconfiguration.hpp"
#include "konversationconfigurationprivate.h"
using namespace Aki;
using namespace Sql;

KonversationConfiguration::KonversationConfiguration(QObject* parent)
    : Aki::ConfigFile("konversationrc", parent)
{
    _d.reset(new Aki::KonversationConfigurationPrivate(this));
}

KonversationConfiguration::~KonversationConfiguration()
{
}

bool
KonversationConfiguration::read()
{
    for (int i = 0; i < _d->serverGroupCount(); ++i) {
        const QString serverGroupName = QString("ServerGroup %1").arg(QString::number(i));
        setCurrentGroup(serverGroupName);
        QStringList channels = _d->isAutoJoinChannels();
        QStringList servers = _d->serverList();
        qDebug() << "Servers:";
        for (int j = 0, jc = servers.count(); j < jc; ++j) {
            setCurrentGroup(servers.value(j));
            qDebug() << _d->server();
            qDebug() << _d->port();
            qDebug() << _d->isSslEnabled();
            qDebug() << _d->password();
        }
        qDebug() << "AutoJoinChannels: " << !_d->isAutoJoinChannels().isEmpty();
        qDebug() << "Channels: ";
        for (int j = 0, jc = channels.count(); j < jc; ++j) {
            setCurrentGroup(channels.value(j));
            qDebug() << _d->name();
            qDebug() << _d->password();
        }
        setCurrentGroup(serverGroupName);
    }
    return true;
}

void
KonversationConfiguration::setDatabase(Aki::Sql::Database* database)
{
    Q_UNUSED(database)
}

void
KonversationConfiguration::setIdentity(Aki::Sql::Identity* identity)
{
    Q_UNUSED(identity)
}
