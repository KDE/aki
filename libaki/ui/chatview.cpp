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
#include "settings.h"
#include "logfile.h"
#include <KDateTime>
#include <KEmoticons>
#include <KEmoticonsTheme>
#include <KLocale>
#include <QScrollBar>
using namespace Aki;

namespace Aki
{
class ChatViewPrivate
{
public:
    ChatViewPrivate(ChatView *qq)
        : q(qq),
        logFile(0)
    {
        KEmoticons emoticons;
        emoTheme = emoticons.theme();
    }

    void appendMessage(const QString &message, bool useTime = true)
    {
        if (useTime) {
            q->append(messageTime() + emoTheme.parseEmoticons(message, KEmoticonsTheme::StrictParse));
        } else {
            q->append(emoTheme.parseEmoticons(message, KEmoticonsTheme::StrictParse));
        }

        if (q->verticalScrollBar()->value() == q->verticalScrollBar()->maximum()) {
            q->document()->setMaximumBlockCount(Aki::Settings::scrollbackLines());
        } else {
            q->document()->setMaximumBlockCount(q->document()->maximumBlockCount() + 1);
        }
    }

    void toLog(const QString &log)
    {
        if (logFile) {
            logFile->appendLine(log);
        }
    }

    QString messageTime() const
    {
        KDateTime time = KDateTime::currentLocalDateTime();
        time = time.toClockTime();

        QString timeString = time.toString("%H:%M:%S");
        return QString("<span>[%1] </span>").arg(timeString);
    }

    QString stripHtml(const QString &message) const
    {
        QString backup = message;
        backup.replace(QRegExp("<[^>]*>"), "");
        return backup;
    }

    ChatView *q;
    Aki::LogFile *logFile;
    KEmoticonsTheme emoTheme;
}; // End of class ChatViewPrivate.
} // End of namespace Aki.

ChatView::ChatView(QWidget *parent)
    : KTextBrowser(parent, true),
    d(new ChatViewPrivate(this))
{
    setFocusPolicy(Qt::NoFocus);
}

ChatView::~ChatView()
{
    delete d;
}

void
ChatView::addAway(const QString &message)
{
    QString colour = i18n("<span style='color: %1;'>[Away] %2</span>",
                          Aki::Settings::noticeColor().name(), message);
    d->toLog(i18n("[Away] %1", message));
    d->appendMessage(colour);
}

void
ChatView::addAway(const QString &nick, const QString &message)
{
    QString colour = i18n("<span style='color :%1;'>[Away] %2 (%3)</span>",
                          Aki::Settings::noticeColor().name(), nick, message);
    d->toLog(i18n("[Away] %1 (%2)", d->stripHtml(nick), message));
    d->appendMessage(colour);
}

void
ChatView::addBan(const QString &nick, const QString &mask)
{
    QString colour = i18n("<span style='color :%1;'>*** %2 set ban on %3</span>",
                          Aki::Settings::banColor().name(), nick, mask);
    d->toLog(i18n("*** %1 set ban on %2", d->stripHtml(nick), mask));
    d->appendMessage(colour);
}

void
ChatView::addBanList(const QString &channel, const QString &mask, const QString &who,
                     const QString &time)
{
    QString colour = i18n("<span style='color: %1;'>[BanList:%2] %3 set by %4 on %5</span>",
                          Aki::Settings::noticeColor().name(), channel, mask, who, time);
    d->appendMessage(colour);
}

void
ChatView::addChannelCreated(const QString &time)
{
    QString colour = i18n("<span style='color: %1;'>*** Channel was created on %2</span>",
                          Aki::Settings::topicColor().name(), time);
    d->toLog(i18n("*** Channel was created on %1", time));
    d->appendMessage(colour);
}

void
ChatView::addChannelModes(const QString &modes)
{
    QString colour = i18n("<span style='color: %1;'>*** Channel Modes: %2</span>",
                          Aki::Settings::topicColor().name(), modes);
    d->toLog(i18n("*** Channel Modes: %1", modes));
    d->appendMessage(colour);
}

void
ChatView::addChannelModes(const QString &from, const QString &modes)
{
    QString colour = i18n("<span style='color: %1;'>*** %2 sets the channel modes to %3</span>",
                          Aki::Settings::topicColor().name(), from, modes);
    d->toLog(i18n("*** %1 sets the channel modes to %2", d->stripHtml(from), modes));
    d->appendMessage(colour);
}

