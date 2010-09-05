#include "application.hpp"
#include "mainwindow.hpp"
#include <KDE/KCmdLineArgs>
#include <KDE/KGlobal>
#include <KDE/KStandardDirs>
#include <KDE/KWindowSystem>
using namespace Aki;

Application::Application()
    : KUniqueApplication(),
    _mainWindow(0)
{
}

Application::~Application()
{
}

int Application::newInstance()
{
    KCmdLineArgs* args = KCmdLineArgs::parsedArgs();

    if (!_mainWindow) {
        const QString db = KGlobal::dirs()->locate("data", "aki/akidatabase.db");
        if (db.isEmpty() || db.isNull()) {
            // Welcome dialog
        }

        _mainWindow = new Aki::AkiWindow;
        _mainWindow->show();
    }

    KWindowSystem::forceActiveWindow(_mainWindow->winId());
    args->clear();

    return 0;
}
