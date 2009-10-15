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

#include "akiapplication.h"
#include "akiversion.h"
#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>
#include <cstdlib>

int
main(int argc, char **argv)
{
    KAboutData aboutData("aki", 0, ki18n("Aki"), AKI_VERSION_STR,
                         ki18n("Extensive IRC client for KDE4."), KAboutData::License_LGPL_V2,
                         ki18n("Copyright (c) 2009 Keith Rusler"),
                         ki18n("IRC: irc.freenode.net - #aki\n"
                               "Build Date: %1 at %2").subs(__DATE__).subs(__TIME__));
    aboutData.addAuthor(ki18n("Keith Rusler"), ki18n("Lead Developer"), "xzekecomax@gmail.com");
    aboutData.addCredit(ki18n("Lee Olson"), ki18n("Aki Icon Author"), "leetolson@gmail.com");

    KCmdLineArgs::init(argc, argv, &aboutData);
    KCmdLineOptions options;
    options.add("server <address>", ki18n("Server address to connect"), "");
    options.add("port <port>", ki18n("Server port to use"), "6667");
    options.add("nick <nickname>", ki18n("Nickname to use"), "");
    options.add("pass <password>", ki18n("Password to connect"), "");
    options.add("ssl", ki18n("Connect using SSL"), "false");
    options.add("noplugins", ki18n("Do not load plugins"), "");
    options.add("noconnect", ki18n("Disables auto-connection"), "");

    KCmdLineArgs::addCmdLineOptions(options);
    KUniqueApplication::addCmdLineOptions();
    if (!AkiApplication::start()) {
        exit(0);
    }

    AkiApplication app;
    return app.exec();
}
