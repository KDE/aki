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

#include "welcomedialog.h"
#include "dialogs/identitywelcomepage.h"
#include "dialogs/introwelcomepage.h"
#include "dialogs/serverwelcomepage.h"
#include <KLocale>

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : KAssistantDialog(parent)
{
    showButton(Help, false);
    setWindowTitle(i18n("Aki IRC Setup"));

    m_introPage = addPage(new IntroWelcomePage, i18n("Welcome to Aki"));
    m_identityPage = addPage(new IdentityWelcomePage, i18n("Setup Identity"));
    m_serverPage = addPage(new ServerWelcomePage, i18n("Setup Server"));

    connect(this, SIGNAL(user1Clicked()),
            SLOT(finishClicked()));
}

WelcomeDialog::~WelcomeDialog()
{
}

void
WelcomeDialog::finishClicked()
{
    qobject_cast<IdentityWelcomePage*>(m_identityPage->widget())->save();
    qobject_cast<ServerWelcomePage*>(m_serverPage->widget())->save();
}
