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

#include "chatview.h"
#include "logfile.h"
#include "settings.h"
#include <KDateTime>
#include <KDebug>
#include <KEmoticons>
#include <KEmoticonsTheme>
#include <KFileDialog>
#include <KIO/Job>
#include <KGlobal>
#include <KLocale>
#include <KSaveFile>
#include <KToolInvocation>
#include <KUniqueApplication>
#include <KUrl>
#include <QClipboard>
#include <QMenu>
#include <QScrollBar>
#include <QWebElement>
#include <QWebFrame>
#include <QWebHitTestResult>
using namespace Aki;

namespace Aki
{
class ChatViewPrivate
{
public:
    ChatViewPrivate(Aki::ChatView *qq)
        : q(qq),
        logFile(0),
        userMoved(false),
        isMarkedWaiting(false)
    {
        KEmoticons emoticons;
        emoTheme = emoticons.theme();
        QString bodyStyle;
        bodyStyle = "font-family: &quot;Sans Serif&quot;; font-size: 13px; font-weight: 400; font-style: normal";
        QString xhtml;
        xhtml += "<!DOCTYPE html PUBLIC &quot;-//W3C//DTD XHTML 1.0 Strict//EN&quot;";
        xhtml += "&quot;http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd&quot;>";
        xhtml += "<html><head><title>Aki IRC Client</title></head>";
        xhtml += QString("<body style='%1'></body></html>").arg(bodyStyle);
        q->setHtml(QString(xhtml));
    }

    QWebFrame* mainFrame()
    {
        return q->page()->mainFrame();
    }

    void appendMessage(const QString &message, bool useTime = true)
    {
        QWebElement body;
        if (useTime) {
            body = mainFrame()->findFirstElement("body");
            body.appendInside(para(messageTime() + convertEmotes(message)));
        } else {
            body = mainFrame()->findFirstElement("body");
            body.appendInside(para(convertEmotes(message)));
        }

        const int maxVert = mainFrame()->scrollBarMaximum(Qt::Vertical);
        const int min = mainFrame()->scrollBarValue(Qt::Vertical);

        if (maxVert == min) {
            userMoved = false;
        } else {
            userMoved = true;
        }

        QWebElement bodyP = mainFrame()->findFirstElement("body p");
        const int count = mainFrame()->findAllElements("p").count();
        if (count > Aki::Settings::scrollbackLines()) {
            bodyP.takeFromDocument();
        }
    }

    QString convertEmotes(const QString &str) const
    {
        if (Aki::Settings::enableEmotions()) {
            return parseEmoticons(str, KEmoticonsTheme::StrictParse);
        } else {
            return str;
        }
    }

    QString messageTime() const
    {
        QTime time = KDateTime::currentLocalTime();
        QString timeString = KGlobal::locale()->formatLocaleTime(time);
        return span("[" + timeString + "] ");
    }

    void toLog(const QString &log)
    {
        if (logFile) {
            logFile->appendLine(log);
        }
    }

    QString stripHtml(const QString &message) const
    {
        QString backup = message;
        backup.replace(QRegExp("<[^>]*>"), "");
        return backup;
    }

    QString span(const QString &message) const
    {
        return "<span>" + message + "</span>";
    }

    QString span(const QString &message, const QColor &color) const
    {
        return QString("<span style='color: %1;'>%2</span>")
                    .arg(color.name(), message);
    }

    QString para(const QString &message) const
    {
        QString style;
        style += "margin-top: 0px; ";
        style += "margin-bottom: 0px; ";
        style += "margin-left: 0px; ";
        style += "margin-right: 0px; ";
        style += "text-indent: 0px;";
        return QString("<p style='%1'>%2</p>").arg(style, message);
    }

    QString url(const QString &u) const
    {
        return url(u, u);
    }

    QString url(const QString &name, const QString &u) const
    {
        return QString("<a href='%1'>%2</a>").arg(name, u);
    }

    QString userA(const QString &username) const
    {
        return QString("<a href='#%1' style='text-decoration: none;'>%2</a>")
            .arg(stripHtml(username), username);
    }

    QString strong(const QString &message) const
    {
        return "<strong>" + message + "</strong>";
    }

