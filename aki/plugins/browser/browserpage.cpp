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

#include "browserpage.h"
#include "browser.h"
#include <KLocale>
#include <KUrl>

BrowserPage::BrowserPage(QWidget *parent)
    : Aki::BaseWindow(i18n("Aki Browser"), Aki::BaseWindow::OtherWindow, parent)
{
    setupUi(this);

    back->setIcon(KIcon("go-previous"));
    forward->setIcon(KIcon("go-next"));
    reload->setIcon(KIcon("view-refresh"));
    stop->setIcon(KIcon("window-close"));
    homePage->setIcon(KIcon("go-home"));
    
    Browser *homePage = new Browser(this);
    homePage->load(KUrl("http://www.kde.org/"));
    browserView->addTab(homePage, homePage->title());

    connect(addressBar, SIGNAL(returnPressed(QString)),
            SLOT(slotAddressBarReturnPressed(QString)));
}

BrowserPage::~BrowserPage()
{
}

bool
BrowserPage::hasInputFocus() const
{
    return false;
}

void
BrowserPage::slotAddressBarReturnPressed(const QString &url)
{
    QWebView *view = qobject_cast<QWebView*>(browserView->currentWidget());
    if (view) {
        view->load(KUrl(url));
        browserView->setTabText(browserView->indexOf(view), view->title());
        browserView->setTabIcon(browserView->indexOf(view), view->icon());
    }
}
