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

#include "amarokremote.h"
#include "amarok.h"
#include <KDebug>
#include <KProcess>
#include <QDBusInterface>
#include <QDBusReply>
#include <QTextCodec>

AmarokRemote::AmarokRemote(QObject *parent)
    : QObject(parent),
    m_interface("org.freedesktop.MediaPlayer"),
    m_service("org.mpris.amarok")
{
    m_iface = new QDBusInterface(m_service, "/Player", m_interface, QDBusConnection::sessionBus(), this);
}

void
AmarokRemote::exit()
{
    QDBusMessage message = QDBusMessage::createMethodCall(m_service, "/", m_interface, "Quit");
    QDBusConnection::systemBus().call(message, QDBus::NoBlock);
}

void
AmarokRemote::nextTrack()
{
    m_iface->call("Next");
}

void
AmarokRemote::pause()
{
    m_iface->call("Pause");
}

void
AmarokRemote::play()
{
    m_iface->call("Play");
}

void
AmarokRemote::previousTrack()
{
    m_iface->call("Prev");
}

void
AmarokRemote::setRepeat(bool enable)
{
    m_iface->call("Repeat", enable);
}

void
AmarokRemote::setVolume(int volume)
{
    m_iface->call("VolumeSet", qBound(0, volume, 100));
}

void
AmarokRemote::stop()
{
    m_iface->call("Stop");
}

QString
AmarokRemote::metaData() const
{
    QDBusReply<QVariantMap> msg = m_iface->call("GetMetadata");
    QVariantMap map = msg.value();
    if (map.isEmpty()) {
        return QString();
    }

    QTextCodec::codecForName("UTF-8")->toUnicode("\xE2\x9C\xB6");

    QString text = Amarok::playingStatus();
    QString ratingStr;
    int rating = map["rating"].toInt();
    if (rating == 0) {
        ratingStr = '0';
    } else if (rating == 1) {
        ratingStr = QTextCodec::codecForName("UTF-8")->toUnicode("\xE2\x9C\xB6");
    } else if (rating == 2) {
        ratingStr = QTextCodec::codecForName("UTF-8")->toUnicode("\xE2\x9C\xB6\xE2\x9C\xB6");
    } else if (rating == 3) {
        ratingStr = QTextCodec::codecForName("UTF-8")->toUnicode("\xE2\x9C\xB6\xE2\x9C\xB6\xE2\x9C\xB6");
    } else if (rating == 4) {
        ratingStr = QTextCodec::codecForName("UTF-8")->toUnicode("\xE2\x9C\xB6\xE2\x9C\xB6\xE2\x9C\xB6"
                                                                 "\xE2\x9C\xB6");
    } else {
        ratingStr = QTextCodec::codecForName("UTF-8")->toUnicode("\xE2\x9C\xB6\xE2\x9C\xB6\xE2\x9C\xB6"
                                                                 "\xE2\x9C\xB6\xE2\x9C\xB6");
    }
    text.replace("%t", map["title"].toString());
    text.replace("%a", map["artist"].toString());
    text.replace("%A", map["album"].toString());
    text.replace("%n", map["tracknumber"].toString());
    text.replace("%p", map["time"].toString());
    text.replace("%r", ratingStr);
    text.replace("%y", map["year"].toString());
    text.replace("%d", map["date"].toString());
    text.replace("%g", map["genre"].toString());

    return text;
}

void
AmarokRemote::open()
{
    KProcess::startDetached("amarok");
}