    void contentsSizeChanged(const QSize &size)
    {
        if (!userMoved) {
            mainFrame()->scroll(size.width(), size.height());
        }
    }

    QString parseEmoticons(const QString &message, KEmoticonsTheme::ParseMode mode,
                           const QStringList &excludes = QStringList()) const
    {
        // Exact code from parseEmoticons in parseEmoticons()
        // Just added function to add file:// to the path because WebKit won't
        // load them without it
        QList<KEmoticonsTheme::Token> tokens =
            emoTheme.tokenize(message, mode | KEmoticonsTheme::SkipHTML);
        if (tokens.isEmpty() && !message.isEmpty()) {
            return message;
        }

        QString result;

        foreach (const KEmoticonsTheme::Token & token, tokens) {
            switch (token.type) {
            case KEmoticonsTheme::Text: {
                result += token.text;
                break;
            }
            case KEmoticonsTheme::Image: {
                if (!excludes.contains(token.text)) {
                    result += token.picHTMLCode;
                } else {
                    result += token.text;
                }
                break;
            }
            default: {
                kWarning() << "Unknown token type. Something's broken.";
                break;
            }
            }
        }
        result.replace("src=\"", "src=\"file://");
        return result;
    }

    void linkClicked(const QUrl &url)
    {
        KUrl newUrl(url);
        KToolInvocation::invokeBrowser(newUrl.url());
    }

    void customContextMenuRequested(const QPoint &pos)
    {
        QWebHitTestResult hit = mainFrame()->hitTestContent(pos);
        if (hit.isNull()) {
            return;
        }

        if (hit.linkElement().tagName() == "A" && hit.linkElement().hasAttribute("href")) {
            KUrl url(hit.linkUrl());
            QString href = url.url().remove("about:blank");

            if (href[0] == '#') {
                emit q->userUrlClicked(href.remove('#'));
            } else {
                QMenu *menu = new QMenu(q);

                QAction *copyUrlAction = new QAction(menu);
                copyUrlAction->setText(i18n("Copy Link Address"));
                copyUrlAction->setIcon(KIcon("edit-copy"));
                q->connect(copyUrlAction, SIGNAL(triggered(bool)),
                           SLOT(copyUrlTriggered()));

                QAction *saveAsAction = new QAction(menu);
                saveAsAction->setText(i18n("Save Link As..."));
                saveAsAction->setIcon(KIcon("document-save-as"));
                q->connect(saveAsAction, SIGNAL(triggered(bool)),
                           SLOT(saveAsTriggered()));

                menu->setTitle(i18n("Normal"));
                menu->addAction(copyUrlAction);
                menu->addAction(saveAsAction);
                menu->exec(QCursor::pos());
            }
        } else {
            QMenu *menu = new QMenu(q);

            QAction *copyAction = new QAction(menu);
            copyAction->setText(i18n("Copy"));
            copyAction->setIcon(KIcon("edit-copy"));
            q->connect(copyAction, SIGNAL(triggered(bool)),
                       SLOT(copyTriggered()));

            QAction *findTextAction = new QAction(menu);
            findTextAction->setText(i18n("Find Text..."));
            findTextAction->setIcon(KIcon("edit-find"));
            q->connect(findTextAction, SIGNAL(triggered(bool)),
                       SLOT(findTextTriggered()));

            menu->addAction(copyAction);
            menu->addAction(findTextAction);
            menu->exec(QCursor::pos());
        }
    }

    void copyUrlTriggered()
    {
        QClipboard *cb = KUniqueApplication::clipboard();
        cb->setText(q->page()->selectedText(), QClipboard::Clipboard);
    }

    void saveAsTriggered()
    {
        KUser user(KUser::UseEffectiveUID);
        const QString fileName =
            KFileDialog::getSaveFileName(user.homeDir(), QString(), q, i18n("Save As..."),
                                         KFileDialog::ConfirmOverwrite);

        KIO::file_copy(q->page()->selectedText(), fileName);
    }

    void copyTriggered()
    {
        QClipboard *cb = KUniqueApplication::clipboard();
        cb->setText(q->page()->selectedText(), QClipboard::Clipboard);
    }

    void findTextTriggered()
    {
        emit q->findTextTriggered();
    }

