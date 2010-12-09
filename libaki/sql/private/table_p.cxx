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

#include "table_p.hpp"
#include "debughelper.hpp"
#include <QtCore/QMetaClassInfo>
#include <QtCore/QMetaProperty>
#include <QtCore/QRegExp>
using namespace Aki;
using namespace Sql;

TablePrivate::TablePrivate(Aki::Sql::Table* qq)
    : _q(qq)
{
}

bool
TablePrivate::checkProperties()
{
    const QMetaObject* metaObject(_q->metaObject());
    const int classInfoCount(metaObject->classInfoCount());
    const QString className(metaObject->className());

    for (int i = 0; i < classInfoCount; ++i) {
        QMetaClassInfo classInfo = metaObject->classInfo(i);
        if (!QString(classInfo.value()).contains("ignore", Qt::CaseInsensitive)) {
            if (metaObject->indexOfProperty(classInfo.name())) {
                const QString errorStr = propertyDontMatchErrorString(classInfo.name());
            }
        }
    }

    return true;
}

bool
TablePrivate::getOption(QString* line, QString* value)
{
    if (line->isEmpty()) {
        *value = QString();
        return false;
    }
    if (!line->contains(',')) {
        *line = QString();
        return true;
    }

    *value = line->left(line->indexOf(','));
    line->remove(0, value->length() + 1);

    return true;
}

bool
TablePrivate::parseClassInfo(const QMetaClassInfo& classInfo)
{
    DEBUG_FUNC_NAME
    // PRIMARY_KEY
    // AUTO_INCREMENT
    // LENGTH=<numeric>
    // FOREIGN_KEY=(table);(field)
    // NULL=(true|false)

    const QMetaObject* metaObject = _q->parent()->metaObject();
    DEBUG_TEXT(metaObject->className())
    const QString className = metaObject->className();
    const int index = metaObject->indexOfProperty(classInfo.name());

    Q_ASSERT_X(index != -1, __PRETTY_FUNCTION__,
               qPrintable(QString("%1 does not contain a property called '%'").arg(className, QString(classInfo.name()))));

    const QMetaProperty property = metaObject->property(index);
    QVariant::Type type = property.type();

    if (type == QVariant::Invalid) {
        return false;
    }

    const QStringList optionList = QString(classInfo.value()).split(',', QString::SkipEmptyParts);
    if (!optionList.count()) {
        return true;
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

    qDebug() << "Primary: " << isPrimaryKey;
    qDebug() << "Unique: " << isUnique;
    qDebug() << "AutoIncrement: " << isAutoIncrement;
    qDebug() << "Null: " << isNull;
    qDebug() << "Length: " << isLength;
    qDebug() << "-- Count: " << length;
    qDebug() << "Foreign: " << isForeignKey;
    qDebug() << "-- Table: " << foreignTable;
    qDebug() << "-- Field: " << foreignTableField;

    return false;
}

bool
TablePrivate::parseMetaOptions()
{
    const QMetaObject* metaObject(_q->metaObject());
    const int classInfoCount(metaObject->classInfoCount());
    const QString className(metaObject->className());

    for (int i = 0; i < classInfoCount; ++i) {
        const QMetaClassInfo classInfo = metaObject->classInfo(i);
        kDebug() << parseClassInfo(classInfo);
    }
    return true;
}

QString
TablePrivate::propertyDontMatchErrorString(const QString& propertyName)
{
    const QString className(_q->metaObject()->className());
    const QString str(QString("%1 contains a class info for \"%2\" name but does not contain a property for it")
                        .arg(className, propertyName));
    return str;
}