void
ChatView::addChannelUrl(const QString &url)
{
    QString colour = i18n("<span style='color: %1;'>*** Channel Url: "
                          "<a href='%2'>%2</a></span>", Aki::Settings::topicColor().name(), url);
    d->toLog(i18n("*** Channel Url: %1", url));
    d->appendMessage(colour);
}

void
ChatView::addCtcp(const QString &from, const QString &message)
{
    Q_UNUSED(from);
    d->toLog(i18n("[Ctcp] %1", message));
    d->appendMessage(QString("<span style='color: %1;'>[Ctcp] %2</span>")
        .arg(Aki::Settings::noticeColor().name(), message));
}

void
ChatView::addCtcpAction(const QString &from, const QString &message)
{
    d->toLog(i18n("*** %1 %2", d->stripHtml(from), message));
    d->appendMessage(QString("<span style=\"color: %1;\">*** %2 %3</span>")
        .arg(Aki::Settings::actionColor().name(), from, message));
}

void
ChatView::addCtcpRequest(const QString &from, const QString &type)
{
    QString colour = i18n("<span style='color: %1;'>[Ctcp] Received CTCP-%2 from %3</span>",
                          Aki::Settings::noticeColor().name(), type, from);
    d->toLog(i18n("Received CTCP-%1 from %2", type, d->stripHtml(from)));
    d->appendMessage(colour);
}

void
ChatView::addError(const QString &errorMessage)
{
    QString colour = i18n("<span style='color: %1;'>[Error] %2</span>",
                          Aki::Settings::errorColor().name(), errorMessage);
    d->toLog(i18n("[Error] %1", errorMessage));
    d->appendMessage(colour);
}

void
ChatView::addError(const QString &channel, const QString &errorMessage)
{
    QString colour = i18n("<span style='color: %1;'>[Error] %2: %3</span>",
                          Aki::Settings::errorColor().name(), channel, errorMessage);
    d->toLog(i18n("[Error] %1: %2", channel, errorMessage));
    d->appendMessage(colour);
}

void
ChatView::addHelp(const QString &message)
{
    d->toLog(i18n("[Help] %1", message));
    d->appendMessage(QString("<span>[Help] %1</span>").arg(message));
}

void
ChatView::addInvite(const QString &nick, const QString &channel)
{
    QString colour = i18n("<span style='color: %1;'>[Invite] You have invited %2 to"
                          " channel %3</span>", Aki::Settings::inviteColor().name(),
                          nick, channel);
    d->toLog(i18n("[Invite] You have invited %1 to channel %2", d->stripHtml(nick), channel));
    d->appendMessage(colour);
}

void
ChatView::addIsOn(const QStringList &nicks)
{
    foreach (const QString &nick, nicks) {
        QString colour = i18n("<span style='color: %1;'>[IsOn] %2 is online</span>",
                              Aki::Settings::noticeColor().name(), nick);
        d->toLog(i18n("[IsOn] %1 is online", d->stripHtml(nick)));
        d->appendMessage(colour);
    }
}

void
ChatView::addLUser(const QString &message)
{
    QString colour = i18n("<span style='color: %1;'>[LUser] %2</span>",
                          Aki::Settings::noticeColor().name(), message);
    d->toLog(i18n("[LUser] %1", message));
    d->appendMessage(colour);
}

void
ChatView::addMessage(const QString &message)
{
    QString colour = QString("<span style='color: %1;'>*** %2</span>")
                             .arg(Aki::Settings::actionColor().name(), message);
    d->toLog(i18n("*** %1", message));
    d->appendMessage(colour);
}

