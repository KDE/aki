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

#include "channelwindow.h"
#include "chatparser.h"
#include "config/identityconfig.h"
#include "config/replaceconfig.h"
#include "dialogs/channeltopicdialog.h"
#include "dialogs/replacedialog.h"
#include "logfile.h"
#include "nicklistmodel.h"
#include "notifications.h"
#include "settings.h"
#include "ui/channelview.h"
#include <Aki/Irc/Color>
#include <Aki/Irc/Socket>
#include <Aki/Irc/User>
#include <KDebug>
#include <KInputDialog>
#include <KPasswordDialog>
#include <QMenu>
#include <QShowEvent>
#include <QTimer>
#include <KMessageBox>
using namespace Aki;

namespace Aki
{
class ChannelWindowPrivate
{
public:
    ChannelWindowPrivate(ChannelWindow *qq)
        : q(qq),
        identity(0),
        parser(0),
        selectedUser(0),
        whoTimer(0),
        isWhoRunning(false)
    {
    }

    void nickSelectorActivated(const QString &nick)
    {
        q->socket()->rfcNick(nick);
        q->chatInput->setFocus();
    }

    void whoIsTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcWhoIs(selectedUser->nick());
    }

    void versionTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcPrivmsg(selectedUser->nick().toLatin1(),
                                QString("\x01" "VERSION" "\x01"));
    }

    void banDomainTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcMode(q->name(), QString("+b *!*@%1").arg(selectedUser->host()));
    }

    void banHostTriggered()
    {
        if (!selectedUser) {
            return;
        }

        QString ipAddress("^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$");
        QString cloak("^[^ ]+/[^ ]+$");

        if (selectedUser->host().contains(QRegExp(ipAddress))) {
            q->socket()->rfcMode(q->name(), QString("+b *!*@*,%1").arg(selectedUser->host()));
        } else if (selectedUser->host().contains(QRegExp(cloak))) {
            q->socket()->rfcMode(q->name(), QString("+b *!*@*.%1").arg(selectedUser->host()));
        } else {
            QString tmpHost = selectedUser->host();
            QString tmp = tmpHost.mid(0, tmpHost.indexOf(QChar('.')));
            tmpHost.remove(0, tmp.length());
            q->socket()->rfcMode(q->name(), QString("+b *!*@*%1").arg(tmpHost));
        }
    }

    void banUserDomainTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcMode(q->name(), QString("+b *!%1@%2").arg(selectedUser->user(), selectedUser->host()));
    }

    void banUserHostTriggered()
    {
        if (!selectedUser) {
            return;
        }

        QString ipAddress("^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$");
        QString cloak("^[^ ]+/[^ ]+$");

        if (selectedUser->host().contains(QRegExp(ipAddress))) {
            q->socket()->rfcMode(q->name(), QString("+b *!%1@*.%2").arg(selectedUser->user(), selectedUser->host()));
        } else if (selectedUser->host().contains(QRegExp(cloak))) {
            q->socket()->rfcMode(q->name(), QString("+b *!%1@*.%2").arg(selectedUser->user(), selectedUser->host()));
        } else {
            QString tmpHost = selectedUser->host();
            QString tmp = tmpHost.mid(0, tmpHost.indexOf(QChar('.')));
            tmpHost.remove(0, tmp.length());
            q->socket()->rfcMode(q->name(), QString("+b *!%1@*%2").arg(selectedUser->user(), tmpHost));
        }
    }

    void customContextMenuRequested(const QPoint &pos)
    {
        Q_UNUSED(pos);

        QMenu *menu = new QMenu(menu);

        QMenu *ctcpMenu = new QMenu(i18n("CTCP"), menu);
        QMenu *modesMenu = new QMenu(i18n("Modes"), menu);
        QMenu *kickBanMenu = new QMenu(i18n("Kick/Ban"), menu);

        QAction *whoIsAction = new QAction(ctcpMenu);
        whoIsAction->setText(i18n("WhoIs"));
        q->connect(whoIsAction, SIGNAL(triggered(bool)),
                   q, SLOT(whoIsTriggered()));

        QAction *versionAction = new QAction(ctcpMenu);
        versionAction->setText(i18n("Version"));
        q->connect(versionAction, SIGNAL(triggered(bool)),
                   q, SLOT(versionTriggered()));

        ctcpMenu->addAction(whoIsAction);
        ctcpMenu->addAction(versionAction);

        QAction *giveOpAction = new QAction(modesMenu);
        giveOpAction->setText(i18n("Give Ops"));
        giveOpAction->setIcon(KIcon("irc-operator"));
        q->connect(giveOpAction, SIGNAL(triggered(bool)),
                   q, SLOT(giveOpTriggered()));

        QAction *takeOpAction = new QAction(modesMenu);
        takeOpAction->setText(i18n("Take Ops"));
        takeOpAction->setIcon(KIcon("irc-remove-operator"));
        q->connect(takeOpAction, SIGNAL(triggered(bool)),
                   q, SLOT(takeOpTriggered()));

        QAction *giveHalfOpAction = new QAction(modesMenu);
        giveHalfOpAction->setText(i18n("Give Half-Op"));
        q->connect(giveHalfOpAction, SIGNAL(triggered(bool)),
                   q, SLOT(giveHalfOpTriggered()));

        QAction *takeHalfOpAction = new QAction(modesMenu);
        takeHalfOpAction->setText(i18n("Take Half-Op"));
        q->connect(takeHalfOpAction, SIGNAL(triggered(bool)),
                   q, SLOT(takeHalfOpTriggered()));

        QAction *giveVoiceAction = new QAction(modesMenu);
        giveVoiceAction->setIcon(KIcon("irc-voice"));
        giveVoiceAction->setText(i18n("Give Voice"));
        q->connect(giveVoiceAction, SIGNAL(triggered(bool)),
                   q, SLOT(giveVoiceTriggered()));

        QAction *takeVoiceAction = new QAction(modesMenu);
        takeVoiceAction->setIcon(KIcon("irc-unvoice"));
        takeVoiceAction->setText(i18n("Take Voice"));
        q->connect(takeVoiceAction, SIGNAL(triggered(bool)),
                   q, SLOT(takeVoiceTriggered()));

        modesMenu->addAction(giveOpAction);
        modesMenu->addAction(takeOpAction);
        modesMenu->addSeparator();
        modesMenu->addAction(giveHalfOpAction);
        modesMenu->addAction(takeHalfOpAction);
        modesMenu->addSeparator();
        modesMenu->addAction(giveVoiceAction);
        modesMenu->addAction(takeVoiceAction);

        QAction *kickUserAction = new QAction(kickBanMenu);
        kickUserAction->setText(i18n("Kick"));
        q->connect(kickUserAction, SIGNAL(triggered(bool)),
                   q, SLOT(kickUserTriggered()));

        QAction *banUserAction = new QAction(kickBanMenu);
        banUserAction->setText(i18n("Ban"));

        QAction *banHostAction = new QAction(kickBanMenu);
        banHostAction->setText(i18n("Ban *!*@*.host"));
        q->connect(banHostAction, SIGNAL(triggered(bool)),
                   q, SLOT(banHostTriggered()));

        QAction *banDomainAction = new QAction(kickBanMenu);
        banDomainAction->setText(i18n("Ban *!*@domain"));
        q->connect(banDomainAction, SIGNAL(triggered(bool)),
                   q, SLOT(banDomainTriggered()));

        QAction *banUserHostAction = new QAction(kickBanMenu);
        banUserHostAction->setText(i18n("Ban *!user@*.host"));
        q->connect(banUserHostAction, SIGNAL(triggered(bool)),
                   q, SLOT(banUserHostTriggered()));

        QAction *banUserDomainAction = new QAction(kickBanMenu);
        banUserDomainAction->setText(i18n("Ban *!user@domain"));
        q->connect(banUserDomainAction, SIGNAL(triggered(bool)),
                   q, SLOT(banUserDomainTriggered()));

        QAction *kickBanHostAction = new QAction(kickBanMenu);
        kickBanHostAction->setText(i18n("KickBan *!*@*.host"));
        q->connect(kickBanHostAction, SIGNAL(triggered(bool)),
                   q, SLOT(kickBanHostTriggered()));

        QAction *kickBanDomainAction = new QAction(kickBanMenu);
        kickBanDomainAction->setText(i18n("KickBan *!*@domain"));
        q->connect(kickBanDomainAction, SIGNAL(triggered(bool)),
                   q, SLOT(kickBanDomainTriggered()));

        QAction *kickBanUserHostAction = new QAction(kickBanMenu);
        kickBanUserHostAction->setText(i18n("KickBan *!user@*.host"));
        q->connect(kickBanUserHostAction, SIGNAL(triggered(bool)),
                   q, SLOT(kickBanUserHostTriggered()));

        QAction *kickBanUserDomainAction = new QAction(kickBanMenu);
        kickBanUserDomainAction->setText(i18n("KickBan *!user@domain"));
        q->connect(kickBanUserDomainAction, SIGNAL(triggered(bool)),
                   q, SLOT(kickBanUserDomainTriggered()));

        QAction *openQueryAction = new QAction(menu);
        openQueryAction->setText(i18n("Open Query"));
        q->connect(openQueryAction, SIGNAL(triggered(bool)),
                   q, SLOT(openQueryTriggered()));

        kickBanMenu->addAction(kickUserAction);
        kickBanMenu->addAction(banUserAction);
        kickBanMenu->addSeparator();
        kickBanMenu->addAction(banHostAction);
        kickBanMenu->addAction(banDomainAction);
        kickBanMenu->addAction(banUserHostAction);
        kickBanMenu->addAction(banUserDomainAction);
        kickBanMenu->addSeparator();
        kickBanMenu->addAction(kickBanHostAction);
        kickBanMenu->addAction(kickBanDomainAction);
        kickBanMenu->addAction(kickBanUserHostAction);
        kickBanMenu->addAction(kickBanUserDomainAction);

        QModelIndex currentIndex = q->userList->currentIndex();
        if (currentIndex.isValid()) {
            menu->addMenu(ctcpMenu);
            menu->addSeparator();
            menu->addMenu(modesMenu);
            menu->addMenu(kickBanMenu);
            menu->addSeparator();
            menu->addAction(openQueryAction);
            menu->exec(QCursor::pos());
        }
    }

    void giveHalfOpTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcMode(q->name(), QString("+h %1").arg(selectedUser->nick()));
    }

    void giveOpTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcMode(q->name(), QString("+o %1").arg(selectedUser->nick()));
    }

    void giveVoiceTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcMode(q->name(), QString("+v %1").arg(selectedUser->nick()));
    }

    void kickBanDomainTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcMode(q->name(), QString("+b *!%1@%2").arg(selectedUser->user(), selectedUser->host()));
        q->socket()->rfcKick(q->name(), selectedUser->nick(), identity->kickMessage());
    }

    void kickBanHostTriggered()
    {
        if (!selectedUser) {
            return;
        }

        QString ipAddress("^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$");
        QString cloak("^[^ ]+/[^ ]+$");

        if (selectedUser->host().contains(QRegExp(ipAddress))) {
            q->socket()->rfcMode(q->name(), QString("+b *!*@*,%1").arg(selectedUser->host()));
        } else if (selectedUser->host().contains(QRegExp(cloak))) {
            q->socket()->rfcMode(q->name(), QString("+b *!*@*.%1").arg(selectedUser->host()));
        } else {
            QString tmpHost = selectedUser->host();
            QString tmp = tmpHost.mid(0, tmpHost.indexOf(QChar('.')));
            tmpHost.remove(0, tmp.length());
            q->socket()->rfcMode(q->name(), QString("+b *!*@*%1").arg(tmpHost));
        }
        q->socket()->rfcKick(q->name(), selectedUser->nick(), identity->kickMessage());
    }

    void kickBanUserDomainTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcMode(q->name(), QString("+b *!%1@%2").arg(selectedUser->user(), selectedUser->host()));
        q->socket()->rfcKick(q->name(), selectedUser->nick(), identity->kickMessage());
    }

    void kickBanUserHostTriggered()
    {
        if (!selectedUser) {
            return;
        }

        QString ipAddress("^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$");
        QString cloak("^[^ ]+/[^ ]+$");

        if (selectedUser->host().contains(QRegExp(ipAddress))) {
            q->socket()->rfcMode(q->name(), QString("+b *!%1@*.%2").arg(selectedUser->user(), selectedUser->host()));
        } else if (selectedUser->host().contains(QRegExp(cloak))) {
            q->socket()->rfcMode(q->name(), QString("+b *!%1@*.%2").arg(selectedUser->user(), selectedUser->host()));
        } else {
            QString tmpHost = selectedUser->host();
            QString tmp = tmpHost.mid(0, tmpHost.indexOf(QChar('.')));
            tmpHost.remove(0, tmp.length());
            q->socket()->rfcMode(q->name(), QString("+b *!%1@*%2").arg(selectedUser->user(), tmpHost));
        }
        q->socket()->rfcKick(q->name(), selectedUser->nick(), identity->kickMessage());
    }

    void kickUserTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcKick(q->name(), selectedUser->nick(), identity->kickMessage());
    }

    void showCompletion()
    {
        if (q->chatInput->text().isEmpty()) {
            return;
        }

        int oldPosition;
        int position;

        QString line = q->chatInput->text();
        oldPosition = position = line.length();

        QString attemptedNick;

        while (position && line[position - 1] != QChar(' ')) {
            --position;
        }

        attemptedNick = line.mid(position, oldPosition + position);

        QStringList nicks;
        Qt::CaseSensitivity cs = Aki::Settings::nickCaseSensitive() ?
                                    Qt::CaseSensitive : Qt::CaseInsensitive;

        foreach (Aki::Irc::User *user, q->userList->users()) {
            if (attemptedNick.isEmpty() ||
                user->nick().startsWith(attemptedNick, cs)) {
                nicks << user->nick();
            }
        }

        qSort(nicks);

        if (nicks.count() == 1) {
            q->chatInput->insertCompletion(nicks.at(0));
        } else {
            q->chatInput->setNickCompletionList(nicks);
        }
    }

    void takeHalfOpTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcMode(q->name(), QString("-h %1").arg(selectedUser->nick()));
    }

    void takeOpTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcMode(q->name(), QString("-o %1").arg(selectedUser->nick()));
    }

    void takeVoiceTriggered()
    {
        if (!selectedUser) {
            return;
        }

        q->socket()->rfcMode(q->name(), QString("-v %1").arg(selectedUser->nick()));
    }

    void textSubmitted()
    {
        emit q->textSubmitted(q, q->chatInput->text());
        q->chatInput->clear();
    }

    void channelKeyStateChanged(bool state)
    {
        if (state) {
            KPasswordDialog dlg(q);
            dlg.setPrompt(i18n("Enter a channel key"));
            if (!dlg.exec()) {
                return;
            }

            q->chatModes->setKey(dlg.password());
            q->socket()->rfcMode(q->name(), "+k " + q->chatModes->key());
        } else {
            q->chatModes->setKey("");
            q->socket()->rfcMode(q->name(), "-k");
        }
    }

    void newChannelKey(const QString &key)
    {
        if (key.isEmpty() || key.isNull()) {
            q->socket()->rfcMode(q->name(), "+k " + key);
        } else {
            q->socket()->rfcMode(q->name(), "-k");
        }
    }

    void channelLimitStateChanged(bool state)
    {
        if (state) {
            bool ok = false;
            int limit = KInputDialog::getInteger(i18n("New Channel Limit"), i18n("Enter a new channel limit"),
                                                 0, 0, 20000, 10, &ok, q);
            if (ok) {
                q->chatModes->setLimit(limit);
                q->socket()->rfcMode(q->name(), "+l " + QString::number(limit));
            }
        } else {
            q->socket()->rfcMode(q->name(), "-l");
        }
    }

    void newChannelLimit(int limit)
    {
        if (limit <= 0) {
            q->socket()->rfcMode(q->name(), "-l");
        } else {
            q->socket()->rfcMode(q->name(), "+l " + QString::number(limit));
        }
    }

    void topicProtectionStateChanged(bool state)
    {
        if (state) {
            q->socket()->rfcMode(q->name(), "+t");
            foreach (const Aki::Irc::User *user, q->userList->users()) {
                if (user->nick() == q->socket()->currentNick()) {
                    if (user->isOp()) {
                        topicDialog->setTopicEditRights(true);
                    } else {
                        topicDialog->setTopicEditRights(false);
                    }
                    return;
                }
            }
        } else {
            q->socket()->rfcMode(q->name(), "-t");
            foreach (const Aki::Irc::User *user, q->userList->users()) {
                if (user->nick() == q->socket()->currentNick()) {
                    topicDialog->setTopicEditRights(true);
                    return;
                }
            }
        }
    }

    void noOutsideMessagesStateChanged(bool state)
    {
        if (state) {
            q->socket()->rfcMode(q->name(), "+n");
        } else {
            q->socket()->rfcMode(q->name(), "-n");
        }
    }

    void secretStateChanged(bool state)
    {
        if (state) {
            q->socket()->rfcMode(q->name(), "+s");
        } else {
            q->socket()->rfcMode(q->name(), "-s");
        }
    }

    void inviteOnlyStateChanged(bool state)
    {
        if (state) {
            q->socket()->rfcMode(q->name(), "+i");
        } else {
            q->socket()->rfcMode(q->name(), "-i");
        }
    }

    void privateStateChanged(bool state)
    {
        if (state) {
            q->socket()->rfcMode(q->name(), "+p");
        } else {
            q->socket()->rfcMode(q->name(), "-p");
        }
    }

    void moderatedStateChanged(bool state)
    {
        if (state) {
            q->socket()->rfcMode(q->name(), "+m");
        } else {
            q->socket()->rfcMode(q->name(), "-m");
        }
    }

    void whoTimerTimeout()
    {
        if (isWhoRunning || q->userList->count() > Aki::Settings::maxNumberOfUsers()) {
            whoTimer->stop();
            whoTimer->start(Aki::Settings::updateInterval() * 1000);
        } else {
            emit q->whoAdded(q->name().toLower());
        }
    }

    void chatTopicReturnPressed(const QString &topic)
    {
        q->socket()->rfcTopic(q->name(), topic);
    }

    void splitterMoved(int /*pos*/, int /*index*/)
    {
        KSharedConfigPtr self = KGlobal::config();
        KConfigGroup splitSize;
        QList<int> sizes = q->splitter->sizes();

        splitSize = KConfigGroup(self, "SplitSize");
        splitSize.writeEntry("size", sizes);
        splitSize.sync();
    }

    void addMode(Aki::Irc::User *user, const QModelIndex &index, const QString &mode)
    {
        QString tmpModes = user->modes();
        user->setModes(tmpModes + mode);

        if (user->nick() == q->socket()->currentNick()) {
            if (mode == "o") {
                q->modeBar()->setEnabled(true);
                topicDialog->setTopicEditRights(true);
            }
        }

        q->userList->model()->setData(index, QVariant::fromValue<Aki::Irc::User*>(user),
                                      Aki::NickListModel::IrcUserRole);
        q->userList->update(index);
    }

    void removeMode(Aki::Irc::User *user, const QModelIndex &index, const QString &mode)
    {
        QString tmpModes = user->modes();
        user->removeModes(mode);

        if (user->nick() == q->socket()->currentNick()) {
            if (mode == "o") {
                q->modeBar()->setEnabled(false);
                topicDialog->setTopicEditRights(!q->modeBar()->isTopicProtectionEnabled());
            }
        }

        q->userList->model()->setData(index, QVariant::fromValue<Aki::Irc::User*>(user),
                                      Aki::NickListModel::IrcUserRole);
        q->userList->update(index);
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

    void userUrlClicked(const QString &nick)
    {
        foreach (Aki::Irc::User *u, q->users()) {
            if (u->nick() == nick) {
                selectedUser = u;
                break;
            }
        }
        
        QMenu *menu = new QMenu(q);
        QMenu *ctcpMenu = new QMenu(i18n("CTCP"), menu);
        QMenu *modesMenu = new QMenu(i18n("Modes"), menu);
        QMenu *kickBanMenu = new QMenu(i18n("Kick/Ban"), menu);

        QAction *whoIsAction = new QAction(ctcpMenu);
        whoIsAction->setText(i18n("WhoIs"));
        q->connect(whoIsAction, SIGNAL(triggered(bool)),
                   q, SLOT(whoIsTriggered()));

        QAction *versionAction = new QAction(ctcpMenu);
        versionAction->setText(i18n("Version"));
        q->connect(versionAction, SIGNAL(triggered(bool)),
                   q, SLOT(versionTriggered()));

        ctcpMenu->addAction(whoIsAction);
        ctcpMenu->addAction(versionAction);

        QAction *giveOpAction = new QAction(modesMenu);
        giveOpAction->setText(i18n("Give Ops"));
        giveOpAction->setIcon(KIcon("irc-operator"));
        q->connect(giveOpAction, SIGNAL(triggered(bool)),
                   q, SLOT(giveOpTriggered()));

        QAction *takeOpAction = new QAction(modesMenu);
        takeOpAction->setText(i18n("Take Ops"));
        takeOpAction->setIcon(KIcon("irc-remove-operator"));
        q->connect(takeOpAction, SIGNAL(triggered(bool)),
                   q, SLOT(takeOpTriggered()));

        QAction *giveHalfOpAction = new QAction(modesMenu);
        giveHalfOpAction->setText(i18n("Give Half-Op"));
        q->connect(giveHalfOpAction, SIGNAL(triggered(bool)),
                   q, SLOT(giveHalfOpTriggered()));

        QAction *takeHalfOpAction = new QAction(modesMenu);
        takeHalfOpAction->setText(i18n("Take Half-Op"));
        q->connect(takeHalfOpAction, SIGNAL(triggered(bool)),
                   q, SLOT(takeHalfOpTriggered()));

        QAction *giveVoiceAction = new QAction(modesMenu);
        giveVoiceAction->setIcon(KIcon("irc-voice"));
        giveVoiceAction->setText(i18n("Give Voice"));
        q->connect(giveVoiceAction, SIGNAL(triggered(bool)),
                   q, SLOT(giveVoiceTriggered()));

        QAction *takeVoiceAction = new QAction(modesMenu);
        takeVoiceAction->setIcon(KIcon("irc-unvoice"));
        takeVoiceAction->setText(i18n("Take Voice"));
        q->connect(takeVoiceAction, SIGNAL(triggered(bool)),
                   q, SLOT(takeVoiceTriggered()));

        modesMenu->addAction(giveOpAction);
        modesMenu->addAction(takeOpAction);
        modesMenu->addSeparator();
        modesMenu->addAction(giveHalfOpAction);
        modesMenu->addAction(takeHalfOpAction);
        modesMenu->addSeparator();
        modesMenu->addAction(giveVoiceAction);
        modesMenu->addAction(takeVoiceAction);

        QAction *kickUserAction = new QAction(kickBanMenu);
        kickUserAction->setText(i18n("Kick"));
        q->connect(kickUserAction, SIGNAL(triggered(bool)),
                   q, SLOT(kickUserTriggered()));

        QAction *banUserAction = new QAction(kickBanMenu);
        banUserAction->setText(i18n("Ban"));

        QAction *banHostAction = new QAction(kickBanMenu);
        banHostAction->setText(i18n("Ban *!*@*.host"));
        q->connect(banHostAction, SIGNAL(triggered(bool)),
                   q, SLOT(banHostTriggered()));

        QAction *banDomainAction = new QAction(kickBanMenu);
        banDomainAction->setText(i18n("Ban *!*@domain"));
        q->connect(banDomainAction, SIGNAL(triggered(bool)),
                   q, SLOT(banDomainTriggered()));

        QAction *banUserHostAction = new QAction(kickBanMenu);
        banUserHostAction->setText(i18n("Ban *!user@*.host"));
        q->connect(banUserHostAction, SIGNAL(triggered(bool)),
                   q, SLOT(banUserHostTriggered()));

        QAction *banUserDomainAction = new QAction(kickBanMenu);
        banUserDomainAction->setText(i18n("Ban *!user@domain"));
        q->connect(banUserDomainAction, SIGNAL(triggered(bool)),
                   q, SLOT(banUserDomainTriggered()));

        QAction *kickBanHostAction = new QAction(kickBanMenu);
        kickBanHostAction->setText(i18n("KickBan *!*@*.host"));
        q->connect(kickBanHostAction, SIGNAL(triggered(bool)),
                   q, SLOT(kickBanHostTriggered()));

        QAction *kickBanDomainAction = new QAction(kickBanMenu);
        kickBanDomainAction->setText(i18n("KickBan *!*@domain"));
        q->connect(kickBanDomainAction, SIGNAL(triggered(bool)),
                   q, SLOT(kickBanDomainTriggered()));

        QAction *kickBanUserHostAction = new QAction(kickBanMenu);
        kickBanUserHostAction->setText(i18n("KickBan *!user@*.host"));
        q->connect(kickBanUserHostAction, SIGNAL(triggered(bool)),
                   q, SLOT(kickBanUserHostTriggered()));

        QAction *kickBanUserDomainAction = new QAction(kickBanMenu);
        kickBanUserDomainAction->setText(i18n("KickBan *!user@domain"));
        q->connect(kickBanUserDomainAction, SIGNAL(triggered(bool)),
                   q, SLOT(kickBanUserDomainTriggered()));

        QAction *openQueryAction = new QAction(menu);
        openQueryAction->setText(i18n("Open Query"));
        q->connect(openQueryAction, SIGNAL(triggered(bool)),
                   q, SLOT(openQueryTriggered()));

        kickBanMenu->addAction(kickUserAction);
        kickBanMenu->addAction(banUserAction);
        kickBanMenu->addSeparator();
        kickBanMenu->addAction(banHostAction);
        kickBanMenu->addAction(banDomainAction);
        kickBanMenu->addAction(banUserHostAction);
        kickBanMenu->addAction(banUserDomainAction);
        kickBanMenu->addSeparator();
        kickBanMenu->addAction(kickBanHostAction);
        kickBanMenu->addAction(kickBanDomainAction);
        kickBanMenu->addAction(kickBanUserHostAction);
        kickBanMenu->addAction(kickBanUserDomainAction);

        menu->addMenu(ctcpMenu);
        menu->addSeparator();
        menu->addMenu(modesMenu);
        menu->addMenu(kickBanMenu);
        menu->addSeparator();
        menu->addAction(openQueryAction);
        menu->exec(QCursor::pos());
    }

    void openQueryTriggered()
    {
        if (!selectedUser) {
            return;
        }

        Aki::Irc::User *self = 0;
        foreach (Aki::Irc::User *u, q->users()) {
            if (u->nick() == q->socket()->currentNick()) {
                self = u;
                break;
            }
        }

        q->channelView()->addQuery(self, selectedUser, QString());
    }

    void userClicked(const QModelIndex &index)
    {
        Aki::NickListModel *model = static_cast<Aki::NickListModel*>(q->userList->model());
        selectedUser = model->data(index, Aki::NickListModel::IrcUserRole)
                            .value<Aki::Irc::User*>();
    }

    void userPressed(const QModelIndex &index)
    {
        userClicked(index);
    }

    void findTextTriggered()
    {
        q->searchBar()->show();
        q->searchBar()->setFocus();
    }

    void channelTopicHistoryTriggered()
    {
        topicDialog->show();
    }

    void changedTopic(const QString &topic)
    {
        q->socket()->rfcTopic(q->name(), topic);
    }

    Aki::ChannelWindow *q;
    Aki::IdentityConfig *identity;
    Aki::ChatParser *parser;
    Aki::Irc::User *selectedUser;
    Aki::ChannelTopicDialog *topicDialog;
    QTimer *whoTimer;
    bool isWhoRunning;
    bool isTopicProtectionEnabled;
}; // End of class ChannelWindowPrivate.
} // End of namespace Aki.

