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

#ifndef AKI_SQL_QUERY_HPP
#define AKI_SQL_QUERY_HPP

#include "aki.hpp"
#include "debughelper.hpp"
#include <QtCore/QMetaClassInfo>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

namespace Aki
{
namespace Sql
{
class Database;
template<typename T>
class LIBAKI_EXPORT Query
{
public:
    Query();
    Query(const Aki::Sql::Query<T>& other);
    ~Query();
    void setDatabase(Aki::Sql::Database* database);
    Aki::Sql::Query<T>& operator=(const Aki::Sql::Query<T>& other);
    Aki::Sql::Query<T>& bind(const QString& placeholder, const QVariant& value);
    Aki::Sql::Query<T>& groupBy(const QString& fields);
    Aki::Sql::Query<T>& limit(int count);
    Aki::Sql::Query<T>& offset(int count);
    Aki::Sql::Query<T>& orderBy(const QString& field);
    QList<QSharedPointer<T> > result();
    QList<QSharedPointer<T> > result() const;
    Aki::Sql::Query<T>& where(const QString& condition);
private:
    QString _sql;
    Aki::Sql::Database* _database;
    QMetaObject _object;
    QSqlQuery _query;
    QMap<QString, QVariant> _placeHolders;
}; // End of class Query.

template<typename T>
Query<T>::Query()
    : _sql(QString()),
    _database(0)
{
    _object = T::staticMetaObject;
    QString className = _object.className();
    if (className.contains("::")) {
        QString tmp = className.left(className.lastIndexOf("::") + 2);
        className.remove(0, tmp.count());
    }
    QStringList propertyList;
    for (int i = 0, c = _object.classInfoCount(); i < c; ++i) {
        propertyList << QString(_object.classInfo(i).name());
    }
    _sql += QString("SELECT %1 FROM %2\n").arg(propertyList.join(","), className);
}

template<typename T>
Query<T>::Query(const Aki::Sql::Query<T>& other)
    : _sql(other._sql),
    _database(other._database),
    _object(other._object),
    _query(other._query)
{
}

template<typename T>
Query<T>::~Query()
{
}

template<typename T> Aki::Sql::Query<T>&
Query<T>::operator=(const Aki::Sql::Query<T>& other)
{
    _sql = other._sql;
    _database = other._database;
    _object = other.object;
    _query = other._query;
    return *this;
}

template<typename T> Aki::Sql::Query<T>&
Query<T>::bind(const QString& placeholder, const QVariant& value)
{
    if (placeholder.isEmpty()) {
        return *this;
    }

    if (!value.isValid()) {
        return *this;
    }

    _placeHolders.insert(placeholder, value);
    return *this;
}

template<typename T> Aki::Sql::Query<T>&
Query<T>::groupBy(const QString& fields)
{
    if (fields.isEmpty()) {
        return;
    }

    _sql += "GROUP BY " + fields + '\n';
    return *this;
}

template<typename T> Aki::Sql::Query<T>&
Query<T>::limit(int count)
{
    if (count < 0) {
        return *this;
    }

    _sql += "LIMIT " + QString::number(count) + '\n';
    return *this;
}

template<typename T> Aki::Sql::Query<T>&
Query<T>::offset(int count)
{
    if (count < 0) {
        return *this;
    }

    _sql += "OFFSET " + QString::number(count) + '\n';
    return *this;
}

template<typename T> Aki::Sql::Query<T>&
Query<T>::orderBy(const QString& field)
{
    if (field.isEmpty()) {
        return *this;
    }

    _sql += "ORDER BY " + field + "\n";
    return *this;
}

template<typename T> QList<QSharedPointer<T> >
Query<T>::result()
{
    DEBUG_FUNC_NAME;
    QList<QSharedPointer<T> > results;
    DEBUG_TEXT2("Preparing Sql Statement:\n%1", _sql);
    _query.prepare(_sql);

    QMapIterator<QString, QVariant> i(_placeHolders);
    while (i.hasNext()) {
        i.next();
        DEBUG_TEXT2("Binding: %1", i.key());
        _query.bindValue(i.key(), i.value());
    }

    if (_query.exec()) {
        while (_query.next()) {
            T* res = qobject_cast<T*>(_object.newInstance());
            for (int i = 0, c = _object.classInfoCount(); i < c; ++i) {
                const QMetaClassInfo classInfo = _object.classInfo(i);
                const QMetaProperty property = _object.property(_object.indexOfProperty(classInfo.name()));
                const int indexRecord = _query.record().indexOf(classInfo.name());
                if (property.isEnumType()) {
                    QMetaEnum enumData = _object.enumerator(_object.indexOfEnumerator(property.typeName()));
                    res->setProperty(classInfo.name(), enumData.valueToKey(_query.value(indexRecord).toInt()));
                } else {
                    res->setProperty(classInfo.name(), _query.value(indexRecord));
                }
            }

            results.append(QSharedPointer<T>(res));
        }
    } else {
        qDebug() << _query.lastError().text();
    }

    return results;
}

template<typename T> QList<QSharedPointer<T> >
Query<T>::result() const
{
    return result();
}

template<typename T> void
Query<T>::setDatabase(Aki::Sql::Database* database)
{
    Q_ASSERT(database);
    _database = database;
}

template<typename T> Aki::Sql::Query<T>&
Query<T>::where(const QString& condition)
{
    _sql += condition + '\n';
    return *this;
}

} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_SQL_QUERY_HPP