void
ChatView::addMode(const QString &fromNick, const QString &toNick, const QChar &mode,
                  bool toYou, bool fromYou)
{
    QString colour;
    if (mode == QChar('o')) {
        if (toYou && fromYou) {
            colour = i18n("<span style='color: %1;'>*** You gave yourself operator status</span>",
                          Aki::Settings::opColor().name());
            d->toLog(i18n("*** You gave yourself operator status"));
        } else if (fromYou && !toYou) {
            colour = i18n("<span style='color: %1;'>*** You gave operator status to %2</span>",
                          Aki::Settings::opColor().name(), toNick);
            d->toLog(i18n("*** You gave operator status to %1", d->stripHtml(toNick)));
        } else if (toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 gave operator status to you</span>",
                          Aki::Settings::opColor().name(), fromNick);
            d->toLog(i18n("*** %1 gave operator status to you", d->stripHtml(fromNick)));
        } else if (!toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 gave operator status to %3</span>",
                          Aki::Settings::opColor().name(), fromNick, toNick);
            d->toLog(i18n("*** %1 gave operator status to %2", d->stripHtml(fromNick), d->stripHtml(toNick)));
        }
    } else if (mode == QChar('h')) {
        if (toYou && fromYou) {
            colour = i18n("<span style='color: %1;'>*** You gave yourself half operator status</span>",
                          Aki::Settings::opColor().name());
            d->toLog(i18n("*** You gace yourself half operator status"));
        } else if (fromYou && !toYou) {
            colour = i18n("<span style='color: %1;'>*** You gave half operator status to %2</span>",
                          Aki::Settings::opColor().name(), toNick);
            d->toLog(i18n("*** You gave half operator status to %1", d->stripHtml(toNick)));
        } else if (toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 gave half operator status to you</span>",
                          Aki::Settings::opColor().name(), fromNick);
            d->toLog(i18n("*** %1 gave half operator status to you", d->stripHtml(fromNick)));
        } else if (!toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 gave half operator status to %3</span>",
                          Aki::Settings::opColor().name(), fromNick, toNick);
            d->toLog(i18n("*** %1 gave half operator status to %2", d->stripHtml(fromNick), d->stripHtml(toNick)));
        }
    } else if (mode == QChar('v')) {
        if (toYou && fromYou) {
            colour = i18n("<span style='color: %1;'>*** You gave yourself permission to talk</span>",
                          Aki::Settings::voiceColor().name());
            d->toLog(i18n("*** You gave yourself permission to talk"));
        } else if (fromYou && !toYou) {
            colour = i18n("<span style='color: %1;'>*** You gave %2 permission to talk</span>",
                          Aki::Settings::voiceColor().name(), toNick);
            d->toLog(i18n("*** You gave %1 permission to talk", d->stripHtml(toNick)));
        } else if (toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 gave you permission to talk</span>",
                          Aki::Settings::voiceColor().name(), fromNick);
            d->toLog(i18n("*** %1 gave you permission to talk", d->stripHtml(fromNick)));
        } else if (!toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 gave %3 permission to talk</span>",
                          Aki::Settings::voiceColor().name(), fromNick, toNick);
            d->toLog(i18n("*** %1 gave %2 permission to talk", d->stripHtml(fromNick), d->stripHtml(toNick)));
        }
    } else {
        return;
    }

    d->appendMessage(colour);
}

void
ChatView::addMotd(const QString &message)
{
    QString colour = i18n("<span style=\"color: %1;\">[Motd] %2</span>",
                          Aki::Settings::noticeColor().name(), d->stripHtml(message));
    d->appendMessage(colour);
}

void
ChatView::addNick(const QString &oldNick, const QString &newNick)
{
    QString colour = i18n("<span style=\"color: %1;\">*** %2 is now known as %3</span>",
                          Aki::Settings::nickColor().name(), oldNick, newNick);
    d->toLog(i18n("*** %1 is now known as %2", oldNick, newNick));
    d->appendMessage(colour);
}

void
ChatView::addNotice(const QString &message)
{
    QString colour = i18n("<span style=\"color: %1;\">[Notice] %2</span>",
                          Aki::Settings::noticeColor().name(), message);
    d->toLog(i18n("[Notice] %1", d->stripHtml(message)));
    d->appendMessage(colour);
}

void
ChatView::addNotice(const QString &from, const QString &message)
{
    QString colour = i18n("<span style=\"color: %1;\">[Notice] <strong>%2</strong>: %3</span>",
                          Aki::Settings::noticeColor().name(), from, message);
    d->toLog(i18n("[Notice] %1: %2", d->stripHtml(from), d->stripHtml(message)));
    d->appendMessage(colour);
}

void
ChatView::addNoticeAuth(const QString &message)
{
    QString colour = i18n("<span style=\"color: %1;\">[Notice] %2</span>",
                          Aki::Settings::noticeColor().name(), d->stripHtml(message));
    d->appendMessage(colour);
}

void
ChatView::addPrivmsg(const QString &from, const QString &message)
{
    QString tmpString = QString("<span>&lt;%1&gt; %2</span>").arg(from).arg(message);
    d->toLog(i18n("<%1> %2", d->stripHtml(from), d->stripHtml(message)));
    d->appendMessage(tmpString);
}

