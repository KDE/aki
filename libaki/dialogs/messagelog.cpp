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

#include "messagelog.h"
#include <KLocale>
#include <KTextBrowser>
using namespace Aki;

namespace Aki
{
class MessageLogPrivate
{
public:
    MessageLogPrivate(Aki::MessageLog *qq)
        : q(qq),
        view(0)
    {
    }

    void closeClicked()
    {
        view->clear();
    }

    void clearClicked()
    {
        view->clear();
    }
    
    Aki::MessageLog *q;
    KTextBrowser *view;
}; // End of class MessageLogPrivate.
} // End of namespace Aki.

MessageLog::MessageLog(QWidget *parent)
    : KDialog(parent),
    d(new MessageLogPrivate(this))
{
    resize(635, 322);
    d->view = new KTextBrowser(this, true);
    setMainWidget(d->view);
    setButtons(User1 | Close);
    setButtonText(User1, i18nc("Clear the text view", "Clear"));
    setWindowTitle(i18n("Aki Message Log"));

    connect(this, SIGNAL(user1Clicked()),
            SLOT(clearClicked()));
    connect(this, SIGNAL(closeClicked()),
            SLOT(closeClicked()));
}

MessageLog::~MessageLog()
{
}

void
MessageLog::addMessage(bool in, const QString &message)
{
    if (!isVisible()) {
        return;
    }

    if (in) {
        d->view->append(">> " + message);
    } else {
        d->view->append("<< " + message);
    }
}

#include "messagelog.moc"
