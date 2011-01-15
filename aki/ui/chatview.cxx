/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
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

#include "chatview.hpp"
#include "debughelper.hpp"
#include "settings.h"
#include "utils/themestylemanager.hpp"
#include <QtWebKit/QWebFrame>
using namespace Aki;

ChatView::ChatView(QWidget* parent)
    : BaseChatView(parent)
{
    connect(Aki::Settings::self(), SIGNAL(themeChanged()),
            SLOT(slotThemeChanged()));
    connect(mainFrame(), SIGNAL(contentsSizeChanged(QSize)),
            SLOT(slotContentsSizeChanged(QSize)));

    setTheme(Aki::Settings::self()->themeListWidget());
    createTemplate();
}

ChatView::ChatView(Aki::Socket* socket, QWidget* parent)
    : Aki::BaseChatView(parent),
    _socket(socket),
    _isUserMoved(false)
{    
    connect(Aki::Settings::self(), SIGNAL(themeChanged()),
            SLOT(slotThemeChanged()));
    connect(mainFrame(), SIGNAL(contentsSizeChanged(QSize)),
            SLOT(slotContentsSizeChanged(QSize)));

    setTheme(Aki::Settings::self()->themeListWidget());
    createTemplate();
}

ChatView::~ChatView()
{
}

void
ChatView::appendMessage(const Aki::Irc::Message& message)
{
    _messages.append(message);
}

void
ChatView::changeTheme()
{
    createTemplate();

    foreach (const Aki::Irc::Message& message, _messages) {
        appendMessage(message);
    }
}

void
ChatView::createTemplate()
{
    DEBUG_FUNC_NAME
    if (!_currentTheme) {
        return;
    }

    setContent(QByteArray(), "application/xhtml+xml");
    DEBUG_TEXT("Cleared html");

    QString baseTemplate = 
        QString("<!DOCTYPE html PUBLIC &quot;-//W3C//DTD XHTML 1.0 Strict//EN&quot;"
        "&quot;http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd&quot;>"
        "<html>\n"
        "<head>\n"
        "<meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=UTF-8\" />\n"
        "    <title>Aki IRC Client</title>\n"
        "<base href=\"%1\">\n"
        "<style id=\"baseStyle\" type=\"text/css\" media=\"screen,print\">\n"
        "   @import url(\"main.css\");\n"
        "   *{ word-wrap:break-word; }\n"
        "</style>\n"
        "<style id=\"mainStyle\" type=\"text/css\" media=\"screen,print\">\n"
        "   @import url(\"%2\");\n"
        "   </style>\n"
        "</head>\n"
        "<body>\n"
        "   %3\n"
        "       <div id=\"Chat\">\n</div>\n"
        "   %4\n"
        "</body>\n"
        "</html>").arg(_currentTheme->baseHref())
        .arg(_currentTheme->variants()[0])
        .arg(_currentTheme->headerHtml())
        ,arg(_currentTheme->footerHtml());
    setContent(baseTemplate.toUtf8(), "application/xhtml+xml");
    DEBUG_TEXT2("Html contents\n%1", baseTemplate)
}

QString
ChatView::formatTheme(const QString& html)
{
    QString tempHtml = html;
    Q_UNUSED(tempHtml)
    return QString();
}

QString
ChatView::formatTheme(const QString& html, const Aki::Irc::Message& message)
{
    Q_UNUSED(html)
    Q_UNUSED(message)
    return QString();
}

QWebFrame*
ChatView::mainFrame()
{
    return page()->mainFrame();
}

void
ChatView::setTheme(const QString& style)
{
    setTheme(Aki::ThemeStyleManager::self()->themeForName(style));
}

void
ChatView::setTheme(Aki::ThemeStyle* style)
{
    _currentTheme = style;

    changeTheme();
}

void
ChatView::slotContentsSizeChanged(const QSize& size)
{
    if (!_isUserMoved) {
        mainFrame()->scroll(size.width(), size.height());
    }
}

void
ChatView::slotThemeChanged()
{
    setTheme(Aki::Settings::self()->themeListWidget());
}
