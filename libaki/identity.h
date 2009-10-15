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

#ifndef AKI_IDENTITY_H
#define AKI_IDENTITY_H

#include "libaki_export.h"
#include <QSharedPointer>
#include <QStringList>

namespace Aki
{
/**
 * Identity class to hold information about the identity. Not to be
 * confused with IdentityConfig.
 */
class LIBAKI_EXPORT Identity
{
public:
    /**
     * Creates a identity.
     */
    Identity();
    /**
     * Creates a identity with a name @p name.
     * @param name Identity name.
     */
    explicit Identity(const QString &name);
    /*! @copydoc IdentityConfig::awayMessage() */
    QString awayMessage() const;
    /*! @copydoc IdentityConfig::awayNickname() */
    QString awayNickname() const;
    /**
     * Gets the identity name.
     * @return Identity name.
     */
    QString name() const;
    /*! @copydoc IdentityConfig::kickMessage() */
    QString kickMessage() const;
    /*! @copydoc IdentityConfig::isAwayMessagesEnabled() */
    bool isAwayMessagesEnabled() const;
    /*! @copydoc IdentityConfig::isMarkLastPositionEnabled() */
    bool isMarkLastPositionEnabled() const;
    /**
     * Gets the nickname at the given index @p index.
     * @return Nickname at the given index.
     */
    QString nickname(int index) const;
    /*! @copydoc IdentityConfig::nicknameList() */
    QStringList nicknameList() const;
    /*! @copydoc IdentityConfig::partMessage() */
    QString partMessage() const;
    /*! @copydoc IdentityConfig::quitMessage() */
    QString quitMessage() const;
    /*! @copydoc IdentityConfig::realName() */
    QString realName() const;
    /*! @copydoc IdentityConfig::returnMessage() */
    QString returnMessage() const;
    /*! @copydoc IdentityConfig::setAwayMessage() */
    void setAwayMessage(const QString &message);
    /*! @copydoc IdentityConfig::setAwayNickname() */
    void setAwayNickname(const QString &nick);
    /**
     * Sets the identity name @p name.
     * @param name Identity name.
     */
    void setName(const QString &name);
    /*! @copydoc IdentityConfig::setKickMessage() */
    void setKickMessage(const QString &message);
    /*! @copydoc IdentityConfig::setMarkLastPosition() */
    void setMarkLastPosition(bool enable);
    /**
     * Sets a nickname @p nickname at the index @p index given.
     * @param index Index of the nickname.
     * @param nickname Nickname to replace with.
     */
    void setNickname(int index, const QString &nickname);
    /*! @copydoc IdentityConfig::setNicknameList() */
    void setNicknameList(const QStringList &nicknames);
    /*! @copydoc IdentityConfig::setPartMessage() */
    void setPartMessage(const QString &message);
    /*! @copydoc IdentityConfig::setQuitMessage() */
    void setQuitMessage(const QString &message);
    /*! @copydoc IdentityConfig::setRealName() */
    void setRealName(const QString &name);
    /*! @copydoc IdentityConfig::setReturnMessage() */
    void setReturnMessage(const QString &message);
    /*! @copydoc IdentityConfig::setUseAwayMessages() */
    void setUseAwayMessages(bool enable);
private:
    QStringList m_nicknameList;
    QString m_awayMessage;
    QString m_awayNickname;
    QString m_name;
    QString m_kickMessage;
    QString m_partMessage;
    QString m_quitMessage;
    QString m_realName;
    QString m_returnMessage;
    bool m_isAwayMessagesEnabled;
    bool m_isMarkLastPositionEnabled;
}; // End of class Identity.
typedef QSharedPointer<Identity> IdentityPointer;
typedef QList<IdentityPointer> IdentityList;
} // End of namespace Aki.

#endif // AKI_IDENTITY_H
