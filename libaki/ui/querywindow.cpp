/*
    This file is part of Aki IRC client.
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

#include "querywindow.h"
#include "config/replaceconfig.h"
#include "logfile.h"
#include "settings.h"
#include "ui/channelview.h"
#include <Aki/Irc/Color>
#include <Aki/Irc/Socket>
#include <Aki/Irc/User>
#include <KDebug>
using namespace Aki;

namespace Aki
{
class QueryWindowPrivate
{
public:
    QueryWindowPrivate(QueryWindow *qq)
        : q(qq)
    {
    }

    void textSubmitted()
    {
        emit q->textSubmitted(q, q->chatInput->text());
        q->chatInput->clear();
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

    void optionButtonClicked()
    {
        q->searchBar()->optionButton()->showMenu();
    }

    void findTextTriggered()
    {
        q->searchBar()->show();
    }

    QueryWindow *q;
    Aki::Irc::User *self;
    Aki::Irc::User *other;
}; // End of class QueryWindowPrivate.
} // End of namespace Aki.

QueryWindow::QueryWindow(const QString &name, Aki::Irc::Socket *socket, QWidget *parent)
    : Aki::BaseWindow(name, Aki::BaseWindow::QueryWindow, parent)
{
    d.reset(new Aki::QueryWindowPrivate(this));

    setupUi(this);
    setSocket(socket);
    setView(chatOutput);
    channelSearch->hide();
    setLogFile(new Aki::LogFile(socket->name(), name, this));
    view()->setLog(logFile());

    QStringList log = logFile()->logLines();
    if (!log.isEmpty()) {
        foreach (const QString &str, log) {
            view()->addLogLine(str);
        }
    }

    logFile()->open();

    connect(chatInput, SIGNAL(textSubmitted()),
            SLOT(textSubmitted()));
    connect(channelSearch, SIGNAL(findNextClicked()),
            SLOT(findNextClicked()));
    connect(channelSearch, SIGNAL(findPreviousClicked()),
            SLOT(findPreviousClicked()));
    connect(channelSearch, SIGNAL(textEdited(QString)),
            SLOT(channelSearchTextEdited(QString)));
    connect(channelSearch->optionButton(), SIGNAL(clicked(bool)),
            SLOT(optionButtonClicked()));
    connect(chatOutput, SIGNAL(findTextTriggered()),
            SLOT(findTextTriggered()));
}

QueryWindow::~QueryWindow()
{
}

void
QueryWindow::setCurrent(bool current)
{
    Aki::BaseWindow::setCurrent(current);
    if (isCurrent()) {
        chatInput->setFocus();
        setTabColor(Default);
    }
}

Aki::Irc::User*
QueryWindow::selfUser()
{
    return d->self;
}

Aki::Irc::User*
QueryWindow::otherUser()
{
    return d->other;
}

void
QueryWindow::setSelfUser(Aki::Irc::User *user)
{
    socket()->rfcWho(user->nick());
    d->self = user;
}

void
QueryWindow::setOtherUser(Aki::Irc::User *user)
{
    socket()->rfcWho(user->nick());
    d->other = user;
}

void
QueryWindow::addWho(const QString &channel, const QString &identName, const QString &address,
                    const QString &server, const QString &nick, const QString &flags,
                    int hops, const QString &realName)
{
    Q_UNUSED(channel);
    Q_UNUSED(server);
    Q_UNUSED(flags);
    Q_UNUSED(hops);

    if (nick == selfUser()->nick()) {
        selfUser()->setUser(identName);
        selfUser()->setNick(nick);
        selfUser()->setHost(address);
        selfUser()->setRealName(realName);
    } else {
        otherUser()->setUser(identName);
        otherUser()->setNick(nick);
        otherUser()->setHost(address);
        otherUser()->setRealName(realName);
        userHostmask->setText(QString("%1 (%2@%3)").arg(realName, identName, address));
    }
}

void
QueryWindow::addMessage(const QString &from, const QString &message)
{
    Aki::ReplaceConfig *replace = new Aki::ReplaceConfig(this);
    QList<Aki::ReplaceConfig::ReplaceItem> items = replace->wordList();
    QString msg = message;

    foreach (Aki::ReplaceConfig::ReplaceItem item, items) {
        if (item.replacementMethod.toLower() == "both" ||
            item.replacementMethod.toLower() == "incoming") {
            if (item.regex) {
                msg.replace(QRegExp(item.text), item.replacementText);
            } else {
                msg.replace(item.text, item.replacementText, Qt::CaseSensitive);
            }
        }
    }
    
    if (from == selfUser()->nick()) {
        QString match = "\\s*" + QRegExp::escape(socket()->currentNick()) + ":*\\s*";
        QString colour = QString("<font color='%1'>%2</font>").arg(selfUser()->color().name(), selfUser()->nick());

        if (message.contains(QRegExp(match))) {
            view()->addPrivmsgHighlight(colour, Aki::Irc::Color::toHtml(message));
            if (!isCurrent()) {
                setTabColor(Highlight);
            }
        } else {
            view()->addPrivmsg(colour, Aki::Irc::Color::toHtml(message));
            if (!isCurrent()) {
                setTabColor(NewMessage);
            }
        }
    } else {
        QString colour = QString("<font color='%1'>%2</font>").arg(otherUser()->color().name(), otherUser()->nick());
        view()->addPrivmsg(colour, Aki::Irc::Color::toHtml(message));
        if (!isCurrent()) {
            setTabColor(NewMessage);
        }
    }
}

void
QueryWindow::addNick(const QString &oldNick, const QString &newNick)
{
    if (oldNick == socket()->currentNick() || newNick == socket()->currentNick()) {
        socket()->appendNick(newNick);
        selfUser()->setNick(newNick);

        if (!Aki::Settings::conferenceMode()) {
            view()->addNick(oldNick, newNick);
        }
        if (!isCurrent()) {
            setTabColor(NewData);
        }
    } else if (oldNick == otherUser()->nick() || newNick == otherUser()->nick()) {
        otherUser()->setNick(newNick);

        if (!Aki::Settings::conferenceMode()) {
            view()->addNick(oldNick, newNick);
        }
    }
}

bool
QueryWindow::hasInputFocus() const
{
    return chatInput->hasFocus();
}

Aki::SearchBar*
QueryWindow::searchBar()
{
    return channelSearch;
}

#include "querywindow.moc"