ChannelWindow::ChannelWindow(const QString &name, Aki::IdentityConfig *identityConfig,
                             Aki::Irc::Socket *socket, QWidget *parent)
    : Aki::BaseWindow(name, Aki::BaseWindow::ChannelWindow, parent)
{
    d.reset(new Aki::ChannelWindowPrivate(this));

    d->identity = identityConfig;
    d->whoTimer = new QTimer(this);

    setSocket(socket);
    setupUi(this);
    setView(chatOutput);
    channelSearch->hide();
    modeBar()->setEnabled(false);
    setLogFile(new Aki::LogFile(socket->name(), name, this));
    view()->setLog(logFile());

    chatOutput->setChannel(true);

    d->topicDialog = new Aki::ChannelTopicDialog(this);
    d->topicDialog->setWindowTitle(i18n("Channel Topic History - %1", name));

    chatInput->setFocus();

    QStringList log = logFile()->logLines();
    if (!log.isEmpty()) {
        foreach (const QString &str, log) {
            view()->addLogLine(str);
        }
    }

    logFile()->open();

    userList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(userList, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customContextMenuRequested(QPoint)));
    connect(chatInput, SIGNAL(textSubmitted()),
            SLOT(textSubmitted()));
    connect(chatInput, SIGNAL(showCompletion()),
            SLOT(showCompletion()));
    connect(nickSelector, SIGNAL(activated(QString)),
            SLOT(nickSelectorActivated(QString)));
    connect(chatTopic, SIGNAL(returnPressed(QString)),
            SLOT(chatTopicReturnPressed(QString)));
    connect(splitter, SIGNAL(splitterMoved(int,int)),
            SLOT(splitterMoved(int,int)));
    connect(chatModes, SIGNAL(channelKeyStateChanged(bool)),
            SLOT(channelKeyStateChanged(bool)));
    connect(chatModes, SIGNAL(newChannelKey(QString)),
            SLOT(newChannelKey(QString)));
    connect(chatModes, SIGNAL(channelLimitStateChanged(bool)),
            SLOT(channelLimitStateChanged(bool)));
    connect(chatModes, SIGNAL(newChannelLimit(int)),
            SLOT(newChannelLimit(int)));
    connect(chatModes, SIGNAL(topicProtectionStateChanged(bool)),
            SLOT(topicProtectionStateChanged(bool)));
    connect(chatModes, SIGNAL(noOutsideMessagesStateChanged(bool)),
            SLOT(noOutsideMessagesStateChanged(bool)));
    connect(chatModes, SIGNAL(secretStateChanged(bool)),
            SLOT(secretStateChanged(bool)));
    connect(chatModes, SIGNAL(inviteOnlyStateChanged(bool)),
            SLOT(inviteOnlyStateChanged(bool)));
    connect(chatModes, SIGNAL(privateStateChanged(bool)),
            SLOT(privateStateChanged(bool)));
    connect(chatModes, SIGNAL(moderatedStateChanged(bool)),
            SLOT(moderatedStateChanged(bool)));
    connect(channelSearch, SIGNAL(findNextClicked()),
            SLOT(findNextClicked()));
    connect(channelSearch, SIGNAL(findPreviousClicked()),
            SLOT(findPreviousClicked()));
    connect(channelSearch, SIGNAL(textEdited(QString)),
            SLOT(channelSearchTextEdited(QString)));
    connect(channelSearch->optionButton(), SIGNAL(clicked(bool)),
            SLOT(optionButtonClicked()));
    connect(chatOutput, SIGNAL(userUrlClicked(QString)),
            SLOT(userUrlClicked(QString)));
    connect(userList, SIGNAL(clicked(QModelIndex)),
            SLOT(userClicked(QModelIndex)));
    connect(userList, SIGNAL(pressed(QModelIndex)),
            SLOT(userPressed(QModelIndex)));
    connect(chatOutput, SIGNAL(findTextTriggered()),
            SLOT(findTextTriggered()));
    connect(chatOutput, SIGNAL(channelTopicHistoryTriggered()),
            SLOT(channelTopicHistoryTriggered()));
    connect(d->topicDialog, SIGNAL(changedTopic(QString)),
            SLOT(changedTopic(QString)));

    socket->rfcMode(name.toLower());
    socket->rfcWho(name.toLower());

    if (Aki::Settings::enableWhoLookup()) {
        connect(d->whoTimer, SIGNAL(timeout()),
                SLOT(whoTimerTimeout()));
        d->whoTimer->start(Aki::Settings::updateInterval() * 1000);
    }
}

