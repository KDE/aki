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

#include "importdialog.hpp"
#include "aki.hpp"
#include "dialogs/importdialogpages/importakipage.hpp"
#include "dialogs/importdialogpages/importidentitypage.hpp"
#include "dialogs/importdialogpages/importstartpage.hpp"
#include "dialogs/importdialogpages/importxchatpage.hpp"
#include <KMessageBox>
using namespace Aki;

ImportDialog::ImportDialog(QWidget* parent)
    : KAssistantDialog(parent)
{
    setCaption(i18n("Import Assistant"));

    Aki::ImportIdentityPage* identityPage = new Aki::ImportIdentityPage(0);
    _identityPage = addPage(identityPage, i18n("Setup Identities"));
    _identityPage->setIcon(KIcon("aki"));

    Aki::ImportStartPage* startPage = new Aki::ImportStartPage;
    _startPage = addPage(startPage, i18n("Import Settings and Data"));
    _startPage->setIcon(KIcon("aki"));
    connect(startPage, SIGNAL(importAkiToggled()),
            SLOT(slotImportAkiPageToggled()));
    connect(startPage, SIGNAL(importDontImportAnythingToggled()),
            SLOT(slotImportDontImportAnythingPageToggled()));
    connect(startPage, SIGNAL(importXChatToggled()),
            SLOT(slotImportXChatPageToggled()));

    Aki::ImportAkiPage* akiPage = new Aki::ImportAkiPage;
    _akiPage = addPage(akiPage, i18n("Import Aki Settings"));
    _akiPage->setIcon(KIcon("aki"));

    Aki::ImportXChatPage* xchatPage = new Aki::ImportXChatPage;
    _xchatPage = addPage(xchatPage, i18n("Import XChat Settings"));
    _xchatPage->setIcon(KIcon("aki"));

    connect(this, SIGNAL(finished()),
            SLOT(slotFinished()));
}

ImportDialog::~ImportDialog()
{
}

void
ImportDialog::slotFinished()
{
}

void
ImportDialog::slotImportAkiPageToggled()
{
    setAppropriate(_akiPage, true);
    setAppropriate(_startPage, false);
    setAppropriate(_xchatPage, false);
}

void
ImportDialog::slotImportDontImportAnythingPageToggled()
{
    setAppropriate(_akiPage, false);
    setAppropriate(_startPage, false);
    setAppropriate(_xchatPage, false);
}

void
ImportDialog::slotImportXChatPageToggled()
{
    setAppropriate(_akiPage, false);
    setAppropriate(_startPage, false);
    setAppropriate(_xchatPage, true);
}