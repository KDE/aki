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

#ifndef AKI_SQL_NICKNAME_HPP
#define AKI_SQL_NICKNAME_HPP

#include "aki.hpp"
#include "sql/database.hpp"
#include "sql/table.hpp"

namespace Aki
{
namespace Sql
{
class NicknamePrivate;
class LIBAKI_EXPORT Nickname
    : public Aki::Sql::Table
{
    Q_OBJECT
    Q_CLASSINFO("id","PRIMARY_KEY,AUTO_INCREMENT")
    Q_CLASSINFO("nickname","LENGTH=0,NULL=false")
    Q_CLASSINFO("nicknameIdentity","FOREIGN_KEY=Identity;id,NULL=false")
    /**
     * This property holds the @p id of the row when it was commited.<br>
     * The @p id is unique. If the @p id is -1, it means it hasn't been committed yet.
     *
     * The default value is -1.
     */
    Q_PROPERTY(int id READ id WRITE setId)
    /**
     * This property holds the user's @p nickname for the server.
     *
     * The default value is an empty QString.
     */
    Q_PROPERTY(QString nickname READ nickname WRITE setNickname)
    /**
     * This property holds the Identity's id of the row when it was commited.<br>
     * The @p nicknameIdentity is an unique. If the @p nicknameIdentity is -1, it means
     * it hasn't been committed yet.
     *
     * The default value is -1.
     */
    Q_PROPERTY(int nicknameIdentity READ nicknameIdentity WRITE setNicknameIdentity)
public:
    Q_INVOKABLE explicit Nickname(QObject* parent = 0);
    ~Nickname();
    /**
     * Gets the id of this data.
     *
     * @return -1 if the data isn't committed yet; otherwise the row of this
     * data when it was committed.
     */
    int id() const;
    /**
     * Gets the user's nickname.
     *
     * @return User's nickname.
     */
    QString nickname() const;
    /**
     * Gets the id of Identity's table this data is connected to.
     *
     * @return -1 if the data isn't committed yet; otherwise the row of this
     * data when it was committed.
     */
    int nicknameIdentity() const;
    void setId(int id);
    /**
     * Sets the user's nickname.
     * @note This cannot be nulled.
     *
     * @param nickname User's nickname.
     */
    void setNickname(const QString& nickname);
    void setNicknameIdentity(int id);
private:
    AKI_DECLARE_PRIVATE(Nickname)
}; // End of class Nickname.
} // End of namespace Sql.
} // End of namespace Aki.

#endif // AKI_SQL_NICKNAME_HPP