ChannelWindow::~ChannelWindow()
{
}

Aki::IdentityConfig*
ChannelWindow::identity()
{
    return d->identity;
}

void
ChannelWindow::setNickList(const QStringList &nickList)
{
    nickSelector->clear();
    nickSelector->addItems(nickList);
    nickSelector->setCurrentIndex(nickSelector->findText(socket()->currentNick(), Qt::MatchExactly));
}

Aki::ChannelModeBar*
ChannelWindow::modeBar()
{
    return chatModes;
}

void
ChannelWindow::addUser(Aki::Irc::User *user)
{
    if (user->nick() == socket()->currentNick()) {
        if (user->isOp()) {
            d->topicDialog->setTopicEditRights(true);
        } else if (!user->isOp() && !modeBar()->isTopicProtectionEnabled()) {
            d->topicDialog->setTopicEditRights(true);
        } else if (!user->isOp() && modeBar()->isTopicProtectionEnabled()) {
            d->topicDialog->setTopicEditRights(false);
        }
    }
    userList->addUser(user);
}

void
ChannelWindow::removeUser(Aki::Irc::User *user)
{
    userList->removeUser(user);
}

void
ChannelWindow::setTopic(const QString &topic)
{
    chatTopic->setText(topic);
    chatTopic->setToolTip("<p>" + Aki::Irc::Color::toHtml(topic) + "</p>");
}

