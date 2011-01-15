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

#ifndef AKI_IDENTITYCONFIG_HPP
#define AKI_IDENTITYCONFIG_HPP

#include "aki.hpp"
#include "configfile.hpp"

namespace Aki
{
class LIBAKI_EXPORT IdentityConfig
    : public Aki::ConfigFile
{
    Q_OBJECT
public:
    explicit IdentityConfig(QObject* parent = 0);
    virtual ~IdentityConfig();
    QString awayMessage();
    QString awayNickname();
    QString kickMessage();
    bool isAwayMessagesEnabled();
    bool isMarkLastPositionEnabled();
    QStringList nicknameList();
    QString partMessage();
    QString quitMessage();
    QString realName();
    QString returnMessage();
    void setAwayMessage(const QString& message);
    void setAwayNickname(const QString& nick);
    void setKickMessage(const QString& message);
    void setMarkLastPosition(bool enabled);
    void setNicknameList(const QStringList& nicknames);
    void setPartMessage(const QString& message);
    void setQuitMessage(const QString& message);
    void setRealName(const QString& name);
    void setReturnMessage(const QString& message);
    void setUseAwayMessages(bool enabled);
    QString name();
    void setName(const QString& name);
}; // End of class IdentityConfig.
} // End of namespace Aki.

#endif // AKI_IDENTITYCONFIG_HPP
