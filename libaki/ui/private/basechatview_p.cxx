/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "basechatview_p.hpp"
#include "ui/basechatview.hpp"
#include <KDE/KAction>
#include <KDE/KApplication>
#include <KDE/KFileDialog>
#include <KDE/KIO/Job>
#include <KDE/KToolInvocation>
#include <KDE/KUser>
#include <QtGui/QClipboard>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebFrame>
using namespace Aki;

BaseChatViewPrivate::BaseChatViewPrivate(Aki::BaseChatView* qq)
    : _q(qq)
{
}

void
BaseChatViewPrivate::copyTriggered()
{
    QClipboard* cb = KApplication::clipboard();
    cb->setText(_q->page()->selectedText(), QClipboard::Clipboard);
}

void
BaseChatViewPrivate::copyUrlTriggered()
{
    KUser user(KUser::UseEffectiveUID);
    const QString fileName = KFileDialog::getSaveFileName(user.homeDir(), QString(), _q,
                                                          i18n("Save As..."),
                                                          KFileDialog::ConfirmOverwrite);
    KIO::file_copy(_q->page()->selectedText(), fileName);
}

void
BaseChatViewPrivate::customContextMenuRequested(const QPoint& pos)
{
    QWebHitTestResult result = mainFrame()->hitTestContent(pos);
    if (result.isNull()) {
        return;
    }

    if (result.linkElement().tagName() == "A" and result.linkElement().hasAttribute("href")) {
        KUrl url(result.linkUrl());
        QString href = url.url().remove("about:blank");

        if (href[0] == '#') {
            emit _q->userUrlClicked(href.remove('#'));
        } else {
            _q->linksMenu()->exec(QCursor::pos());
        }
    } else {
        _q->viewMenu()->exec(QCursor::pos());
    }
}

void
BaseChatViewPrivate::findTextTriggered()
{
    emit _q->findTextTriggered();
}

QWebFrame*
BaseChatViewPrivate::mainFrame()
{
    return _q->page()->mainFrame();
}

void
BaseChatViewPrivate::linkClicked(const QUrl& url)
{
    KToolInvocation::invokeBrowser(url.toString(QUrl::None));
}

QString
BaseChatViewPrivate::parseEmotions(const QString& message, KEmoticonsTheme::ParseMode mode,
                                   const QStringList& excludes) const
{
    /* Exact code from parseEmoticons in parseEmoticons()
     * Just added function to add file:// to the path because WebKit won't
     * load them without it.
     */
    QList<KEmoticonsTheme::Token> tokens =
        emotTheme.tokenize(message, mode | KEmoticonsTheme::SkipHTML);
    if (tokens.isEmpty() and !message.isEmpty()) {
        return message;
    }

    QString result;

    foreach (const KEmoticonsTheme::Token& token, tokens) {
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

    result.replace("src=\"", "src\"file://");
    return result;
}

void
BaseChatViewPrivate::saveAsTriggered()
{
}

void
BaseChatViewPrivate::setupWebkitSettings()
{
    QWebSettings* settings = _q->settings();
    settings->setAttribute(QWebSettings::AcceleratedCompositingEnabled, true);
    settings->setAttribute(QWebSettings::AutoLoadImages, true);
    settings->setAttribute(QWebSettings::FrameFlatteningEnabled, false);
    settings->setAttribute(QWebSettings::JavaEnabled, false);
    settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
    settings->setAttribute(QWebSettings::PluginsEnabled, true);
    settings->setAttribute(QWebSettings::PrintElementBackgrounds, true);
    settings->setDefaultTextEncoding("UTF-8");
}

void
BaseChatViewPrivate::topicHistoryTriggered()
{
}
