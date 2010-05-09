#ifndef AKI_SQLNICKNAME_HPP
#define AKI_SQLNICKNAME_HPP

#include "libaki_export.hpp"
#include <QtCore/QObject>

namespace Aki
{
class SqlServer;
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
     * Retrives all the nicknames that are connected to the @p server.
     * @note You are responsible for the deletion of the pointers in the list.
     *
     * @param server Sql Server.
     *
     * @return List of SqlNickname items if any are found. Empty list otherwise.
     */
    static Aki::SqlNickname::List nicknamesForServer(const Aki::SqlServer* server);
    /**
     * Gets the nickname that has the @p nickname and the @p server.
     * @note You are responsible for the deletion of the pointer returned.
     *
     * @param nickname Name of the nickname to look for.
     * @param server Server the nickname will look for in.
     *
     * @return Nickname that was found; 0 if the nickname was not found.
     */
    static Aki::SqlNickname* findNickname(const QString& nickname, const Aki::SqlServer* server);
    /**
     * Gets the newly created nickname with @p nickname and assigns it the @p server table id.
     * @note You are responsible for the deletion of the pointer returned.
     *
     * @param nickname Name of the new SqlNickname.
     * @param server Server the new SqlNickname will be connected to.
     *
     * @return New create nickname.
     */
    static Aki::SqlNickname* newNickname(const QString& nickname, const Aki::SqlServer* server);
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
     * Sets the @p id of the server this nickname is connected with.
     *
     * @param id ID of the Server this nickname will belong to.
     */
    void setNicknameServerId(int id);
    /**
     * Gets the id of the server this nickname is connected with.
     *
     * @return ID of the Server this server belongs to.
     */
    int nicknameServerId() const;
    /**
     * Saves any changes the user has made to the SQL database.
     *
     * @return true if it was successful; false otherwise.
     */
    bool save();
    /**
     * Removes the nickname from the current server from the list.
     *
     * @return true if the server was remvoed; false otherwise.
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
    friend class SqlNicknamePrivate;
    QScopedPointer<SqlNicknamePrivate> _d;
}; // End of class SqlNickname.
} // End of namespace Aki.

#endif // AKI_SQLNICKNAME_HPP