QList<Aki::Irc::User*>
ChannelWindow::users() const
{
    return userList->users();
}

QList<Aki::Irc::User*>
ChannelWindow::users()
{
    return userList->users();
}

void
ChannelWindow::addMode(const QString &nick, const QString &mode)
{
    QModelIndex index;
    Aki::Irc::User *user = 0;

    for(int i = 0; i < userList->count(); ++i) {
        index = userList->model()->index(i, 0);
        user = userList->model()->data(index, Aki::NickListModel::IrcUserRole)
                                        .value<Aki::Irc::User*>();
        if (user->nick() == nick) {
            d->addMode(user, index, mode);
            return;
        }
    }
}

void
ChannelWindow::removeMode(const QString &nick, const QString &mode)
{
    QModelIndex index;
    Aki::Irc::User *user = 0;

    for(int i = 0; i < userList->count(); ++i) {
        index = userList->model()->index(i, 0);
        user = userList->model()->data(index, Aki::NickListModel::IrcUserRole)
                                        .value<Aki::Irc::User*>();
        if (user->nick() == nick) {
            d->removeMode(user, index, mode);
            return;
        }
    }
}

void
ChannelWindow::addMessage(const QString &from, const QString &message)
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
    
    foreach (Aki::Irc::User *user, userList->users()) {
        if (user->nick() == from) {
            user->setIdleTime(KDateTime::currentLocalDateTime());

            QString match = "\\s*" + QRegExp::escape(socket()->currentNick()) + ":*\\s*";
            QString colour = QString("<font color='%1'>%2</font>")
                                    .arg(user->color().name(), user->nick());

            if (msg.contains(QRegExp(match))) {
                view()->addPrivmsgHighlight(colour, Aki::Irc::Color::toHtml(msg));
                if (!isCurrent()) {
                    if (notifications()) {
                        notifications()->highlight(from, msg);
                    }
                    setTabColor(Highlight);
                }
                return;
            } else {
                view()->addPrivmsg(colour, Aki::Irc::Color::toHtml(msg));
                if (!isCurrent()) {
                    setTabColor(NewMessage);
                }
                return;
            }
        }
    }
}

