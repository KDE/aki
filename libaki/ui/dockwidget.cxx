#include "dockwidget.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "private/dockwidget_p.hpp"
using namespace Aki;

DockWidget::DockWidget(QWidget* parent)
    : QDockWidget(parent)
{
    _d.reset(new Aki::DockWidgetPrivate(this));
    _d->setupActions();
    setContextMenuPolicy(Qt::CustomContextMenu);
}

DockWidget::DockWidget(const QString& title, const KIcon& icon, QWidget* parent)
    : QDockWidget(title, parent)
{
    _d.reset(new Aki::DockWidgetPrivate(this));
    _d->setupActions();
    setIcon(icon);
    setTitle(title);
    setContextMenuPolicy(Qt::CustomContextMenu);
}

DockWidget::~DockWidget()
{
}

Aki::DockBar*
DockWidget::dockBar()
{
    return _d->dockBar;
}

Aki::DockBar*
DockWidget::dockBar() const
{
    return _d->dockBar;
}

KIcon
DockWidget::icon() const
{
    return _d->icon;
}

void
DockWidget::setDockBar(Aki::DockBar* dock)
{
    Q_ASSERT(dock);
    _d->dockBar = dock;
}

void
DockWidget::setIcon(const KIcon& icon)
{
    if (icon.isNull()) {
        return;
    }

    _d->icon = icon;
}

void
DockWidget::setTitle(const QString& title)
{
    setObjectName(title);
}

QString
DockWidget::title() const
{
    return objectName();
}

void DockWidget::leaveEvent(QEvent*)
{
    if (!_d->dockButton) {
        _d->dockButton = _d->dockBar->buttonForDock(this);
    }

    if (_d->dockButton && _d->dockButton->isAutoHide()) {
        emit mouseLeave();
    }
}
