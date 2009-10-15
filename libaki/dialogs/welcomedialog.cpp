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
using namespace Aki;

namespace Aki
{
class WelcomeDialogPrivate
{
public:
    WelcomeDialogPrivate(Aki::WelcomeDialog *qq)
        : q(qq),
        introPage(0),
        identityPage(0),
        serverPage(0)
    {
    }

    void finishClicked()
    {
        qobject_cast<Aki::IdentityWelcomePage*>(identityPage->widget())->save();
        qobject_cast<Aki::ServerWelcomePage*>(serverPage->widget())->save();
    }

    Aki::WelcomeDialog *q;
    KPageWidgetItem *introPage;
    KPageWidgetItem *identityPage;
    KPageWidgetItem *serverPage;
}; // End of class WelcomeDialogPrivate.
} // End of namespace Aki.

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : KAssistantDialog(parent)
{
    d.reset(new WelcomeDialogPrivate(this));
    showButton(Help, false);
    setWindowTitle(i18n("Aki IRC Setup"));

    d->introPage = addPage(new Aki::IntroWelcomePage, i18n("Welcome to Aki"));
    d->identityPage = addPage(new Aki::IdentityWelcomePage, i18n("Setup Identity"));
    d->serverPage = addPage(new Aki::ServerWelcomePage, i18n("Setup Server"));

    connect(this, SIGNAL(user1Clicked()),
            SLOT(finishClicked()));
}

WelcomeDialog::~WelcomeDialog()
{
}

#include "welcomedialog.moc"
