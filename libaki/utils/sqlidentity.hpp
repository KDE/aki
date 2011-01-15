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

#ifndef AKI_SQLIDENTITY_HPP
#define AKI_SQLIDENTITY_HPP

#include "aki.hpp"
#include <QtCore/QObject>
#include <QtCore/QStringList>

namespace Aki
{
class SqlIdentityPrivate;
class LIBAKI_EXPORT SqlIdentity
    : public QObject
{
    Q_OBJECT
public:
    virtual ~SqlIdentity();
    bool operator==(const Aki::SqlIdentity& rhs) const;
    bool operator!=(const Aki::SqlIdentity& rhs) const;
    static QStringList identityNames();
    static Aki::SqlIdentity* newIdentity(const QString& name);
    static Aki::SqlIdentity* findIdentity(const QString& name);
    int id() const;
    void setAwayMessage(const QString& message);
    QString awayMessage() const;
    void setAwayNickname(const QString& nickname);
    QString awayNickname() const;
    void setName(const QString& name);
    QString name() const;
    void setKickMessage(const QString& message);
    QString kickMessage() const;
    void setEnableMessages(bool enabled);
    bool isMessagesEnabled() const;
    void setMarkLastPosition(bool enabled);
    bool isMarkLastPositionEnabled() const;
    void setPartMessage(const QString& message);
    QString partMessage() const;
    void setQuitMessage(const QString& message);
    QString quitMessage() const;
    void setRealName(const QString& name);
    QString realName() const;
    void setReturnMessage(const QString& message);
    QString returnMessage() const;
    bool save();
    bool remove();
protected:
    explicit SqlIdentity(QObject* parent = 0);
    void setId(int id);
private:
    AKI_DECLARE_PRIVATE(SqlIdentity)
}; // End of class SqlIdentity.
} // End of namespace Aki.

#endif // AKI_SQLIDENTITY_HPP