void
ChannelWindow::setCurrent(bool current)
{
    Aki::BaseWindow::setCurrent(current);
    if (isCurrent()) {
        chatInput->setFocus();
        setTabColor(Default);
    }
}

void
ChannelWindow::setIsWhoRunning(bool running)
{
    d->isWhoRunning = running;
}

void
ChannelWindow::addWho(const QString &channel, const QString &identName, const QString &address,
                      const QString &server, const QString &nick, const QString &flags,
                      int hops, const QString &realName)
{
    Q_UNUSED(channel);
    Q_UNUSED(server);
    Q_UNUSED(hops);

    QModelIndex index;
    Aki::Irc::User *user = 0;

    for(int i = 0; i < userList->count(); ++i) {
        index = userList->model()->index(i, 0);
        user = userList->model()->data(index, Aki::NickListModel::IrcUserRole)
                                        .value<Aki::Irc::User*>();
        if (user->nick() == nick) {
            user->setUser(identName);
            user->setNick(nick);
            user->setHost(address);
            user->setRealName(realName);

            foreach (QChar c, flags) {
                if (c == QChar('@')) {
                    user->setModes(QChar('o'));
                } else if (c == QChar('+')) {
                    user->setModes(QChar('v'));
                } else if (c == QChar('H')) {
                    user->setAway(false);
                } else if (c == QChar('G')) {
                    user->setAway(true);
                } else {
                    kDebug() << "Unsupported flag: " << c;
                }
            }

            userList->model()->setData(index, QVariant::fromValue<Aki::Irc::User*>(user),
                                       Aki::NickListModel::IrcUserRole);
            return;
        }
    }
}

