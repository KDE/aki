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
#include <KEmoticons>
#include <KEmoticonsTheme>
#include <KGlobal>
#include <KLocale>
#include <QScrollBar>
#include <QWebElement>
#include <QWebFrame>
using namespace Aki;

namespace Aki
{
class ChatViewPrivate
{
public:
    ChatViewPrivate(Aki::ChatView *qq)
        : q(qq)
    {
        KEmoticons emoticons;
        emoTheme = emoticons.theme();
    }

    QWebFrame* mainFrame()
    {
        return q->page()->mainFrame();
    }

    void appendMessage(const QString &message, bool useTime = true)
    {
        if (useTime) {
            QWebElement body = mainFrame()->findFirstElement("body");
            body.appendInside(para(messageTime() + convertEmotes(message)));
            mainFrame()->documentElement().replace(body);
        } else {
            QWebElement body = mainFrame()->findFirstElement("body");
            body.appendInside(para(convertEmotes(message)));
            mainFrame()->documentElement().replace(body);
        }

        const int maxVert = mainFrame()->scrollBarMaximum(Qt::Vertical);
        const int value = mainFrame()->scrollBarValue(Qt::Vertical);

        if (value == maxVert) {
            mainFrame()->setScrollBarValue(Qt::Vertical, Aki::Settings::scrollbackLines());
        } else {
            mainFrame()->setScrollBarValue(Qt::Vertical, Aki::Settings::scrollbackLines() + 1);
        }
    }

    QString convertEmotes(const QString &str) const
    {
        return emoTheme.parseEmoticons(str, KEmoticonsTheme::StrictParse);
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
        return "<p>" + message + "</p>";
    }

    QString url(const QString &u) const
    {
        return url(u, u);
    }

    QString url(const QString &name, const QString &u) const
    {
        return QString("<a href='%1'>%2</a>").arg(name, u);
    }

    QString strong(const QString &message) const
    {
        return "<strong>" + message + "</strong>";
    }

    Aki::ChatView *q;
    Aki::LogFile *logFile;
    KEmoticonsTheme emoTheme;
}; // End of class ChatViewPrivate.
} // End of namespace Aki.

ChatView::ChatView(QWidget *parent)
    : QWebView(parent)
{
    d.reset(new Aki::ChatViewPrivate(this));
    setFocusPolicy(Qt::NoFocus);
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
ChatView::addChannelModes(const QString &from, const QString &modes)
{
    QString msg = i18n("*** %1 sets the channel modes to %2", from, modes);
    QString span = d->span(msg, Aki::Settings::topicColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addChannelUrl(const QString &url)
{
    QString msg = i18n("*** Channel url: ", d->url(url));
    QString span = d->span(msg, Aki::Settings::topicColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addCtcp(const QString &from, const QString &message)
{
    Q_UNUSED(from);
    QString msg = QString("[Ctcp] %1").arg(message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addCtcpRequest(const QString &from, const QString &type)
{
    QString msg = i18n("[Ctcp] Received CTCP-%1 from %2").arg(type, from);
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
    QString msg = i18n("[Invite] You have invited %1 to channel %2", nick, channel);
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
    }

    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addMotd(const QString &message)
{
    QString msg = i18n("[Motd] %1").arg(message);
    QString span = d->span(msg, Aki::Settings::noticeColor());
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addNick(const QString &oldNick, const QString &newNick)
{
    QString msg = i18n("*** %1 is now known as as %2", oldNick, newNick);
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
    QString msg = i18n("[Notice] %1: %2", from, d->strong(message));
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
                        from, message);
    QString span = d->span(msg);
    d->toLog(msg);
    d->appendMessage(span);
}

void
ChatView::addPrivmsgHighlight(const QString &from, const QString &message)
{
    QString msg = i18nc("Nick name, the colour and the message",
                        "&lt;%1&gt; %2", from, d->span(message, Aki::Settings::highlightColor()));
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