void
ChatView::addPrivmsgHighlight(const QString &from, const QString &message)
{
    QString colour = QString("&lt;%2&gt; <span style='color: %1;'>%3</span>")
                        .arg(Aki::Settings::highlightColor().name(), from, message);
    d->toLog(i18n("<%1> %2", d->stripHtml(from), d->stripHtml(message)));
    d->appendMessage(colour);
}

void
ChatView::addRemoveBan(const QString &nick, const QString &mask)
{
    QString colour = i18n("<span style='color %1;'>*** %2 removed ban on %3</span>",
                          Aki::Settings::unbanColor().name(), nick, mask);
    d->toLog(i18n("*** %1 removed ban on %2", d->stripHtml(nick), mask));
    d->appendMessage(colour);
}

void
ChatView::addRemoveMode(const QString &fromNick, const QString &toNick, const QChar &mode,
                        bool toYou, bool fromYou)
{
    QString colour;
    if (mode == QChar('o')) {
        if (toYou && fromYou) {
            colour = i18n("<span style='color: %1;'>*** You took operator status from yourself</span>",
                          Aki::Settings::deOpColor().name());
            d->toLog(i18n("*** You took operator status from yourself"));
        } else if (fromYou && !toYou) {
            colour = i18n("<span style='color: %1;'>*** You took operator status from %2</span>",
                          Aki::Settings::deOpColor().name(), toNick);
            d->toLog(i18n("*** You took operator status from %1", d->stripHtml(toNick)));
        } else if (toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 took operator status from you</span>",
                          Aki::Settings::deOpColor().name(), fromNick);
            d->toLog(i18n("*** %1 took operator status from you", d->stripHtml(fromNick)));
        } else if (!toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 took operator status from %3</span>",
                          Aki::Settings::deOpColor().name(), fromNick, toNick);
            d->toLog(i18n("*** %1 took operator status from %2", d->stripHtml(fromNick), d->stripHtml(toNick)));
        }
    } else if (mode == QChar('h')) {
        if (toYou && fromYou) {
            colour = i18n("<span style='color: %1;'>*** You took half operator status from yourself</span>",
                          Aki::Settings::deOpColor().name());
            d->toLog(i18n("*** You took half operator status from yourself"));
        } else if (fromYou && !toYou) {
            colour = i18n("<span style='color: %1;'>*** You took half operator status from %2</span>",
                          Aki::Settings::deOpColor().name(), toNick);
            d->toLog(i18n("*** You took half operator status from %1", d->stripHtml(toNick)));
        } else if (toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 took half operator status from you</span>",
                          Aki::Settings::deOpColor().name(), fromNick);
            d->toLog(i18n("*** %1 took half operator status from you", d->stripHtml(fromNick)));
        } else if (!toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 took half operator status from %3</span>",
                          Aki::Settings::deOpColor().name(), fromNick, toNick);
            d->toLog(i18n("*** %1 took half operator status from %2", d->stripHtml(fromNick), d->stripHtml(toNick)));
        }
    } else if (mode == QChar('v')) {
        if (toYou && fromYou) {
            colour = i18n("<span style='color: %1;'>*** You took permission to talk from yourself</span>",
                          Aki::Settings::deVoiceColor().name());
            d->toLog(i18n("*** You took permission to talk from yourself"));
        } else if (fromYou && !toYou) {
            colour = i18n("<span style='color: %1;'>*** You took %2 permission to talk</span>",
                          Aki::Settings::deVoiceColor().name(), toNick);
            d->toLog(i18n("*** You took %1 permission to talk", d->stripHtml(toNick)));
        } else if (toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 took your permission to talk</span>",
                          Aki::Settings::deVoiceColor().name(), fromNick);
            d->toLog(i18n("*** %1 took your permission to talk", d->stripHtml(fromNick)));
        } else if (!toYou && !fromYou) {
            colour = i18n("<span style='color: %1;'>*** %2 took %3 permission to talk</span>",
                          Aki::Settings::deVoiceColor().name(), fromNick, toNick);
            d->toLog(i18n("*** %1 took %2 permission to talk", d->stripHtml(fromNick), d->stripHtml(toNick)));
        }
    } else {
        return;
    }

    d->appendMessage(colour);
}

void
ChatView::addSelfUModeSet(const QString &modes)
{
    QString colour = i18n("<span style='color: %1;'>[UMode] You changed your modes to %2</span>",
                          Aki::Settings::noticeColor().name(), modes);
    d->toLog(i18n("[UMode] You changed your modes to %1", modes));
    d->appendMessage(colour);
}

