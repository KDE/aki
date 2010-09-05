#include "dockwidget.hpp"
#include "ui/dockbar.hpp"
#include "ui/dockbutton.hpp"
#include "ui/private/dockwidget_p.hpp"
#include <KDE/KIcon>
#include <QtCore/QEvent>
using namespace Aki;

DockWidget::DockWidget(QWidget* parent)
    : QDockWidget(parent)
{
    _d.reset(new Aki::DockWidgetPrivate(this));
    _d->initialise();
    setIcon(KIcon("aki")),
    setTitle(QString());
}

DockWidget::DockWidget(const QString& title, QWidget* parent)
    : QDockWidget(title, parent)
{
    _d.reset(new Aki::DockWidgetPrivate(this));
    _d->initialise();
    setIcon(KIcon("aki"));
    setTitle(title);
}

DockWidget::DockWidget(const QString& title, const KIcon& icon, QWidget* parent)
    : QDockWidget(title, parent)
{
    _d.reset(new Aki::DockWidgetPrivate(this));
    _d->initialise();
    setIcon(icon);
    setTitle(title);
}

DockWidget::~DockWidget()
{

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
DockWidget::leaveEvent(QEvent*)
{
    if (!_d->dockButton) {
        _d->dockButton = _d->dockBar->buttonForDockWidget(this);
    }

    if (_d->dockButton && _d->dockButton->isAutoHide()) {
        emit mouseLeave();
    }
}

void
DockWidget::setDockBar(DockBar* dockBar)
{
    Q_ASSERT(dockBar);
    _d->dockBar = dockBar;
}

void
DockWidget::setIcon(const KIcon& icon)
{
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

#include "ui/dockwidget.moc"
