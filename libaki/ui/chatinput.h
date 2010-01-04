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

#ifndef CHATINPUT_H
#define CHATINPUT_H

#include "libaki_export.h"
#include <KLineEdit>
#include <QScopedPointer>

namespace Aki
{
class ChatInputPrivate;
class LIBAKI_EXPORT ChatInput : public KLineEdit
{
    Q_OBJECT
public:
    ChatInput(QWidget *parent = 0);
    ~ChatInput();
    void setNickCompletionList(const QStringList &list);
    void insertCompletion(const QString &nick);
Q_SIGNALS:
    void showCompletion();
    void textSubmitted();
protected:
    virtual bool event(QEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
private:
    friend class ChatInputPrivate;
    QScopedPointer<ChatInputPrivate> d;
private:
    Q_PRIVATE_SLOT(d, void insertCompletion(const QString &nick))
}; // End of class ChatInput.
} // End of namespace Aki.

#endif // CHATINPUT_H