void
ChatView::addSupport(const QString &message)
{
    QString colour = i18n("<span style=\"color: %1;\">[Support] %2</span>",
                          Aki::Settings::noticeColor().name(), message);
    d->appendMessage(colour);
}

void
ChatView::addTime(const QString &server, const QString &time)
{
    QString colour = i18n("<span style='color: %1;'>[Time] %2 %3</span>",
                          Aki::Settings::noticeColor().name(), server, time);
    d->toLog(i18n("[Time] %1 %2", server, time));
    d->appendMessage(colour);
}

void
ChatView::addTopic(const QString &topic)
{
    QString colour = i18n("<span style='color: %1;'>*** The channel's topic is \"%2\"</span>",
                          Aki::Settings::topicColor().name(), topic);
    d->toLog(i18n("*** The channel's topic is \"%1\"", d->stripHtml(topic)));
    d->appendMessage(colour);
}

void
ChatView::addTopicChanged(const QString &nick, const QString &topic)
{
    QString colour = i18n("<span style='color: %1;'>*** %2 has changed the channel's topic to "
                          "\'%3\'</span>", Aki::Settings::topicColor().name(), nick, topic);
    d->toLog(i18n("*** %1 has changed the channel's topic to \"%2\"", d->stripHtml(nick), topic));
    d->appendMessage(colour);
}

void
ChatView::addTopicSetBy(const QString &name, const QString &time)
{
    QString colour = i18n("<span style='color: %1;'>*** Topic set by %2 on %3</span>",
                          Aki::Settings::topicColor().name(), name, time);
    d->toLog(i18n("*** Topic set by %1 on %2", name, time));
    d->appendMessage(colour);
}

void
ChatView::addUMode(const QString &modes)
{
    QString colour = i18n("<span style='color: %1;'>[UMode] Your modes are: %2</span>",
                          Aki::Settings::noticeColor().name(), modes);
    d->toLog(i18n("[UMode] Your modes are: %1", modes));
    d->appendMessage(colour);
}

void
ChatView::addUMode(const QString &nick, const QString &modes)
{
    QString colour = i18n("<span style='color: %1;'>[UMode] %2 set your modes to %3</span>",
                          Aki::Settings::noticeColor().name(), nick, modes);
    d->toLog(i18n("[UMode] %1 set your modes to %2", d->stripHtml(nick), modes));
    d->appendMessage(colour);
}

void
ChatView::addUserHost(const QStringList &nicks, const QStringList &hosts)
{
    foreach (const QString &nick, nicks) {
        foreach (const QString &host, hosts) {
            QString colour = i18n("<span style='color: %1;'>[UserHost] %2 is %3</span>",
                                  Aki::Settings::noticeColor().name(),
                                  nick, host);
            d->appendMessage(colour);
        }
    }
}

void
ChatView::addUserJoin(const QString &channel, const QString &nick, const QString &hostMask)
{
    QString colour = i18n("<span style='color: %1;'>--&gt; %2 (%3) has joined %4</span>",
                          Aki::Settings::joinColor().name(), nick, hostMask, channel);
    d->toLog(i18n("--> %1 (%2) has joined %3", d->stripHtml(nick), hostMask, channel));
    d->appendMessage(colour);
}

void
ChatView::addUserPart(const QString &channel, const QString &nick, const QString &hostMask,
                      const QString &partMessage)
{
    QString tmp;

    if (partMessage.isEmpty()) {
        tmp = hostMask;
    } else {
        tmp = partMessage;
    }

    QString colour = i18n("<span style='color: %1;'>&lt;-- %2 (%3) has left %4</span>",
                          Aki::Settings::partColor().name(), nick, tmp, channel);
    d->toLog(i18n("<-- %1 (%2) has left %4", d->stripHtml(nick), tmp, channel));
    d->appendMessage(colour);
}

void
ChatView::addUserQuit(const QString &nick, const QString &hostMask,
                      const QString &quitMessage)
{
    QString colour = i18n("<span style='color: %1;'>&lt;-- %2 (%3) has quit</span>",
                          Aki::Settings::quitColor().name(), nick,
                          (quitMessage.isEmpty() || quitMessage.isNull()) ? hostMask : quitMessage);
    d->toLog(i18n("<-- %1 (%2) has quit.", d->stripHtml(nick),
                  (quitMessage.isEmpty() || quitMessage.isNull()) ? hostMask : quitMessage));
    d->appendMessage(colour);
}