void
ChannelWindow::resetWho(bool join)
{
    if (join) {
        emit whoAdded(name().toLower());
    }

    if (Aki::Settings::enableWhoLookup()) {
        d->whoTimer->stop();
        d->whoTimer->start(Aki::Settings::updateInterval() * 1000);
    } else {
        d->whoTimer->stop();
    }
}

void
ChannelWindow::addNick(const QString &oldNick, const QString &newNick)
{
    QModelIndex index;
    Aki::Irc::User *user = 0;

    for(int i = 0; i < userList->count(); ++i) {
        index = userList->model()->index(i, 0);
        user = userList->model()->data(index, Aki::NickListModel::IrcUserRole)
                                    .value<Aki::Irc::User*>();

        if (user->nick() == oldNick) {
            if (oldNick == socket()->currentNick() || newNick == socket()->currentNick()) {
                socket()->appendNick(newNick);

                if (!nickSelector->contains(newNick)) {
                    nickSelector->addItem(newNick);
                }

                const int index = nickSelector->findText(newNick, Qt::MatchExactly);
                nickSelector->setCurrentIndex(index);
            }

            user->setNick(newNick);
            userList->model()->setData(index, QVariant::fromValue<Aki::Irc::User*>(user),
                                   Aki::NickListModel::IrcUserRole);
            userList->update(index);

            if (!Aki::Settings::conferenceMode()) {
                view()->addNick(oldNick, newNick);
            }

            if (!isCurrent()) {
                setTabColor(NewData);
            }
            return;
        }
    }
}

