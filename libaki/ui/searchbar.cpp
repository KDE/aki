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

#include "searchbar.h"
#include <QAction>
#include <QMenu>
using namespace Aki;

namespace Aki
{
class SearchBarPrivate
{
public:
    SearchBarPrivate(Aki::SearchBar *qq)
        : q(qq),
        isCaseSensitive(false),
        isHighlightAll(false),
        isWrapAround(true)
    {
    }

    void closeButtonClicked()
    {
        q->hide();
    }

    void findNextButtonClicked()
    {
        emit q->findNextClicked();
    }

    void findPreviousButtonClicked()
    {
        emit q->findPreviousClicked();
    }

    void searchTermsTextEdited(const QString &text)
    {
        emit q->textEdited(text);

        if (text.isEmpty() || text.isNull()) {
            q->findNextButton->setDisabled(true);
            q->findPreviousButton->setDisabled(true);
        } else {
            q->findNextButton->setEnabled(true);
            q->findPreviousButton->setEnabled(true);
        }
    }

    void caseSensitiveTriggered(bool checked)
    {
        isCaseSensitive = checked;
    }

    void highlightAllTriggered(bool checked)
    {
        isHighlightAll = checked;
    }

    void wrapAroundTriggered(bool checked)
    {
        isWrapAround = checked;
    }

    Aki::SearchBar *q;
    QBrush baseColor;
    bool isCaseSensitive;
    bool isHighlightAll;
    bool isWrapAround;
}; // End of class SearchBarPrivate.
} // End of namespace Aki.

SearchBar::SearchBar(QWidget *parent)
    : QWidget(parent)
{
    d.reset(new Aki::SearchBarPrivate(this));
    setupUi(this);

    d->baseColor = searchTerms->palette().base();

    closeButton->setIcon(KIcon("dialog-close"));
    findNextButton->setIcon(KIcon("go-up"));
    findPreviousButton->setIcon(KIcon("go-down"));

    QMenu *menu = new QMenu(this);
    QAction *caseSensitive = new QAction(i18n("Case Sensitive"), menu);
    caseSensitive->setCheckable(true);
    caseSensitive->setChecked(false);
    connect(caseSensitive, SIGNAL(triggered(bool)),
            SLOT(caseSensitiveTriggered(bool)));

    QAction *highlightAll = new QAction(i18n("Highlight all"), menu);
    highlightAll->setCheckable(true);
    highlightAll->setChecked(false);
    connect(highlightAll, SIGNAL(triggered(bool)),
            SLOT(highlightAllTriggered(bool)));

    QAction *wrapAround = new QAction(i18n("Wrap around"), menu);
    wrapAround->setCheckable(true);
    wrapAround->setChecked(true);
    connect(wrapAround, SIGNAL(triggered(bool)),
            SLOT(wrapAroundTriggered(bool)));

    menu->addAction(caseSensitive);
    menu->addAction(highlightAll);
    menu->addAction(wrapAround);
    optionsButton->setMenu(menu);

    connect(closeButton, SIGNAL(clicked(bool)),
            SLOT(closeButtonClicked()));
    connect(findNextButton, SIGNAL(clicked(bool)),
            SLOT(findNextButtonClicked()));
    connect(findPreviousButton, SIGNAL(clicked(bool)),
            SLOT(findPreviousButtonClicked()));
    connect(searchTerms, SIGNAL(textEdited(QString)),
            SLOT(searchTermsTextEdited(QString)));
}

SearchBar::~SearchBar()
{
}

QString
SearchBar::text() const
{
    return searchTerms->text();
}

void
SearchBar::setFound(bool state)
{
    QPalette pal = searchTerms->palette();
    pal.setBrush(QPalette::Base, QBrush(state ? Qt::green : Qt::red));
    searchTerms->setPalette(pal);
}

void
SearchBar::resetPalette()
{
    QPalette pal = searchTerms->palette();
    pal.setBrush(QPalette::Base, d->baseColor);
    searchTerms->setPalette(pal);
}

bool
SearchBar::isCaseSensitive() const
{
    return d->isCaseSensitive;
}

bool
SearchBar::isHighlightAll() const
{
    return d->isHighlightAll;
}

bool
SearchBar::isWrapAround() const
{
    return d->isWrapAround;
}

#include "searchbar.moc"
