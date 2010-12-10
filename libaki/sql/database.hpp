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

#ifndef AKI_SQL_DATABASE_HPP
#define AKI_SQL_DATABASE_HPP

#include "aki.hpp"
#include "singleton.hpp"
#include <boost/static_assert.hpp>
#include <QtCore/QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <tr1/type_traits>

namespace Aki
{
namespace Sql
{
class Table;
class DatabasePrivate;
class LIBAKI_EXPORT Database
    : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject* parent = 0);
    explicit Database(const QString& type, const QString& connectionName = QLatin1String(QSqlDatabase::defaultConnection),
                      QObject* parent = 0);
    ~Database();
    /**
     * Closes the database.
     */
    void close();
    /**
     * Creates a database for the given T.
     *
     * @code
     * if (!Aki::Sql::Database::self()->create<Aki::Sql::Identity>()) {
     *     ... Unable to open database ...
     * }
     * @endcode
     *
     * @return true if the database was created; false otherwise.
     */
    template<typename T> bool create();
    /**
     * Gets the database connection object that was set.
     *
     * @return Database connection.
     */
    QSqlDatabase database();
    /**
     * Gets the open state of the database.
     *
     * @return true if the database is open; false otherwise.
     */
    bool isOpen() const;
    /**
     * Gets and opens the database with the connection. If setDatabase was never called
     * or an invalid connect was given this will return false.
     *
     * @return true if the database was open successfully; false otherwise.
     */
    bool open(const QString& path) const;
    /**
     *
     */
    static bool registerClass(Aki::Sql::Table* t);
    /**
     *
     */
    template<typename T> static bool registerClass();
    /**
     * Drops the the table from the database.
     *
     * * @code
     * if (!Aki::Sql::Database::self()->remove<Aki::Sql::Identity>()) {
     *     ... Unable to drop database ...
     * }
     * @endcode
     *
     * @return true if the table was drop; false otherwise.
     */
    template<typename T> bool remove();
    static void removeDatabase(const QString& connectionName);
Q_SIGNALS:
    /**
     * This signal is emitted when an error has occurred.
     *
     * @param error Recent error that has occurred.
     */
    void error(const QSqlError& error);
private:
    AKI_DECLARE_PRIVATE(Database)
}; // End of class Database.

class DatabaseDeleter
{
public:
    static inline void cleanup(Aki::Sql::Database* db)
    {
        db->close();
        delete db;
    }
};

typedef QScopedPointer<Aki::Sql::Database, Aki::Sql::DatabaseDeleter> DatabaseScopedPointer;

template<typename T> bool
Database::create()
{
    using namespace std::tr1;
    BOOST_STATIC_ASSERT((is_base_of<Aki::Sql::Table, T>::value));
    const QMetaObject tableObject = T::staticMetaObject;
    QObject* tObject = tableObject.newInstance();
    bool status = false;
    QMetaObject::invokeMethod(tObject, "create", Q_RETURN_ARG(bool, status));
    return status;
}

template<typename T> bool
Database::remove()
{
    using namespace std::tr1;
    BOOST_STATIC_ASSERT((is_base_of<Aki::Sql::Table, T>::value));
    const QMetaObject tableObject = T::staticMetaObject;
    QObject* tObject = tableObject.newInstance();
    bool status = false;
    QMetaObject::invokeMethod(tObject, "remove", Q_RETURN_ARG(bool, false));
    return false;
}

template<typename T> bool
Database::registerClass()
{
    return Aki::Sql::Database::registerClass(new T);
}

} // End of namespace Sql.
} // End of namespace Aki.

template<typename T>
struct RegisterTable
{
public:
    RegisterTable()
    {
        Aki::Sql::Database::registerClass<T>();
    }
};

#define AKI_REGISTER_SQL_TABLE(TYPE, NAME) \
    RegisterTable<TYPE> AKIreg##NAME;

#endif // AKI_SQL_DATABASE_HPP
