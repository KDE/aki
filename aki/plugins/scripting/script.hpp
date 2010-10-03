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

#ifndef AKI_SCRIPT_HPP
#define AKI_SCRIPT_HPP

#include <QtCore/QObject>

namespace Kross
{
class Action;
} // End of namespace Kross.

namespace Aki
{
class Script : public QObject
{
    Q_OBJECT
public:
    class MetaData {
    public:
        QString author;
        QString comment;
        QString email;
        QString icon;
        QString interpreter;
        QString name;
        QString website;
    }; // End of class MetaData.

    explicit Script(QObject* parent = 0);
    ~Script();
    bool execute();
    bool isEnabled() const;
    bool isRunning() const;
    bool loadDesktopFile(const QString& file);
    Aki::Script::MetaData metaData() const;
    void setCode(const QString& code);
    void setEnabled(bool enable);
    void setInterpreter(const QString& interpreter);
    void stop();
private:
    Kross::Action* _action;
    Script::MetaData _meta;
    QString _file;
    QString _actionName;
    bool _isRunning;
    bool _isEnabled;
}; // End of class Script.
} // End of namespace Aki.

#endif // AKI_SCRIPT_HPP
