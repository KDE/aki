#include "akiversion.hpp"
#include "mainwindow.hpp"
#include <KDE/KAboutData>
#include <KDE/KApplication>
#include <KDE/KCmdLineArgs>
#include <KDE/KLocale>

int
main(int argc, char** argv)
{
    KAboutData aboutData("aki", 0, ki18n("Aki"), AKI_VERSION_STR,
                         ki18n("Extensive IRC client for KDE4."), KAboutData::License_GPL_V2,
                         ki18n("Copyright (c) 2009-2010 Keith Rusler"),
                         ki18n("IRC: irc.freenode.net - #akiirc\n"
                               "Build Data: %1 at %2").subs(__DATE__).subs(__TIME__));
    aboutData.addAuthor(ki18n("Keith Rusler"), ki18n("Lead Developer"), "xzekecomax@gmail.com");
    aboutData.addCredit(ki18n("Lee Olson"), ki18n("Aki Icon Author"), "leetolson@gmail.com");
    aboutData.addCredit(ki18n("Andreas Marschke"), ki18n("Bughunter, Testing and Design"), "xxtjaxx@gmail.com");

    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add("+[url]", ki18n("irc:// Server URL or hostname"));
    options.add("server <address>", ki18n("Server address to connect"), "");
    options.add("port <port>", ki18n("Server port to use"), "6667");
    options.add("nick <nickname>", ki18n("Nickname to use"), "");
    options.add("pass <password>", ki18n("Password to connect"), "");
    options.add("ssl", ki18n("Connect using SSL"), "false");
    options.add("noplugins", ki18n("Do not load plugins"), "");
    options.add("noconnect", ki18n("Disables auto-connection"), "");
    options.add("debug", ki18n("Enable debug mode"));
    options.add("log", ki18n("Log to file debug info"));

    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;
    Aki::AkiWindow *win = new Aki::AkiWindow;
    win->show();

    return KApplication::exec();
}