bool
ChannelWindow::hasInputFocus() const
{
    return chatInput->hasFocus();
}

void
ChannelWindow::setNewNick(const QString &nick)
{
    const int index = nickSelector->findText(nick, Qt::MatchExactly);
    nickSelector->setCurrentIndex(index);
}

void
ChannelWindow::showEvent(QShowEvent *event)
{
    KSharedConfigPtr self = KGlobal::config();
    KConfigGroup splitSize;
    QList<int> sizes;

    if (self->hasGroup("SplitSize")) {
        splitSize = KConfigGroup(self, "SplitSize");
        sizes = splitSize.readEntry("size", QList<int>());
    } else {
        sizes << (width() -  userList->width())
              << (fontMetrics().averageCharWidth() * 17 + 30);
        splitSize = KConfigGroup(self, "SplitSize");
        splitSize.writeEntry("size", sizes);
    }

    splitter->setSizes(sizes);
    Aki::BaseWindow::showEvent(event);
}

Aki::SearchBar*
ChannelWindow::searchBar()
{
    return channelSearch;
}

void
ChannelWindow::clearUserList()
{
}

void
ChannelWindow::populateUserList()
{
}

void
ChannelWindow::addTopicHistory(const QString &nickname, const QString &topic)
{
    d->topicDialog->addTopicHistoryEntry(nickname, topic);
}

#include "channelwindow.moc"
