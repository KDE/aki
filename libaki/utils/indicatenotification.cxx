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

    const QString appName = KGlobal::mainComponent().componentName();
    DEBUG_TEXT2("Application Name: %1", appName);
    KService::Ptr service = KService::serviceByDesktopName(appName);
    if (service) {
        _d->server->setDesktopFile(service->entryPath());
    } else {
        DEBUG_TEXT("Unable to find desktop file for application")
    }

    connect(_d->server.data(), SIGNAL(serverDisplay()),
            SLOT(showMainWindow()));

    _d->server->show();
}

IndicateNotification::~IndicateNotification()
{
}

void
IndicateNotification::addChannelMessage(const QString& channelName, const QString& message)
{
    DEBUG_FUNC_NAME;
//     _d->indicator = new QIndicate::Indicator(_d->server.data());
//     _d->indicator->setIconProperty(KIcon("aki").pixmap(QSize(16, 16)).toImage());
//     _d->indicator->setNameProperty(QString("%1: \"%2\"").arg(channelName).arg(message));
//     _d->indicator->setTimeProperty(QDateTime::currentDateTimeUtc());
//     _d->indicator->setDrawAttentionProperty(true);
// 
//     connect(_d->indicator, SIGNAL(display(QIndicate::Indicator*)),
//             SLOT(displayIndicator(QIndicate::Indicator*)));
// 
//     _d->indicator->show();
    if (!_d->indicator) {
        _d->indicator = new QIndicate::Indicator(_d->server.data());
        _d->indicateList.append(_d->indicator);
        connect(_d->indicator, SIGNAL(display(QIndicate::Indicator*)),
                SLOT(displayIndicator(QIndicate::Indicator*)));
        _d->indicator->show();
    }

    _d->indicator->setNameProperty(QString("%1: %2").arg(channelName, message));
    _d->indicator->setTimeProperty(QDateTime::currentDateTimeUtc());
    _d->indicator->setDrawAttentionProperty(true);
}

void
IndicateNotification::addCustomMessage(const QString& message, const KIcon& icon, const QString&)
{
    QIndicate::Indicator* ind = new QIndicate::Indicator(this);
    ind->setIconProperty(KIcon("aki").pixmap(QSize(16, 16)).toImage());
    ind->setNameProperty(message);
    ind->setTimeProperty(QDateTime::currentDateTimeUtc());

    connect(ind, SIGNAL(display(QIndicate::Indicator*)),
            SLOT(displayIndicator(QIndicate::Indicator*)));

    ind->show();
}

void
IndicateNotification::addPrivateMessage(const Aki::Irc::NickInfo& from, const QString& message)
{
    DEBUG_FUNC_NAME;
    QIndicate::Indicator* ind = new QIndicate::Indicator(this);
    ind->setIconProperty(KIcon("aki").pixmap(QSize(16, 16)).toImage());
    ind->setNameProperty(QString("%1: \"%2\"").arg(from.nick()).arg(message));
    ind->setTimeProperty(QDateTime::currentDateTimeUtc());

    connect(ind, SIGNAL(display(QIndicate::Indicator*)),
            SLOT(displayIndicator(QIndicate::Indicator*)));

    ind->show();
}

void
IndicateNotification::setMainWindow(Aki::MainWindow* window)
{
    Q_ASSERT(window);
    _d->mainWindow = window;
    _d->server->show();
}

#include "utils/indicatenotification.moc"