    Aki::ChatView *q;
    Aki::LogFile *logFile;
    KEmoticonsTheme emoTheme;
    bool userMoved;
    bool isMarkedWaiting;
}; // End of class ChatViewPrivate.
} // End of namespace Aki.

ChatView::ChatView(QWidget *parent)
    : QWebView(parent)
{
    d.reset(new Aki::ChatViewPrivate(this));
    setFocusPolicy(Qt::NoFocus);
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customContextMenuRequested(QPoint)));
    connect(d->mainFrame(), SIGNAL(contentsSizeChanged(QSize)),
            SLOT(contentsSizeChanged(QSize)));
    connect(page(), SIGNAL(linkClicked(QUrl)),
            SLOT(linkClicked(QUrl)));
}

ChatView::~ChatView()
{
}

void
ChatView::addAway(const QString &message)
{
    QString msg = i18nc("The away message", "[Away] %1", message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addAway(const QString &nick, const QString &message)
{
    QString msg = i18nc("Nick, followed by away message", "[Away] %1 (%2)", nick, message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addBan(const QString &nick, const QString &mask)
{
    QString msg = i18n("*** %1 set ban on %2", nick, mask);
    QString span = d->span(msg, Aki::Settings::banColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addBanList(const QString &channel, const QString &mask, const QString &who,
                     const QString &time)
{
    Q_UNUSED(channel);
    Q_UNUSED(mask);
    Q_UNUSED(who);
    Q_UNUSED(time);
    /*QString msg = i18nc("Channel, followed by ban mask, nick and nick",
                        "[BanList: %1] %2 set by %3 on %4",
                        channel, mask, who, time);*/
}

void
ChatView::addChannelCreated(const QString &time)
{
    QString msg = i18n("*** Channel was created on %1", time);
    QString span = d->span(msg, Aki::Settings::topicColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addChannelModes(const QString &modes)
{
    QString msg = i18n("*** Channel Modes: %1", modes);
    QString span = d->span(msg, Aki::Settings::topicColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addChannelModes(const QString &from, const QString &modes)
{
    QString msg = i18n("*** %1 sets the channel's mode to %2", from, modes);
    QString span = d->span(msg, Aki::Settings::topicColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addChannelUrl(const QString &url)
{
    QString msg = i18n("*** Channel URL: %1", d->url(url));
    QString span = d->span(msg, Aki::Settings::topicColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addCtcp(const QString &from, const QString &message)
{
    QString msg = QString("[Ctcp:%1] %2").arg(from, message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addCtcpAction(const QString &from, const QString &message)
{
    QString msg = QString("*** %1 %2").arg(from, message);
    QString span = d->span(msg, Aki::Settings::actionColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addCtcpRequest(const QString &from, const QString &type)
{
    QString msg = i18n("[Ctcp] Received CTCP-%1 from %2", type, from);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addError(const QString &message)
{
    QString msg = i18n("[Error] %1", message);
    QString span = d->span(msg, Aki::Settings::errorColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addError(const QString &type, const QString &message)
{
    QString msg = i18n("[Error] %1: %2", type, message);
    QString span = d->span(msg, Aki::Settings::errorColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addHelp(const QString &message)
{
    QString msg = i18n("[Help] %1", message);
    QString span = d->span(msg);
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addInvite(const QString &nick, const QString &channel)
{
    QString msg = i18n("[Invite] You've' invited %1 to channel %2", nick, channel);
    QString span = d->span(msg, Aki::Settings::inviteColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addIsOn(const QStringList &nicks)
{
    QStringListIterator nickIter(nicks);
    while (nickIter.hasNext()) {
        QString msg = i18n("[IsOn] %1 is online", nickIter.next());
        QString span = d->span(msg, Aki::Settings::noticeColor());
        d->toLog(msg);
        d->appendMessage(span);
    }
}

void
ChatView::addLUser(const QString &message)
{
    QString msg = i18n("[LUser] %1", message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addMessage(const QString &message)
{
    QString msg = QString("*** %1").arg(message);
    QString span = d->span(msg, Aki::Settings::actionColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addChannelMode(const QString &fromNick, const QChar &mode, const QString &params,
                         bool self)
{
    QString msg;
    QString span;

    if (mode == 'i') {
        if (self) {
            msg = i18n("*** You set the channel to invite only");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 set the channel to invite only", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 'm') {
        if (self) {
            msg = i18n("*** You set the channel to moderated");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 set the channel to moderated", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 'p') {
        if (self) {
            msg = i18n("*** You set the channel to private");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 set the channel to private", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 's') {
        if (self) {
            msg = i18n("*** You set the channel to secret");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 set the channel to secret", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 't') {
        if (self) {
            msg = i18n("*** You turned on topic protection");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 turned on topic protection", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 'n') {
        if (self) {
            msg = i18n("*** You set the channel to no outside messages");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 set the channel to no outside messages", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 'k') {
        if (self) {
            msg = i18n("*** You set the channel keyword to '%1'", params);
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 set the channel keyword to '%2'", fromNick, params);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 'l') {
        if (self) {
            msg = i18n("*** You set the channel's user limit to %1", params);
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 set the channel's user limit to %2", fromNick, params);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else {
        if (self) {
            msg = i18n("*** You set the channel's mode to +%1", mode);
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 set the channel's mode to +%2", fromNick, mode);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    }
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addMode(const QString &fromNick, const QString &toNick, const QChar &mode,
                  bool toYou, bool fromYou)
{
    QString msg;
    QString span;
    if (mode == 'o') {
        if (toYou && fromYou) {
            msg = i18n("*** You gave yourself operator status");
            span = d->span(msg, Aki::Settings::opColor());
        } else if (fromYou && !toYou) {
            msg = i18n("*** You gave operator status to %1", toNick);
            span = d->span(msg, Aki::Settings::opColor());
        } else if (toYou && !fromYou) {
            msg = i18n("*** %1 gave operator status to you", fromNick);
            span = d->span(msg, Aki::Settings::opColor());
        } else if (!toYou && !fromYou) {
            msg = i18n("*** %1 gave operator status to %2", fromNick, toNick);
            span = d->span(msg, Aki::Settings::opColor());
        }
    } else if (mode == 'h') {
        if (toYou && fromYou) {
            msg = i18n("*** You gave yourself half operator status");
            span = d->span(msg, Aki::Settings::opColor());
        } else if (fromYou && !toYou) {
            msg = i18n("*** You gave half operator status to %1", toNick);
            span = d->span(msg, Aki::Settings::opColor());
        } else if (toYou && !fromYou) {
            msg = i18n("*** %1 gave half operator status to you", fromNick);
            span = d->span(msg, Aki::Settings::opColor());
        } else if (!toYou && !fromYou) {
            msg = i18n("*** %1 gave half operator status to %2", fromNick, toNick);
            span = d->span(msg, Aki::Settings::opColor());
        }
    } else if (mode == 'v') {
        if (toYou && fromYou) {
            msg = i18n("*** You gave yourself permission to talk");
            span = d->span(msg, Aki::Settings::voiceColor());
        } else if (fromYou && !toYou) {
            msg = i18n("*** You gave %1 permission to talk", toNick);
            span = d->span(msg, Aki::Settings::voiceColor());
        } else if (toYou && !fromYou) {
            msg = i18n("*** %1 gave you permission to talk", fromNick);
            span = d->span(msg, Aki::Settings::voiceColor());
        } else if (!toYou && !fromYou) {
            msg = i18n("*** %1 gave %2 permission to talk", fromNick, toNick);
            span = d->span(msg, Aki::Settings::voiceColor());
        }
    } else {
        return;
    }

    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addMotd(const QString &message)
{
    QString msg = i18n("[Motd] %1", message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addNick(const QString &oldNick, const QString &newNick)
{
    QString msg = i18n("*** %1 is now known as %2", oldNick, newNick);
    QString span = d->span(msg, Aki::Settings::nickColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addNotice(const QString &message)
{
    QString msg = i18n("[Notice] %1", message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addNotice(const QString &from, const QString &message)
{
    QString msg = i18n("[Notice] %1: %2", d->strong(from), message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(d->stripHtml(msg));
    d->appendMessage(span);
}

void
ChatView::addNoticeAuth(const QString &message)
{
    QString msg = i18n("[Notice] %1", message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addPrivmsg(const QString &from, const QString &message)
{
    QString msg = i18nc("Nick, followed by a message", "&lt;%1&gt; %2",
                        d->userA(from), message);
    QString span = d->span(msg);
    d->toLog(d->stripHtml(msg));
    d->appendMessage(span);
}

void
ChatView::addPrivmsgHighlight(const QString &from, const QString &message)
{
    QString msg = i18nc("Nick name, the colour and the message",
                        "&lt;%1&gt; %2", d->userA(from), d->span(message, Aki::Settings::highlightColor()));
    QString span = d->span(msg);
    d->toLog(d->stripHtml(msg));
    d->appendMessage(span);
}

void
ChatView::addRemoveBan(const QString &nick, const QString &mask)
{
    QString msg = i18n("*** %1 removed ban on %2", nick, mask);
    QString span = d->span(msg, Aki::Settings::unbanColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addRemoveMode(const QString &fromNick, const QChar &mode, bool self)
{
    QString msg;
    QString span;
    if (mode == 'i') {
        if (self) {
            msg = i18n("*** You removed the channel's invite only mode");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 removed the channel's invite only mode", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 'm') {
        if (self) {
            msg = i18n("*** You removed the channel's moderation");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 removed the channel's moderation", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 'p') {
        if (self) {
            msg = i18n("*** You set the channel to public");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 set the channel to public", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 's') {
        if (self) {
            msg = i18n("*** You set the channel to visible");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 set the channel to visible", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 't') {
        if (self) {
            msg = i18n("*** You removed the channel's topic protection");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 removed the channel's topic protection", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 'n') {
        if (self) {
            msg = i18n("*** You set the channel to allow outside messages");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 set the channel to allow outside messages", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 'k') {
        if (self) {
            msg = i18n("*** You removed the channel's keyword");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 removed the channel's keyword", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    } else if (mode == 'l') {
        if (self) {
            msg = i18n("*** You removed the channel's user limit");
            span = d->span(msg, Aki::Settings::topicColor());
        } else {
            msg = i18n("*** %1 removed the channel's user limit", fromNick);
            span = d->span(msg, Aki::Settings::topicColor());
        }
    }

    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addRemoveMode(const QString &fromNick, const QString &toNick, const QChar &mode,
                        bool toYou, bool fromYou)
{
    QString msg;
    QString span;
    if (mode == QChar('o')) {
        if (toYou && fromYou) {
            msg = i18n("*** You took operator status from yourself");
            span = d->span(msg, Aki::Settings::deOpColor());
        } else if (fromYou && !toYou) {
            msg = i18n("*** You took operator status from %1", toNick);
            span = d->span(msg, Aki::Settings::deOpColor());
        } else if (toYou && !fromYou) {
            msg = i18n("*** %1 took operator status from you", fromNick);
            span = d->span(msg, Aki::Settings::deOpColor());
        } else if (!toYou && !fromYou) {
            msg = i18n("*** %1 took operator status from %2", fromNick, toNick);
            span = d->span(msg, Aki::Settings::deOpColor());
        }
    } else if (mode == QChar('h')) {
        if (toYou && fromYou) {
            msg = i18n("*** You took half operator status from yourself");
            span = d->span(msg, Aki::Settings::deOpColor());
        } else if (fromYou && !toYou) {
            msg = i18n("*** You took half operator status from %1", toNick);
            span = d->span(msg, Aki::Settings::deOpColor());
        } else if (toYou && !fromYou) {
            msg = i18n("*** %1 took half operator status from you", fromNick);
            span = d->span(msg, Aki::Settings::deOpColor());
        } else if (!toYou && !fromYou) {
            msg = i18n("*** %1 took half operator status from %2", fromNick, toNick);
            span = d->span(msg, Aki::Settings::deOpColor());
        }
    } else if (mode == QChar('v')) {
        if (toYou && fromYou) {
            msg = i18n("*** You took permission to talk from yourself");
            span = d->span(msg, Aki::Settings::deVoiceColor());
        } else if (fromYou && !toYou) {
            msg = i18n("*** You took %1's permission to talk", toNick);
            span = d->span(msg, Aki::Settings::deVoiceColor());
        } else if (toYou && !fromYou) {
            msg = i18n("*** %1 took your permission to talk", fromNick);
            span = d->span(msg, Aki::Settings::deVoiceColor());
        } else if (!toYou && !fromYou) {
            msg = i18n("*** %1 took %2's permission to talk", fromNick, toNick);
            span = d->span(msg, Aki::Settings::deVoiceColor());
        }
    } else {
        return;
    }

    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addSelfUModeSet(const QString &modes)
{
    QString msg = i18n("[UMode] You changed your modes to %1", modes);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addSupport(const QString &message)
{
    QString msg = i18n("[Support] %1", message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addTime(const QString &server, const QString &time)
{
    QString msg = i18n("[Time] %2 %3", server, time);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addTopic(const QString &topic)
{
    QString msg = i18n("*** The channel's topic is \"%1\"", topic);
    QString span = d->span(msg, Aki::Settings::topicColor());
    d->toLog(d->stripHtml(msg));
    d->appendMessage(span);
}

void
ChatView::addTopicChanged(const QString &nick, const QString &topic)
{
    QString msg = i18n("*** %1 has changed the channel's topic to \"%2\"", nick, topic);
    QString span = d->span(msg, Aki::Settings::topicColor());
    d->toLog(d->stripHtml(msg));
    d->appendMessage(span);
}

void
ChatView::addTopicSetBy(const QString &nick, const QString &time)
{
    QString msg = i18n("*** Topic set by %1 on %2", nick, time);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addUMode(const QString &modes)
{
    QString msg = i18n("[UMode] Your modes are: %1", modes);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addUMode(const QString &nick, const QString &modes)
{
    QString msg = i18n("[UMode] %1 set your modes to %2", nick, modes);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addUserHost(const QStringList &nicks, const QStringList &hosts)
{
    QStringListIterator nickIter(nicks);
    QStringListIterator hostIter(hosts);

    while (nickIter.hasNext()) {
        while (hostIter.hasNext()) {
            QString msg = i18n("[UserHost] %1 is %2", nickIter.next(), hostIter.next());
            QString span = d->span(msg, Aki::Settings::noticeColor());
            d->toLog(msg);
            d->appendMessage(span);
        }
    }
}

void
ChatView::addUserJoin(const QString &channel, const QString &nick,
                      const QString &hostMask)
{
    QString msg = i18n("--&gt; %1 (%2) has joined %3", nick, hostMask, channel);
    QString span = d->span(msg, Aki::Settings::joinColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addUserPart(const QString &channel, const QString &nick, const QString &hostMask,
                      const QString &message)
{
    QString tmp = message.isEmpty() ? hostMask : message;
    QString msg = i18nc("Nickname (message)", "&lt-- %1 (%2) has left %3", nick, tmp, channel);
    QString span = d->span(msg, Aki::Settings::partColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addUserQuit(const QString &nick, const QString &hostMask, const QString &message)
{
    QString tmp = message.isEmpty() ? hostMask : message;
    QString msg = i18nc("Nickname (message)", "&lt-- %1 (%2) has quit", nick, tmp);
    QString span = d->span(msg, Aki::Settings::quitColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addVersion(const QString &message)
{
    QString msg = i18n("[Version] %1", message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addWelcome(const QString &message)
{
    QString msg = i18n("[Welcome] %1", message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addWho(const QString &channel, const QString &message)
{
    QString msg = i18n("[Who:%1] %2", channel, message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addWho(const QString &channel, const QString &userName, const QString &address,
                 const QString &server, const QString &nick, const QString &flags, int hops,
                 const QString &realName)
{
    QString msg = i18n("[Who:%1] <strong><u>Username</u></strong>: %2, <strong><u>Address</u></strong>: %3, "
                       "<strong><u>Server</u></strong>: %4, <strong><u>Nick</u></strong>: %5, <strong><u>Flags</u></strong>: %6, "
                       "<strong><u>Hops</u></strong>: %7, <strong><u>Real Name</u></strong>: %8",
                       Aki::Settings::noticeColor().name(), channel, userName, address, server, nick,
                       flags, hops, realName);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addWhoIs(const QString &nick, const QString &info)
{
    QString msg = i18n("[WhoIs:%1] %2", nick, info);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addWhoIsChannels(const QString &nick, const QString &channels)
{
    QString msg = i18n("[WhoIs:%1] is currently on channels: %2", nick, channels);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addWhoIsIdentified(const QString &nick, const QString &info)
{
    QString msg = i18nc("Nick, info", "[WhoIs:%1] %2", nick, info);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addWhoIsIdle(const QString &nick, const QString &idleTime, const QString &signOn)
{
    QString msg = i18n("[WhoIs:%1] idle: %2, signon: %2", nick, idleTime, signOn);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addWhoIsServer(const QString &nick, const QString &server, const QString &info)
{
    QString msg = i18n("[WhoIs:%1] is online via %2 (%3)", nick, server, info);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addWhoIsUser(const QString &nick, const QString &username, const QString &address,
                         const QString &info)
{
    QString msg = i18n("[WhoIs:%1] is %2@%3 (%4)",
                       nick, username, address, info);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addWhoWas(const QString &nick, const QString &message)
{
    QString msg = i18n("[WhoWas:%1] %2", nick, message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addWhoWasUser(const QString &nick, const QString &username, const QString &address,
                        const QString &info)
{
    QString msg = i18n("[WhoWas:%1] is %2@%3 (%4)",
                       nick, username, address, info);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::add901(const QString &id, const QString &identName, const QString &address,
                 const QString &message)
{
    QString msg = QString("[901] %1 %2@%3 %4").arg(id, identName, address, message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(d->stripHtml(msg));
    d->appendMessage(span);
}

void
ChatView::setLog(Aki::LogFile *logFile)
{
    d->logFile = logFile;
}

void
ChatView::addLogLine(const QString &line)
{
    QString msg = d->span(line, QColor("#C3C3C3"));
    d->appendMessage(msg, false);
}

void
ChatView::clear()
{
    QWebElement body = d->mainFrame()->findFirstElement("body");
    QWebElementCollection paragraphs = body.findAll("p");
    QWebElementCollection markers = body.findAll("hr");

    foreach (QWebElement p, paragraphs) {
        p.takeFromDocument();
    }
    foreach (QWebElement hr, markers) {
        hr.takeFromDocument();
    }
}

void
ChatView::insertMarker()
{
    QWebElement body = d->mainFrame()->findFirstElement("body");
    QWebElement marker = body.findFirst("hr");
    if (!marker.isNull()) {
        marker.takeFromDocument();
    }
    body.appendInside("<hr style='background-color: red;' />");
}

void
ChatView::clearMarker()
{
    QWebElement body = d->mainFrame()->findFirstElement("body");
    QWebElement marker = body.findFirst("hr");
    if (!marker.isNull()) {
        marker.takeFromDocument();
    }
}

void
ChatView::addKick(const QString &from, const QString &channel, const QString &nick,
                  const QString &message, bool toYou, bool fromYou)
{
    QString msg;
    if (toYou && fromYou) {
        if (message.isEmpty()) {
            msg = i18nc("channel", "*** You have kicked yourself from %1", channel);
        } else {
            msg = i18nc("channel (kick message)","*** You have kicked yourself from %1 (%2)",
                        channel, message);
        }
    } else if (toYou && !fromYou) {
        if (message.isEmpty()) {
            msg = i18nc("channel, nickname", "*** You have been kicked from %1 by %2", channel, from);
        } else {
            msg = i18nc("channel, nickname (kick message)", "*** You have been kicked from %1 by %2 (%3)",
                        channel, from, message);
        }
    } else if (!toYou && fromYou) {
        if (message.isEmpty()) {
            msg = i18nc("nickname, channel", "*** You have kicked %1 from %2", nick, channel);
        } else {
            msg = i18nc("nickname, channel (kick message)", "*** You have kicked %1 from %2 (%3)",
                        nick, channel, message);
        }
    } else if (!toYou && !fromYou) {
        if (message.isEmpty()) {
            msg = i18nc("nickname, nickname, channel", "*** %1 has kicked %2 from %3", from, nick, channel);
        } else {
            msg = i18nc("nickname, nickname, channel (kick message)", "*** %1 has kicked %2 from %3 (%4)",
                        from, nick, channel, message);
        }
    }
    QString span = d->span(msg, Aki::Settings::kickColor());
    d->toLog(msg);
    d->appendMessage(span);
}

#include "chatview.moc"