void
ChatView::addVersion(const QString &message)
{
    QString colour = i18n("<span style='color: %1;'>[Version] %2</span>",
                          Aki::Settings::noticeColor().name(), message);
    d->appendMessage(colour);
}

void
ChatView::addWelcome(const QString &message)
{
    QString colour = i18n("<span style=\"color: %1;\">[Welcome] %2</span>",
                           Aki::Settings::noticeColor().name(), message);
    d->appendMessage(colour);
}

void
ChatView::addWho(const QString &channel, const QString &message)
{
    QString colour = i18n("<span style='color: %1;'>[Who:%2] %3</span>",
                          Aki::Settings::noticeColor().name(), channel, message);
    d->appendMessage(colour);
}

void
ChatView::addWho(const QString &channel, const QString &userName, const QString &address,
                 const QString &server, const QString &nick, const QString &flags, int hops,
                 const QString &realName)
{
    QString colour = i18n("<span style='color: %1;'>[Who:%2] <b><u>Username</u></b>: %3, <b><u>Address</u></b>: %4, "
                          "<b><u>Server</u></b>: %5, <b><u>Nick</u></b>: %6, <b><u>Flags</u></b>: %7, "
                          "<b><u>Hops</u></b>: %8, <b><u>Real Name</u></b>: %9</span>",
                          Aki::Settings::noticeColor().name(), channel, userName, address, server, nick,
                          flags, hops, realName);
    d->appendMessage(colour);
}

void
ChatView::addWhoIs(const QString &nick, const QString &info)
{
    QString colour = i18n("<span style='color: %1;'>[WhoIs:%2] %3</span>",
                          Aki::Settings::noticeColor().name(), nick, info);
    d->appendMessage(colour);
}

void
ChatView::addWhoIsChannels(const QString &nick, const QString &channels)
{
    QString colour = i18n("<span style='color: %1;'>[WhoIs:%2] is currently on channels: %3</span>",
                          Aki::Settings::noticeColor().name(), nick, channels);
    d->appendMessage(colour);
}

void
ChatView::addWhoIsIdentified(const QString &nick, const QString &info)
{
    QString colour = i18n("<span style='color: %1;'>[WhoIs:%2] %3</span>",
                          Aki::Settings::noticeColor().name(), nick, info);
    d->appendMessage(colour);
}

void
ChatView::addWhoIsIdle(const QString &nick, const QString &idleTime, const QString &signon)
{
    QString colour = i18n("<span style='color: %1;'>[WhoIs:%2] idle: %3, signon: %4</span>",
                          Aki::Settings::noticeColor().name(), nick, idleTime, signon);
    d->appendMessage(colour);
}

void
ChatView::addWhoIsServer(const QString &nick, const QString &server, const QString &info)
{
    QString colour = i18n("<span style='color: %1;'>[WhoIs:%2] is online via %3 (%4)</span>",
                          Aki::Settings::noticeColor().name(), nick, server, info);
    d->appendMessage(colour);
}

void
ChatView::addWhoIsUser(const QString &nick, const QString &username, const QString &address,
                       const QString &info)
{
    QString colour = i18n("<span style='color: %1;'>[WhoIs:%2] is %3@%4 (%5)</span>",
                          Aki::Settings::noticeColor().name(), nick, username, address, info);
    d->appendMessage(colour);
}

void
ChatView::addWhoWas(const QString &nick, const QString &message)
{
    QString colour = i18n("<span style='color: %1;'>[WhoWas:%2] %3</span>",
                          Aki::Settings::noticeColor().name(), nick, message);
    d->appendMessage(colour);
}

void
ChatView::addWhoWasUser(const QString &nick, const QString &username, const QString &address,
                       const QString &info)
{
    QString colour = i18n("<span style='color: %1;'>[WhoWas:%2] is %3@%4 (%5)</span>",
                          Aki::Settings::noticeColor().name(), nick, username, address, info);
    d->appendMessage(colour);
}

void
ChatView::add901(const QString &id, const QString &identName, const QString &address,
                 const QString &message)
{
    d->toLog(QString("%1 %2@%3 %4").arg(id, identName, address, d->stripHtml(message)));
    addMessage(QString("%1 %2@%3 %4").arg(id, identName, address, message));
}

void
ChatView::setLog(Aki::LogFile *logFile)
{
    d->logFile = logFile;
}

void
ChatView::addLogLine(const QString &line)
{
    QString colour = QString("<span style='color: #C3C3C3;'>%1</span>")
                             .arg(line);
    d->appendMessage(colour, false);
}
