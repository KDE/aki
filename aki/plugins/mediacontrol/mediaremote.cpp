/*
    This file is part of Aki IRC.
    Copyright 2009 - 2010 Keith Rusler <xzekex@live.co.uk>

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

#include "mediaremote.h"
#include "media.h"
#include <KProcess>
#include <QtDBus>

MediaRemote::MediaRemote()
    : _name("amarok"),
    _interfaceName("org.freedesktop.MediaPlayer"),
    _interfaceService("org.mpris.amarok")
{
    _interface.reset(new QDBusInterface(_interfaceService, "/Player", _interfaceName,
                                        QDBusConnection::sessionBus()));
}

MediaRemote::~MediaRemote()
{
}

void
MediaRemote::exit()
{
    QDBusMessage message = QDBusMessage::createMethodCall(_interfaceService, "/", _interfaceName, "Quit");
    QDBusConnection::systemBus().call(message, QDBus::NoBlock);
}

QString
MediaRemote::metaData() const
{
    QDBusReply<QVariantMap> msg = _interface->call("GetMetadata");
    QVariantMap map = msg.value();
    if (map.isEmpty()) {
        return QString();
    }

    const QString star = QString::fromUtf8("✶");

    QString text = Media::playingStatus();
    QString ratingStr;
    int rating = map["rating"].toInt();
    if (rating == 0) {
        ratingStr = "No Rating";
    } else if (rating == 1) {
        ratingStr = star;
    } else if (rating == 2) {
        ratingStr = star + star;
    } else if (rating == 3) {
        ratingStr = star + star + star;
    } else if (rating == 4) {
        ratingStr = star + star + star + star;
    } else {
        ratingStr = star + star + star + star + star;
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
    text.replace("%c", _name);
    text.replace("%m", QString());

    return text;
}

void
MediaRemote::nextTrack()
{
    _interface->call("Next");
}

void
MediaRemote::open()
{
    if (_name == "vlc") {
        KProcess::startDetached("vlc");
    } else if (_name == "amarok") {
        KProcess::startDetached("amarok");
    }
}

void
MediaRemote::pause()
{
    _interface->call("Pause");
}

void
MediaRemote::play()
{
    _interface->call("Play");
}

void
MediaRemote::previousTrack()
{
    _interface->call("Prev");
}

void
MediaRemote::setPlayer(const QString& name)
{
    _name = name;
    _interfaceService = QString("org.mpris.%1").arg(_name);
    _interface.reset(new QDBusInterface(_interfaceService, "/Player", _interfaceName,
                                        QDBusConnection::sessionBus()));
}

void
MediaRemote::setRepeat(bool state)
{
    _interface->call("Repeat", state);
}

void
MediaRemote::setVolume(int volume)
{
    _interface->call("VolumeSet", qBound(0, volume, 100));
}

void
MediaRemote::stop()
{
    _interface->call("Stop");
}
