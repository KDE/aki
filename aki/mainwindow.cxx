#include "mainwindow.hpp"
#include "dialogs/serverdialog.hpp"
#include "interfaces/ibasewindow.hpp"
#include "ui/chatview.hpp"
#include "ui/view.hpp"
#include "ui/viewtabbar.hpp"
#include <KDE/KAction>
#include <KDE/KActionCollection>
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
    actionCollection()->addAction(QLatin1String("akiNetworkList"), action);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(serverListTriggered()));

    action = new KAction(i18n("Server Tab"), this);
    actionCollection()->addAction(QLatin1String("akiNewServerTab"), action);

    action = new KAction(i18n("Channel Tab"), this);
    actionCollection()->addAction(QLatin1String("akiNewChannelTab"), action);

    action = new KAction(i18n("Quick Connection"), this);
    actionCollection()->addAction(QLatin1String("akiQuickConnection"), action);

    action = new KAction(KIcon(QLatin1String("edit-redo")), i18n("Reconnect"), this);
    actionCollection()->addAction(QLatin1String("akiReconnect"), action);

    action = new KAction(KIcon(QLatin1String("edit-delete")), i18n("Disconnect"), this);
    actionCollection()->addAction(QLatin1String("akiDisconnect"), action);

    action = new KAction(KIcon(QLatin1String("application-exit")), i18n("Quit"), this);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(quitTriggered()));
    actionCollection()->addAction(QLatin1String("akiQuit"), action);
    setupGUI(ToolBar | Keys | StatusBar | Create);
}

void
AkiWindow::createDialogs()
{
    _serverDialog = new Aki::ServerDialog(this);
}

void
AkiWindow::serverListTriggered()
{
    _serverDialog->show();
}

void
AkiWindow::quitTriggered()
{
}
