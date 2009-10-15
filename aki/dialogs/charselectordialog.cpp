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

class CharSelectorDialogPrivate
{
public:
    CharSelectorDialog instance;
}; // End of class CharSelectorDialogPrivate.

K_GLOBAL_STATIC(CharSelectorDialogPrivate, charSelector)

CharSelectorDialog::CharSelectorDialog()
    : KDialog(0)
{
    setButtons(User1 | Close);
    setButtonIcon(User1, KIcon("dialog-ok"));
    setButtonText(User1, i18n("Insert"));
    m_charPicker = new KCharSelect(this, 0, KCharSelect::SearchLine |
                                   KCharSelect::FontCombo | KCharSelect::BlockCombos |
                                   KCharSelect::CharacterTable);
    QFont font = m_charPicker->currentFont();
    font.setPointSize(12);
    m_charPicker->setCurrentFont(font);
    setMainWidget(m_charPicker);

    connect(this, SIGNAL(user1Clicked()), SLOT(slotUser1Clicked()));
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

void
CharSelectorDialog::slotUser1Clicked()
{
    emit charSelected(m_charPicker->currentChar());
}
