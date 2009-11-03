/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef AKI_CHATPARSER_H
#define AKI_CHATPARSER_H

#include "libaki_export.h"
#include <QObject>
#include <QScopedPointer>
#include <QStringList>

namespace Aki
{
namespace Irc
{
class User;
}
class BaseWindow;
class ChatParserPrivate;
class ChatParser : public QObject
{
    Q_OBJECT
public:
    ChatParser(QObject *parent = 0);
    ~ChatParser();
    void parse(const QString &data);
    void setWindow(Aki::BaseWindow *window);
    Aki::BaseWindow* window();
Q_SIGNALS:
    void customCommand(const QString &command, const QString &message = QString());
    void whoRequest(const QString &channel);
    void queryMessage(const QString &to, const QString &message);
private:
    friend class ChatParserPrivate;
    QScopedPointer<ChatParserPrivate> d;
}; // End of class ChatParser.
} // End of namespace Aki.

#endif // AKI_CHATPARSER_H
