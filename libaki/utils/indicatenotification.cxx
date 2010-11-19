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

#include "indicatenotification.hpp"
#include "debughelper.hpp"
#include "private/indicatenotification_p.hpp"
#include "ui/mainwindow.hpp"
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <KDE/KComponentData>
#include <KDE/KDateTime>
#include <KDE/KGlobal>
#include <KDE/KService>
#include <qindicateindicator.h>
#include <qindicateserver.h>
using namespace Aki;

IndicateNotification::IndicateNotification(QObject* parent)
    : QObject(parent)
{
    DEBUG_FUNC_NAME;
    _d.reset(new Aki::IndicateNotificationPrivate(this));
    _d->server.reset(QIndicate::Server::defaultInstance());
    _d->server->setType("message.irc");

    QString appName = KGlobal::mainComponent().componentName();
    KService::Ptr service = KService::serviceByDesktopName(appName);
    _d->server->setDesktopFile(service->entryPath());
    _d->server->show();

    connect(_d->server.data(), SIGNAL(serverDisplay()),
            SLOT(showMainWindow()));
}

IndicateNotification::~IndicateNotification()
{
}

void
IndicateNotification::addChannelMessage(const QString& channelName, const QString& message)
{
    DEBUG_FUNC_NAME;
    _d->indicator = new QIndicate::Indicator(_d->server.data());
    _d->indicator->setIconProperty(KIcon("aki").pixmap(QSize(16, 16)).toImage());
    _d->indicator->setNameProperty(QString("%1: \"%2\"").arg(channelName).arg(message));
    _d->indicator->setTimeProperty(QDateTime::currentDateTimeUtc());
    _d->indicator->setDrawAttentionProperty(true);

    connect(_d->indicator, SIGNAL(display(QIndicate::Indicator*)),
            SLOT(displayIndicator(QIndicate::Indicator*)));

    _d->indicator->show();
}

void
IndicateNotification::addCustomMessage(const QString& message, const KIcon& icon, const QString&)
{
    DEBUG_FUNC_NAME;
    _d->indicator = new QIndicate::Indicator(_d->server.data());
    _d->indicator->setIconProperty(icon.pixmap(QSize(16, 16)).toImage());
    _d->indicator->setNameProperty(message);
    _d->indicator->setTimeProperty(QDateTime::currentDateTimeUtc());
    _d->indicator->setDrawAttentionProperty(true);

    connect(_d->indicator, SIGNAL(display(QIndicate::Indicator*)),
            SLOT(displayIndicator(QIndicate::Indicator*)));

    _d->indicator->show();
}

void
IndicateNotification::addPrivateMessage(const Aki::Irc::NickInfo& from, const QString& message)
{
    DEBUG_FUNC_NAME;
    _d->indicator = new QIndicate::Indicator(_d->server.data());
    _d->indicator->setIconProperty(KIcon("aki").pixmap(QSize(16, 16)).toImage());
    _d->indicator->setNameProperty(QString("%1: \"%2\"").arg(from.nick()).arg(message));
    _d->indicator->setTimeProperty(QDateTime::currentDateTimeUtc());

    connect(_d->indicator, SIGNAL(display(QIndicate::Indicator*)),
            SLOT(displayIndicator(QIndicate::Indicator*)));

    _d->indicator->show();
}

void
IndicateNotification::setMainWindow(Aki::MainWindow* window)
{
    Q_ASSERT(window);
    _d->mainWindow = window;
}

#include "utils/indicatenotification.moc"
