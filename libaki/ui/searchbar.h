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

#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include "ui_searchbar.h"
#include <QScopedPointer>
#include <QWidget>

namespace Aki
{
class SearchBarPrivate;
class SearchBar : public QWidget,
                  private Ui::SearchBar
{
    Q_OBJECT
public:
    SearchBar(QWidget *parent = 0);
    ~SearchBar();
    QString text() const;
    void setFound(bool state);
    void resetPalette();
    bool isCaseSensitive() const;
    bool isHighlightAll() const;
    bool isWrapAround() const;
    QToolButton* optionButton() const;
Q_SIGNALS:
    void findNextClicked();
    void findPreviousClicked();
    void textEdited(const QString &text);
private:
    Q_PRIVATE_SLOT(d, void closeButtonClicked())
    Q_PRIVATE_SLOT(d, void findNextButtonClicked())
    Q_PRIVATE_SLOT(d, void findPreviousButtonClicked())
    Q_PRIVATE_SLOT(d, void searchTermsTextEdited(const QString &text))
    Q_PRIVATE_SLOT(d, void caseSensitiveTriggered(bool checked))
    Q_PRIVATE_SLOT(d, void highlightAllTriggered(bool checked))
    Q_PRIVATE_SLOT(d, void wrapAroundTriggered(bool checked))
private:
    friend class SearchBarPrivate;
    QScopedPointer<SearchBarPrivate> d;
}; // End of class SearchBar.
} // End of namespace Aki.

#endif // SEARCHBAR_H
