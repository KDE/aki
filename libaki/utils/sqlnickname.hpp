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

#ifndef AKI_SQLNICKNAME_HPP
#define AKI_SQLNICKNAME_HPP

#include "aki.hpp"
#include <QtCore/QObject>

namespace Aki
{
class SqlIdentity;
class SqlNicknamePrivate;
/**
 * Represents the Nickname table in the configuration database.
 */
class LIBAKI_EXPORT SqlNickname : public QObject
{
    Q_OBJECT
public:
    /**
     * Typedef of QList<Aki::SqlNickname*>.
     */
    typedef QList<Aki::SqlNickname*> List;
    /**
     * Destroys the object.
     */
    virtual ~SqlNickname();
    /**
     * Compares two SqlNickname's if they are the same based on their content.
     *
     * @param rhs Right hand side of the ==.
     *
     * @return true if the two SqlNickname's contents are the same.
     */
    bool operator==(const Aki::SqlNickname& rhs) const;
    /**
     * Compares two SqlNicknamer's if they are not the same based on their content.
     *
     * @param rhs Right hand side of the !=.
     *
     * @return true if the two SqlNickname's contents are different.
     */
    bool operator!=(const Aki::SqlNickname& rhs) const;
    /**
     * Retrives all the nicknames that are connected to the @p identity.
     * @note You are responsible for the deletion of the pointers in the list.
     *
     * @param identity Sql Identity.
     *
     * @return List of SqlNickname items if any are found. Empty list otherwise.
     */
    static Aki::SqlNickname::List nicknamesForIdentity(const Aki::SqlIdentity* identity);
    /**
     * Gets the nickname that has the @p nickname and the @p identity.
     * @note You are responsible for the deletion of the pointer returned.
     *
     * @param nickname Name of the nickname to look for.
     * @param identity Identity the nickname will look for in.
     *
     * @return Nickname that was found; 0 if the nickname was not found.
     */
    static Aki::SqlNickname* findNickname(const QString& nickname, const Aki::SqlIdentity* identity);
    /**
     * Gets the newly created nickname with @p nickname and assigns it the @p identity table id.
     * @note You are responsible for the deletion of the pointer returned.
     *
     * @param nickname Name of the new SqlNickname.
     * @param identity Identity the new SqlNickname will be connected to.
     *
     * @return New create nickname.
     */
    static Aki::SqlNickname* newNickname(const QString& nickname, const Aki::SqlIdentity* identity);
    /**
     * Sets the @p name
     *
     * @param name Nickname.
     */
    void setNickname(const QString& name);
    /**
     * Gets the nickname of this nickname.
     *
     * @return Nickname,
     */
    QString nickname() const;
    /**
     * Gets the id of the row this nickname is in.
     *
     * @return Nickname row id.
     */
    int id() const;
    /**
     * Sets the @p id of the identity this nickname is connected with.
     *
     * @param id ID of the Identity this nickname will belong to.
     */
    void setNicknameIdentityId(int id);
    /**
     * Gets the id of the identity this nickname is connected with.
     *
     * @return ID of the Identity this nickname belongs to.
     */
    int nicknameIdentityId() const;
    /**
     * Saves any changes the user has made to the SQL database.
     *
     * @return true if it was successful; false otherwise.
     */
    bool save();
    /**
     * Removes the nickname from the current identity from the list.
     *
     * @return true if the identity was removed; false otherwise.
     */
    bool remove();
protected:
    /**
     * Creates a new SqlNickname object.
     *
     * @param parent Parent of the object.
     */
    SqlNickname(QObject* parent = 0);
    /**
     * Sets the @p id for this nickname.
     * @param id This should never be called. Internal use only.
     */
    void setId(int id);
private:
    AKI_DECLARE_PRIVATE(SqlNickname)
}; // End of class SqlNickname.
} // End of namespace Aki.

#endif // AKI_SQLNICKNAME_HPP
