#include "viewtabbar_p.hpp"
#include "ui/viewtabbar.hpp"
#include <KDE/KDebug>
#include <KDE/KIcon>
#include <KDE/KLocale>
#include <QtGui/QAction>
#include <QtGui/QMenu>
using namespace Aki;

ViewTabBarPrivate::ViewTabBarPrivate(ViewTabBar* qq)
    : QObject(),
    _q(qq)
{
    _q->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_q, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customContextMenuRequested()));
}

void
ViewTabBarPrivate::customContextMenuRequested()
{
    if (!_q->createPopupMenu()) {
        return;
    }

    contextMenuPosition = QCursor::pos();
    _q->createPopupMenu()->exec(contextMenuPosition);
}

void
ViewTabBarPrivate::setupContextActions()
{
    QAction* action = new QAction(KIcon("edit-redo"), i18n("Reconnect"), contextMenu);
    contextMenu->addAction(action);

    action = new QAction(KIcon("edit-delete"), i18n("Disconnect"), contextMenu);
    contextMenu->addAction(action);

    contextMenu->addSeparator();

    action = new QAction(KIcon(""), i18n("Close Tab"), contextMenu);
    connect(action, SIGNAL(triggered(bool)),
            SLOT(closeTabTriggered()));
    contextMenu->addAction(action);
}

void
ViewTabBarPrivate::closeTabTriggered()
{
    contextMenuPosition = _q->mapFromGlobal(contextMenuPosition);
    _q->removeTab(_q->tabAt(contextMenuPosition));
    contextMenuPosition = QPoint();
}
