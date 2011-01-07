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

#include "user.hpp"
#include "private/user_p.hpp"
using namespace Aki;
using namespace Irc;

User::User(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::Irc::UserPrivate);
}

User::~User()
{
}

QString
User::awayMessage() const
{
    return _d->awayMessage;
}

QColor
User::colour() const
{
    return _d->colour;
}

bool
User::containsMode(const QChar& mode) const
{
    if (!mode.isLetter() || mode.isNull()) {
        return false;
    }

    return _d->modes.contains(mode, Qt::CaseSensitive);
}

QDateTime
User::idleTime() const
{
    return _d->idleTime;
}

bool
User::isAway() const
{
    return _d->isAway;
}

bool
User::isHalfOp() const
{
    return containsMode('h');
}

bool
User::isNormal() const
{
    if (isOp() || isHalfOp() || isVoice()) {
        return false;
    }

    return true;
}

bool
User::isOp() const
{
    return containsMode('o');
}

bool
User::isVoice() const
{
    return containsMode('v');
}

QString
User::modes() const
{
    return _d->modes;
}

Aki::Irc::NickInfo&
User::nickInfo() const
{
    return _d->nickInfo;
}

void
User::removeModes(const QString& modes)
{
    if (modes.isEmpty()) {
        return;
    }

    QString newModes = this->modes();

    foreach (const QChar& c, modes) {
        if (containsMode(c)) {
            newModes.remove(c);
        }
    }

    _d->modes = newModes;
}

void
User::setAway(bool away)
{
    _d->isAway = away;
}

void
User::setAwayMessage(const QString& message)
{
    _d->awayMessage = message;
}

void
User::setColour(const QColor& colour)
{
    _d->colour = colour;
}

void
User::setIdleTime(const QDateTime& idleTime)
{
    _d->idleTime = idleTime;
}

void
User::setModes(const QString& modes)
{
    QString tmp = this->modes();

    foreach (const QChar c, modes) {
        if (!containsMode(c)) {
            tmp.append(c);
        }
    }

    _d->modes = tmp;
}

void
User::setNickInfo(const Aki::Irc::NickInfo& info)
{
    _d->nickInfo = info;
}

#include "irc/user.moc"
