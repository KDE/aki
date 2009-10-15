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

#include "chatinput.h"
#include "settings.h"
#include <KCompletionBox>
#include <QKeyEvent>
#include <QStringList>
using namespace Aki;

namespace Aki
{
class ChatInputPrivate
{
public:
    ChatInputPrivate(ChatInput *qq)
        : q(qq),
        completionBox(0),
        lineNumber(0)
    {
        history.empty();
    }

    void insertCompletion(const QString &nick)
    {
        int oldPosition;
        int position;

        oldPosition = position = q->cursorPosition();
        QString input = q->text();

        while (position && input[position - 1] != QChar(' ')) {
            --position;
        }

        input.remove(position, oldPosition - position);

        if  (position) {
            input.insert(position, nick);
            position += nick.length();
        } else {
            const QString prefix = Aki::Settings::nickCompletionPrefix();
            const QString suffix = Aki::Settings::nickCompletionSuffix();
            input.insert(position, prefix + nick + suffix);
            position += prefix.length() + nick.length() + suffix.length();
        }

        q->setText(input + QChar(' '));
        q->setCursorPosition(position + 1);
    }

    ChatInput *q;
    QStringList history;
    KCompletionBox *completionBox;
    int lineNumber;
}; // End of class ChatInputPrivate.
} // End of namespace Aki.

ChatInput::ChatInput(QWidget *parent)
    : KLineEdit(parent),
    d(new ChatInputPrivate(this))
{
    d->completionBox = new KCompletionBox(this);
    connect(d->completionBox, SIGNAL(activated(QString)),
            SLOT(insertCompletion(QString)));
}

ChatInput::~ChatInput()
{
    delete d;
}

bool
ChatInput::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);

        if (ke->key() == Qt::Key_Tab && ke->modifiers() == Qt::NoModifier &&
            !ke->text().isEmpty()) {
            emit showCompletion();
            return true;
        }
    }
    return KLineEdit::event(event);
}

void
ChatInput::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return: {
        if (text().isEmpty()) {
            return;
        }

        d->history << text();
        d->lineNumber = d->history.count();

        if (d->history.count() > HISTORY_LIMIT) {
            d->history.takeFirst();
            d->lineNumber = d->history.count();
        }

        if (d->completionBox->isHidden()) {
            emit textSubmitted();
        } else {
            if (d->completionBox->currentItem()) {
                d->insertCompletion(d->completionBox->currentItem()->text());
            } else {
                d->insertCompletion(d->completionBox->item(0)->text());
            }
            d->completionBox->hide();
        }
        break;
    }
    case Qt::Key_Up: {
        if (d->history.isEmpty()) {
            return;
        }

        if (d->lineNumber <= 0) {
            d->lineNumber = 0;
        } else {
            --d->lineNumber;
        }
        setText(d->history[d->lineNumber]);
        break;
    }
    case Qt::Key_Down: {
        if (d->history.isEmpty()) {
            return;
        }

        if (d->lineNumber >= d->history.count() - 1) {
            d->lineNumber = d->history.count();
            setText(QString());
        } else {
            ++d->lineNumber;
            setText(d->history[d->lineNumber]);
        }
        break;
    }
    case Qt::Key_Backspace: {
        if (!d->completionBox->isHidden()) {
            d->completionBox->hide();
        }
        break;
    }
    default: {
        break;
    }
    }

    KLineEdit::keyPressEvent(event);
}

void
ChatInput::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        if (d->history.isEmpty()) {
            return;
        }

        if (d->lineNumber <= 0) {
            d->lineNumber = 0;
        } else {
            --d->lineNumber;
        }
        setText(d->history[d->lineNumber]);
    } else if (event->delta() < 0) {
        if (d->history.isEmpty()) {
            return;
        }

        if (d->lineNumber >= d->history.count() - 1) {
            d->lineNumber = d->history.count();
            setText(QString());
        } else {
            ++d->lineNumber;
            setText(d->history[d->lineNumber]);
        }
    }

    KLineEdit::wheelEvent(event);
}

void
ChatInput::setNickCompletionList(const QStringList &list)
{
    d->completionBox->setItems(list);
    d->completionBox->popup();
}

#include "chatinput.moc"
