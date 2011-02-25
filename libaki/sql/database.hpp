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

#ifndef AKI_SQL_DATABASE_HPP
#define AKI_SQL_DATABASE_HPP

#include "aki.hpp"
#include "debughelper.hpp"
#include "singleton.hpp"
#include "sql/metatable.hpp"
#include "sql/query.hpp"
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
    Q_PROPERTY(QString connectOptions READ connectOptions WRITE setConnectOptions)
    Q_PROPERTY(QString connectionName READ connectionName)
    Q_PROPERTY(bool isOpen READ isOpen)
    Q_PROPERTY(int port READ port WRITE setPort)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(QString databaseName READ databaseName WRITE setDatabaseName)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName)
    Q_PROPERTY(QString userName READ userName WRITE setUserName)
public:
    /**
     * Creates a new Database object.
     *
     * @param parent Parent of the object.
     */
    explicit Database(QObject* parent = 0);
    /**
     * Creates a new Database object with the SQL @p type and option @p connectionName.
     *
     * @param type SQL type QSQLITE, QMYSQL, etc.
     * @param connectionName Connection name for this connection. Defaults to defaultConnection.
     * @param parent Parent of the object.
     */
    explicit Database(const QString& type, const QString& connectionName = QLatin1String(QSqlDatabase::defaultConnection),
                      QObject* parent = 0);
    ~Database();
    template<typename T> bool add(T* data);
    /**
     * Closes the database.
     */
    void close();
    /**
     *
     */
    bool commit();
    /**
     * Gets the connection name that was set in the constructor if any.
     *
     * @return QSqlDatabase::defaultConnection if the one in the constructor was not set.
     */
    QString connectionName() const;
    /**
     *
     */
    QString connectOptions() const;
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
     * Gets the database name if using a SQL server to connect to or the path.
     *
     * @return SQL database name if using an SQL server else the path to the sql file.
     */
    QString databaseName() const;
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
    template<typename T> bool drop();
    /**
     *
     */
    template<typename T> Aki::Sql::Query<T> find();
    /**
     * Gets the host name of the connection. Usually localhost.
     *
     * @return Host name of the server.
     */
    QString hostName() const;
    /**
     * Gets the open state of the database.
     *
     * @return true if the database is open; false otherwise.
     */
    bool isOpen() const;
    /**
     * Opens the database connection for use.
     *
     * @return true if the database was successfully opened; false otherwise.
     */
    bool open();
    /**
     * Opens the database connection for use with the specified @p user and @p password.
     *
     * @return true if the database was successfully opened; false otherwise.
     */
    bool open(const QString& user, const QString& password);
    /**
     * Gets the password for the connection.
     *
     * @return Password for the connection if used; else empty QString.
     */
    QString password() const;
    /**
     * Gets the port for the connection.
     *
     * @return Port for the connection.
     */
    int port() const;
    /**
     * Registers the @p t with the database.
     * @internal internal use only.
     *
     * @return true if the class was successfully registered; false otherwise.
     */
    static bool registerClass(Aki::Sql::Table* t);
    /**
     * Registers the class with the database.
     * @internal internal use only.
     *
     * @return true if the class was successfully registered; false otherwise.
     */
    template<typename T> static bool registerClass();
    /**
     *
     */
    template<typename T> bool remove(T* data);
    /**
     * Removes the database with the @p connectionName from the global database.
     *
     * @param connectionName Connection name of the database.
     */
    static void removeDatabase(const QString& connectionName);
    /**
     *
     */
    bool rollback();
    /**
     *
     */
    void setConnectOptions(const QString& options = QString());
    /**
     *
     */
    void setDatabaseName(const QString& name);
    /**
     *
     */
    void setHostName(const QString& host);
    /**
     *
     */
    void setPassword(const QString& password);
    /**
     *
     */
    void setPort(int port);
    /**
     *
     */
    void setUserName(const QString& name);
    /**
     *
     */
    bool transaction();
    template<typename T> bool update(T* data);
    /**
     *
     */
    QString userName() const;
Q_SIGNALS:
    /**
     * This signal is emitted when an error has occurred.
     *
     * @param error Recent error that has occurred.
     */
    void error(const QSqlError& error);
protected:
    static Aki::Sql::MetaTable metaTable(const Aki::Sql::Table* table);
private:
    friend class Aki::Sql::Table;
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
Database::add(T* data)
{
    //DEBUG_FUNC_NAME;
    using namespace std::tr1;
    Q_ASSERT(data);
    AKI_STATIC_ASSERT((is_base_of<Aki::Sql::Table, T>::value));

    QStringList propertyList;
    QStringList params;
    QVariantList valueList;
    for (int i = 0, c = data->metaObject()->classInfoCount(); i < c; ++i) {
        const QString name = data->metaObject()->classInfo(i).name();
        params << "?";
        propertyList << name;
        valueList << data->property(name.toAscii().constData());
    }

    QString mainField = propertyList.at(0);
    propertyList.removeAt(0);
    valueList.removeAt(0);
    params.removeAt(0);

    QString className = data->metaObject()->className();
    if (className.contains("::")) {
        QString tmp = className.left(className.lastIndexOf("::") + 2);
        className.remove(0, tmp.count());
    }

    QString str = QString("INSERT INTO %1 (%2) VALUES(%3)").arg(className, propertyList.join(","), params.join(","));

    QSqlQuery query(str);
    for (int i = 0, c = propertyList.count(); i < c; ++i) {
        query.addBindValue(valueList.at(i));
    }


    if (query.exec()) {
        data->setProperty(mainField.toUtf8().constData(), query.lastInsertId());
        query.clear();
        return true;
    } else {
        qDebug() << str;
        qDebug() << query.lastError().text();
    }

    query.clear();
    return false;
}

