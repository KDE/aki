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

#ifndef AKI_IRC_USER_HPP
#define AKI_IRC_USER_HPP

#include "aki.hpp"
#include "irc/nickinfo.hpp"
#include <QtCore/QDateTime>
#include <QtCore/QObject>
#include <QtGui/QColor>

namespace Aki
{
namespace Irc
{
class UserPrivate;
class LIBAKI_EXPORT User
    : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString awayMessage READ awayMessage WRITE setAwayMessage)
    Q_PROPERTY(QDateTime idleTime READ idleTime WRITE setIdleTime)
    Q_PROPERTY(QColor colour READ colour WRITE setColour)
    Q_PROPERTY(bool isAway READ isAway WRITE setAway)
    Q_PROPERTY(bool isHalfOp READ isHalfOp)
    Q_PROPERTY(bool isNormal READ isNormal)
    Q_PROPERTY(bool isOp READ isOp)
    Q_PROPERTY(bool isVoice READ isVoice)
    Q_PROPERTY(NickInfo nickInfo READ nickInfo WRITE setNickInfo)
public:
    User(QObject* parent = 0);
    virtual ~User();
    QString awayMessage() const;
    QColor colour() const;
    bool containsMode(const QChar& mode) const;
    QDateTime idleTime() const;
    bool isAway() const;
    bool isHalfOp() const;
    bool isNormal() const;
    bool isOp() const;
    bool isVoice() const;
    Aki::Irc::NickInfo& nickInfo() const;
    void setAway(bool away);
    void setAwayMessage(const QString& message);
    void setColour(const QColor& colour);
    void setIdleTime(const QDateTime& idleTime);
    void setNickInfo(const Aki::Irc::NickInfo& info);
private:
    AKI_DECLARE_PRIVATE(Aki::Irc::User)
}; // End of class User.
} // End of namespace Irc.
} // End of namespace Aki.

Q_DECLARE_METATYPE(Aki::Irc::User*)

#endif // AKI_IRC_USER_HPP
