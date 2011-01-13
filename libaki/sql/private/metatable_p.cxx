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

#include "metatable_p.hpp"
#include "debughelper.hpp"
using namespace Aki;
using namespace Sql;

MetaTablePrivate::MetaTablePrivate()
    : QSharedData(),
    name(QString())
{
}

MetaTablePrivate::MetaTablePrivate(const Aki::Sql::MetaTablePrivate& other)
    : QSharedData(other),
    fields(other.fields),
    name(other.name)
{
}

bool
MetaTablePrivate::parseClassInfo(const QMetaObject* object, const QMetaClassInfo& classInfo)
{
    const QMetaObject* metaObject(object);
    const QString className(metaObject->className());
    const int index(metaObject->indexOfProperty(classInfo.name()));

    Q_ASSERT_X(index != -1, __PRETTY_FUNCTION__,
               qPrintable(QString("%1 does not contain a property called '%2'").arg(className, QString(classInfo.name()))));

    const QMetaProperty property = metaObject->property(index);

    Aki::Sql::Field field(classInfo.name(), property.type());
    Q_ASSERT(field.type() != QVariant::Invalid);

    const QStringList optionList = QString(classInfo.value()).split(',', QString::SkipEmptyParts);

    foreach (const QString& option, optionList) {
        if (!option.contains('=')) {
            if (option == "PRIMARY_KEY") {
                field.setAsPrimaryKey(true);
            } else if (option == "UNIQUE") {
                
            } else if (option == "AUTO_INCREMENT") {
                field.setAutoValue(true);
            }
        } else {
            const QStringList optionSplit = option.split('=', QString::SkipEmptyParts);

            Q_ASSERT_X(optionSplit.count() == 2, __PRETTY_FUNCTION__,
                       qPrintable(QString("Missing option or value of class info '%1'").arg(QString(classInfo.name()))));

            const QString sqlOption = optionSplit.at(0);
            const QString value = optionSplit.at(1);

            if (sqlOption == "NULL") {
                field.setRequiredStatus((value == "true") ? QSqlField::Optional : QSqlField::Required);
            } else if (sqlOption == "LENGTH") {
                field.setLength(value.toInt());
            } else if (sqlOption == "FOREIGN_KEY") {
                Q_ASSERT_X(value.contains(';'), __PRETTY_FUNCTION__, "FOREIGN_KEY does not contain an ';'");
                const QStringList tableId = value.split(';', QString::SkipEmptyParts);
                Q_ASSERT_X(tableId.count() == 2, __PRETTY_FUNCTION__,
                           qPrintable(QString("Missing table or field for FOREIGN_KEY in classinfo '%1'").arg(classInfo.name())));
                field.setForeignKeyTable(tableId.at(0));
                field.setForeignKey(tableId.at(1));
            }
        }
    }

    fields.append(field);
    return true;
}

bool
MetaTablePrivate::parseMetaObjects(Aki::Sql::Table* table)
{
    const QMetaObject* metaObject(table->metaObject());
    const int classInfoCount(metaObject->classInfoCount());
    const QString className(metaObject->className());

    for (int i = 0; i < classInfoCount; ++i) {
        const QMetaClassInfo classInfo = metaObject->classInfo(i);
        Q_ASSERT(parseClassInfo(metaObject, classInfo));
    }

    return true;
}