template<typename T> bool
Database::create()
{
    using namespace std::tr1;
    AKI_STATIC_ASSERT((is_base_of<Aki::Sql::Table, T>::value));
    const QMetaObject tableObject = T::staticMetaObject;
    T* tObject = qobject_cast<T*>(tableObject.newInstance());
    Q_ASSERT(tObject);
    bool status = false;
    QMetaObject::invokeMethod(tObject, "create", Q_RETURN_ARG(bool, status));
    return status;
}

template<typename T> bool
Database::drop()
{
    using namespace std::tr1;
    AKI_STATIC_ASSERT((is_base_of<Aki::Sql::Table, T>::value));
    const QMetaObject tableObject = T::staticMetaObject;
    QObject* tObject = tableObject.newInstance();
    Q_ASSERT(tObject);
    bool status = false;
    QMetaObject::invokeMethod(tObject, "remove", Q_RETURN_ARG(bool, status));
    return false;
}

template<typename T> Aki::Sql::Query<T>
Database::find()
{
    using namespace std::tr1;
    AKI_STATIC_ASSERT((is_base_of<Aki::Sql::Table, T>::value));
    return Aki::Sql::Query<T>();
}

template<typename T> bool
Database::registerClass()
{
    return Aki::Sql::Database::registerClass(new T);
}

template<typename T> bool
Database::remove(T* data)
{
    using namespace std::tr1;
    Q_ASSERT(data);
    AKI_STATIC_ASSERT((is_base_of<Aki::Sql::Table, T>::value));

    QStringList propertyList;
    QVariantList valueList;
    for (int i = 0, c = data->metaObject()->classInfoCount(); i < c; ++i) {
        propertyList << QString(data->metaObject()->classInfo(i).name());
        valueList << data->property(data->metaObject()->classInfo(i).name());
    }

    QString className = data->metaObject()->className();
    if (className.contains("::")) {
        QString tmp = className.left(className.lastIndexOf("::") + 2);
        className.remove(0, tmp.count());
    }

    QString str = QString("DELETE FROM %1 WHERE ").arg(className);
    for (int i = 0, c = propertyList.count(); i < c; ++i) {
        str += propertyList.at(i) + "='" + valueList.at(i).toString() + "' AND ";
    }

    QSqlQuery query;
    if (query.exec(str)) {
        query.clear();
        return true;
    }

    query.clear();
    return false;
}

template<typename T> bool
Database::update(T* data)
{
    using namespace std::tr1;
    Q_ASSERT(data);
    AKI_STATIC_ASSERT((is_base_of<Aki::Sql::Table, T>::value));

    QStringList propertyList;
    QVariantList valueList;
    for (int i = 0, c = data->metaObject()->classInfoCount(); i < c; ++i) {
        propertyList << QString(data->metaObject()->classInfo(i).name());
        valueList << data->property(data->metaObject()->classInfo(i).name());
    }

    QString className = data->metaObject()->className();
    if (className.contains("::")) {
        QString tmp = className.left(className.lastIndexOf("::") + 2);
        className.remove(0, tmp.count());
    }

    QString str = QString("UPDATE %1 SET").arg(className);
    for (int i = 0, c = propertyList.count(); i < c; ++i) {
        str += ' ' + propertyList.at(i) + "='" +  valueList.at(i).toString() + "',";
    }
    str.remove(str.length() - 1, 1);
    str += " WHERE " + propertyList.at(0) + "='" + valueList.at(0).toString() + "'";

    QSqlQuery query;
    if (query.exec(str)) {
        query.clear();
        return true;
    }

    query.clear();
    return false;
}

} // End of namespace Sql.
} // End of namespace Aki.

/**
 * @internal Internal use only.
 */
template<typename T>
struct RegisterTable
{
public:
    RegisterTable()
    {
        DEBUG_FUNC_NAME;
        if (Aki::Sql::Database::registerClass<T>()) {
            DEBUG_TEXT("Successfully registered class");
        } else {
            DEBUG_TEXT("Unable to register class");
        }
    }
};

/**
 * Registers the class @p TYPE in the global table registry
 * with the @p BAME.
 *
 * @param TYPE Table class to register.
 * @param NAME Name to help with registering.
 */
#define AKI_REGISTER_SQL_TABLE(TYPE, NAME) \
    RegisterTable<TYPE> AKIreg##NAME;

#endif // AKI_SQL_DATABASE_HPP
