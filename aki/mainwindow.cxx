#include "mainwindow.hpp"
#include "aki.hpp"
#include "dialogs/serverdialog.hpp"
#include "interfaces/ibasewindow.hpp"
#include "ui/chatview.hpp"
#include "ui/view.hpp"
#include "ui/viewtabbar.hpp"
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KCmdLineArgs>
#include <KDE/KDebug>
#include <KDE/KLocale>
#include <KDE/KMenuBar>
#include <KDE/KMessageBox>
#include <KDE/KTabBar>
#include <QtGui/QVBoxLayout>
#include <QWebFrame>
#include <QWebElement>
#include <QNetworkRequest>
using namespace Aki;

AkiWindow::AkiWindow()
    : Aki::MainWindow()
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    args->clear();
    setTopRightCorner(new Aki::ViewTabBar(this));
    setView(new Aki::View);
    createMenus();
    createDialogs();
}

AkiWindow::~AkiWindow()
{
}

void
AkiWindow::createMenus()
{
    KAction* action = new KAction(i18n("Network List"), this);
    actionCollection()->addAction("akiNetworkList", action);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(serverListTriggered()));

    action = new KAction(i18n("Server Tab"), this);
    actionCollection()->addAction("akiNewServerTab", action);

    action = new KAction(i18n("Channel Tab"), this);
    actionCollection()->addAction("akiNewChannelTab", action);

    action = new KAction(i18n("Quick Connection"), this);
    actionCollection()->addAction("akiQuickConnection", action);

    action = new KAction(KIcon("edit-redo"), i18n("Reconnect"), this);
    actionCollection()->addAction("akiReconnect", action);

    action = new KAction(KIcon("edit-delete"), i18n("Disconnect"), this);
    actionCollection()->addAction("akiDisconnect", action);

    action = new KAction(KIcon("application-exit"), i18n("Quit"), this);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(quitTriggered()));
    actionCollection()->addAction("akiQuit", action);
    setupGUI(ToolBar | Keys | StatusBar | Create);
}

void
AkiWindow::createDialogs()
{
}

void
AkiWindow::serverListTriggered()
{
    Aki::ServerDialog* serverDialog = new Aki::ServerDialog;
    serverDialog->show();
}

void
AkiWindow::quitTriggered()
{
}
