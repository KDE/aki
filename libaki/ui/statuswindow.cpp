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

#include "statuswindow.h"
#include "config/identityconfig.h"
#include "settings.h"
#include <Aki/Irc/Socket>
#include <KDebug>
#include <QShowEvent>
using namespace Aki;

namespace Aki
{
class StatusWindowPrivate
{
public:
    StatusWindowPrivate(StatusWindow *qq)
        : q(qq)
    {
    }

    void textSubmitted()
    {
        emit q->textSubmitted(q, q->chatInput->text());
        q->chatInput->clear();
    }

    void nickSelectorActivated(const QString &nick)
    {
        q->socket()->rfcNick(nick);
        q->chatInput->setFocus();
    }

    void findNextClicked()
    {
        QWebPage::FindFlags flags;
        if (q->searchBar()->isCaseSensitive()) {
            flags |= QWebPage::FindCaseSensitively;
        }
        if (q->searchBar()->isWrapAround()) {
            flags |= QWebPage::FindWrapsAroundDocument;
        }
        if (q->searchBar()->isHighlightAll()) {
            flags |= QWebPage::HighlightAllOccurrences;
        }

        bool state = q->view()->findText(q->searchBar()->text(),
                                         QWebPage::FindBackward | flags);
        q->searchBar()->setFound(state);
    }

    void findPreviousClicked()
    {
        QWebPage::FindFlags flags;
        if (q->searchBar()->isCaseSensitive()) {
            flags |= QWebPage::FindCaseSensitively;
        }
        if (q->searchBar()->isWrapAround()) {
            flags |= QWebPage::FindWrapsAroundDocument;
        }
        if (q->searchBar()->isHighlightAll()) {
            flags |= QWebPage::HighlightAllOccurrences;
        }

        bool state = q->view()->findText(q->searchBar()->text(), flags);
        q->searchBar()->setFound(state);
    }

    void channelSearchTextEdited(const QString &text)
    {
        QWebPage::FindFlags flags;
        if (q->searchBar()->isCaseSensitive()) {
            flags |= QWebPage::FindCaseSensitively;
        }
        if (q->searchBar()->isWrapAround()) {
            flags |= QWebPage::FindWrapsAroundDocument;
        }
        if (q->searchBar()->isHighlightAll()) {
            flags |= QWebPage::HighlightAllOccurrences;
        }

        if (text.isEmpty() || text.isNull()) {
            q->searchBar()->resetPalette();
            q->view()->findText("", flags);
        } else {
            bool state = q->view()->findText(text, flags);
            q->searchBar()->setFound(state);
        }
    }

    StatusWindow *q;
    Aki::IdentityConfig *identity;
}; // End of class StatusWindowPrivate.
} // End of namespace Aki.

StatusWindow::StatusWindow(const QString &name, Aki::IdentityConfig *identityConfig,
                           Aki::Irc::Socket *socket, QWidget *parent)
    : Aki::BaseWindow(name, Aki::BaseWindow::StatusWindow, parent)
{
    d.reset(new Aki::StatusWindowPrivate(this));

    setupUi(this);
    setView(chatOutput);
    channelSearch->hide();
    d->identity = identityConfig;
    setSocket(socket);

    connect(chatInput, SIGNAL(textSubmitted()),
            SLOT(textSubmitted()));
    connect(nickSelector, SIGNAL(activated(QString)),
            SLOT(nickSelectorActivated(QString)));
    connect(channelSearch, SIGNAL(findNextClicked()),
            SLOT(findNextClicked()));
    connect(channelSearch, SIGNAL(findPreviousClicked()),
            SLOT(findPreviousClicked()));
    connect(channelSearch, SIGNAL(textEdited(QString)),
            SLOT(channelSearchTextEdited(QString)));
}

StatusWindow::~StatusWindow()
{
}

void
StatusWindow::setNickList(const QStringList &nickList)
{
    nickSelector->clear();
    nickSelector->addItems(nickList);
    nickSelector->setCurrentIndex(nickSelector->findText(socket()->currentNick(), Qt::MatchExactly));
}

Aki::IdentityConfig*
StatusWindow::identity()
{
    return d->identity;
}

void
StatusWindow::setCurrent(bool current)
{
    Aki::BaseWindow::setCurrent(current);
    if (!isCurrent()) {
        chatInput->setFocus();
        setTabColor(Default);
    }
}

void
StatusWindow::addNick(const QString &oldNick, const QString &newNick)
{
    if (oldNick == socket()->currentNick() ||
        newNick == socket()->currentNick()) {
        if (!nickSelector->contains(newNick)) {
            nickSelector->addItem(newNick);
            socket()->appendNick(newNick);
        }

        const int index = nickSelector->findText(newNick, Qt::MatchExactly);
        nickSelector->setCurrentIndex(index);

        if (!Aki::Settings::conferenceMode()) {
            view()->addNick(oldNick, newNick);
        }

        if (!isCurrent()) {
            setTabColor(NewData);
        }
    }
}

void
StatusWindow::showEvent(QShowEvent *event)
{
    chatInput->setFocus();
    Aki::BaseWindow::showEvent(event);
}

bool
StatusWindow::hasInputFocus() const
{
    return chatInput->hasFocus();
}

void
StatusWindow::setNewNick(const QString &nick)
{
    const int index = nickSelector->findText(nick, Qt::MatchExactly);
    nickSelector->setCurrentIndex(index);
}

Aki::SearchBar*
StatusWindow::searchBar()
{
    return channelSearch;
}

#include "statuswindow.moc"
