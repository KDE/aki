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

#ifndef AUTHOR_HPP
#define AUTHOR_HPP

#include "sql/table.hpp"
#include <QtCore/QDate>

class Author
    : public Aki::Sql::Table
{
    Q_OBJECT
    Q_CLASSINFO("id", "PRIMARY_KEY,AUTO_INCREMENT")
    Q_CLASSINFO("name", "LENGTH=0,NULL=false")
    Q_CLASSINFO("birthday", "")
    Q_CLASSINFO("sex", "")
    Q_ENUMS(Sex)
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QDate birthday READ birthday WRITE setBirthday)
    Q_PROPERTY(Sex sex READ sex WRITE setSex)
public:
    enum Sex {
        Male = 0,
        Female = 1
    }; // End of enum Sex.
    Q_INVOKABLE explicit Author(QObject* parent = 0);
    ~Author();
    const QDate& birthday() const;
    int id() const;
    QString name() const;
    void setBirthday(const QDate& date);
    void setId(int id);
    void setName(const QString& name);
    void setSex(Author::Sex sex);
    Author::Sex sex() const;
private:
    int _id;
    QString _name;
    QDate _birthday;
    Author::Sex _sex;
};

Q_DECLARE_METATYPE(Author::Sex)

#endif // AUTHOR_HPP
