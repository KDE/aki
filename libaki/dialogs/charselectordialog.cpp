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

#include "charselectordialog.h"
#include <KCharSelect>
#include <KGlobal>
#include <KLocale>
#include <QCoreApplication>
using namespace Aki;

namespace Aki
{
class CharSelectorSingleton
{
public:
    Aki::CharSelectorDialog instance;
}; // End of class CharSelectorSingleton.

K_GLOBAL_STATIC(Aki::CharSelectorSingleton, charSelector)

class CharSelectorDialogPrivate
{
public:
    CharSelectorDialogPrivate(Aki::CharSelectorDialog *qq)
        : q(qq)
    {
    }

    void _user1Clicked()
    {
        if (charSelect->currentChar().isNull()) {
            return;
        }

        emit q->charSelected(charSelect->currentChar());
    }

    Aki::CharSelectorDialog *q;
    KCharSelect *charSelect;
}; // End of class CharSelectorDialogPrivate.
} // End of namespace Aki.

CharSelectorDialog::CharSelectorDialog()
    : KDialog()
{
    d.reset(new CharSelectorDialogPrivate(this));
    d->charSelect = new KCharSelect(this, 0, KCharSelect::SearchLine |
                                    KCharSelect::FontCombo | KCharSelect::BlockCombos |
                                    KCharSelect::CharacterTable);

    QFont font = d->charSelect->currentFont();
    font.setPointSize(12);
    d->charSelect->setCurrentFont(font);
    setMainWidget(d->charSelect);

    connect(d->charSelect, SIGNAL(charSelected(QChar)),
            this, SLOT(_user1Clicked()));
    qAddPostRoutine(charSelector.destroy);
}

CharSelectorDialog::~CharSelectorDialog()
{
    qRemovePostRoutine(charSelector.destroy);
}

CharSelectorDialog*
CharSelectorDialog::self()
{
    return &charSelector->instance;
}

#include "charselectordialog.moc"
