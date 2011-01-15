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

#include "database_p.hpp"
#include "sql/table.hpp"
using namespace Aki::Sql;

Aki::Sql::TableList Aki::Sql::DatabasePrivate::tableList = Aki::Sql::TableList();

DatabasePrivate::DatabasePrivate(Aki::Sql::Database* qq)
    : _q(qq)
{
}

bool
DatabasePrivate::parseClassInfo(const QMetaObject* object, const QMetaClassInfo& classInfo)
{
    const QMetaObject* metaObject = object;
    const QString className = metaObject->className();
    const int index = metaObject->indexOfProperty(classInfo.name());

    Q_ASSERT_X(index != -1, __PRETTY_FUNCTION__,
               qPrintable(QString("%1 does not contain a property called '%2'").arg(className, QString(classInfo.name()))));

    const QMetaProperty property = metaObject->property(index);
    QVariant::Type type = property.type();

    if (type == QVariant::Invalid) {
        qDebug() << "Invalid type";
        return false;
    }

    const QStringList optionList = QString(classInfo.value()).split(',', QString::SkipEmptyParts);
    if (!optionList.count()) {
        return false;
    }

    bool isPrimaryKey = false;
    bool isUnique = false;
    bool isAutoIncrement = false;
    bool isNull = false;
    bool isLength = false;
    int length = -1;
    bool isForeignKey = false;
    QString foreignTable;
    QString foreignTableField;

    Q_UNUSED(isForeignKey)

    foreach (const QString &optionValue, optionList) {
        if (!optionValue.contains('=')) {
            if (optionValue == QString("PRIMARY_KEY")) {
                isPrimaryKey = true;
            } else if (optionValue == QString("UNIQUE")) {
                Q_ASSERT_X(type == QVariant::String, __PRETTY_FUNCTION__,
                           "UNIQUE is only useful for fields that have type of string");
                isUnique = true;
            } else if (optionValue == QString("AUTO_INCREMENT")) {
                isAutoIncrement = true;
            }
        } else {
            const QStringList optionValueSplit = optionValue.split('=', QString::SkipEmptyParts);

            Q_ASSERT_X(optionValueSplit.count() == 2, __PRETTY_FUNCTION__,
                       qPrintable(QString("Missing option or value of class info '%1'").arg(QString(classInfo.name()))));

            const QString option = optionValueSplit.at(0);
            const QString value = optionValueSplit.at(1);

            if (option == QString("NULL")) {
                if (value.toLower() == QString("false")) {
                    isNull = false;
                } else if (value.toLower() == QString("true")) {
                    isNull = true;
                } else {
                    Q_ASSERT_X(false, __PRETTY_FUNCTION__, "NULL contains invalid value, use true or false");
                }
            } else if (option == QString("LENGTH")) {
                isLength = true;
                bool okInt;
                length = value.toInt(&okInt);
                Q_ASSERT_X(okInt, __PRETTY_FUNCTION__, "Unable to convert LENGTH's value to integer");

            } else if (option == QString("FOREIGN_KEY")) {
                Q_ASSERT_X(value.contains(';'), __PRETTY_FUNCTION__, "FOREIGN_KEY does not contain an ';'");
                const QStringList tableId = value.split(';', QString::SkipEmptyParts);
                Q_ASSERT_X(tableId.count() == 2, __PRETTY_FUNCTION__,
                           qPrintable(QString("Missing table or field for FOREIGN_KEY in class info %1").arg(classInfo.name())));
                foreignTable = tableId.at(0);
                foreignTableField = tableId.at(1);
            }
        }
    }

    return true;
}

bool
DatabasePrivate::parseMetaObjects(Aki::Sql::Table* table)
{
    const QMetaObject* metaObject(table->metaObject());
    const int classInfoCount(metaObject->classInfoCount());
    const QString className(metaObject->className());
    qDebug() << "--> " << className;

    for (int i = 0; i < classInfoCount; ++i) {
        const QMetaClassInfo classInfo = metaObject->classInfo(i);
        parseClassInfo(metaObject, classInfo);
    }
    return true;
}
