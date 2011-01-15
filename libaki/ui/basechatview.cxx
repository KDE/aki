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

#include "basechatview.hpp"
#include "private/basechatview_p.hpp"
#include <KDE/KAction>
#include <KDE/KIcon>
#include <KDE/KMenu>
#include <QtWebKit/QWebFrame>
using namespace Aki;

BaseChatView::BaseChatView(QWidget* parent)
    : KWebView(parent)
{
    _d.reset(new Aki::BaseChatViewPrivate(this));
    _d->setupWebkitSettings();

    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    setContextMenuPolicy(Qt::CustomContextMenu);
    
    connect(page(), SIGNAL(linkClicked(QUrl)),
            SLOT(linkClicked(QUrl)));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customContextMenuRequested(QPoint)));
}

BaseChatView::~BaseChatView()
{
}

KMenu*
BaseChatView::linksMenu()
{
    KMenu* menu = new KMenu(i18n("Links Menu"), this);

    KAction* action = new KAction(KIcon("edit-copy"), i18n("Copy Link Address"), menu);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(copyUrlTriggered()));
    menu->addAction(action);

    action = new KAction(KIcon("document-save-as"), i18n("Save Link As..."), menu);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(saveAsTriggered()));
    menu->addAction(action);

    return menu;
}

QString
BaseChatView::parseEmotions(const QString& text) const
{
    return _d->parseEmotions(text, KEmoticonsTheme::StrictParse);
}

KMenu*
BaseChatView::viewMenu()
{
    KMenu* menu = new KMenu(this);

    KAction* action = new KAction(KIcon("edit-copy"), i18n("Copy"), menu);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(copyTriggered()));
    menu->addAction(action);

    action = new KAction(KIcon("edit-find"), i18n("Find Text..."), menu);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(findTextTriggered()));
    menu->addAction(action);
    menu->addSeparator();

    action = new KAction(i18n("Channel Topic History..."),
                         menu);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(topicHistoryTriggered()));
    menu->addAction(action);

    return menu;
}

#include "ui/basechatview.moc"
