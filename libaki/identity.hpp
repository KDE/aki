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

#ifndef AKI_IDENTITY_HPP
#define AKI_IDENTITY_HPP

#include "aki.hpp"
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>

namespace Aki
{
class IdentityPrivate;
class LIBAKI_EXPORT Identity
{
public:
    typedef QSharedPointer<Aki::Identity> Ptr;
    typedef QList<Aki::Identity::Ptr> List;
    Identity();
    explicit Identity(const QString& name);
    ~Identity();
    QString awayMessage() const;
    QString awayNickname() const;
    QString name() const;
    QString kickMessage() const;
    bool isAwayMessagesEnabled() const;
    bool isMarkLastPositionEnabled() const;
    QString nickname(int index) const;
    QStringList nicknameList() const;
    QString partMessage() const;
    QString quitMessage() const;
    QString realName() const;
    QString returnMessage() const;
    void setAwayMessage(const QString& message);
    void setAwayNickname(const QString& nick);
    void setName(const QString& name);
    void setKickMessage(const QString& message);
    void setMarkLastPosition(bool enabled);
    void setNickname(int index, const QString& nickname);
    void setNicknameList(const QStringList& nicknames);
    void setPartMessage(const QString& message);
    void setQuitMessage(const QString& message);
    void setRealName(const QString& name);
    void setReturnMessage(const QString& message);
    void setUseAwayMessages(bool enabled);
private:
    AKI_DECLARE_PRIVATE(Identity)
}; // End of class Identity.
} // End of namespace Aki.

Q_DECLARE_METATYPE(Aki::Identity*)

#endif // AKI_IDENTITY_HPP
