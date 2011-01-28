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

#include "welcomedialog.hpp"
#include "aki.hpp"
#include "sql/database.hpp"
#include "welcomedialogpages/identitywelcomepage.hpp"
#include "welcomedialogpages/introwelcomepage.hpp"
#include "welcomedialogpages/serverwelcomepage.hpp"
#include <KDE/KMessageBox>
using namespace Aki;

WelcomeDialog::WelcomeDialog(QWidget* parent)
    : KAssistantDialog(parent)
{
    _database = new Aki::Sql::Database("QSQLITE");
    //_database->setDatabaseName(Aki::databaseFile());
    if (!_database->open()) {
        KMessageBox::error(this, i18n("Unable to open database file.\nSettings will not be saved."),
                           i18n("Unable to open database."));
    }

    showButton(KAssistantDialog::Help, false);
    setCaption(i18n("IRC Setup"));

    _introPage = addPage(new IntroWelcomePage, i18n("Welcome to Aki"));
    _identityPage = addPage(new IdentityWelcomePage(_database), i18n("Setup Identity"));
    _serverPage = addPage(new ServerWelcomePage(_database), i18n("Setup Server"));

    if (_database->isOpen()) {
        connect(this, SIGNAL(user1Clicked()),
                SLOT(finishClicked()));
    }
}

WelcomeDialog::~WelcomeDialog()
{
    _database->close();
    delete _database;
}

void
WelcomeDialog::finishClicked()
{
    //qobject_cast<Aki::ServerWelcomePage*>(_serverPage->widget())->save();
    //qobject_cast<Aki::IdentityWelcomePage*>(_serverPage->widget())->save();
}
