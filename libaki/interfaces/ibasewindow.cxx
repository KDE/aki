#include "ibasewindow.hpp"
#include "private/ibasewindow_p.hpp"
using namespace Aki;

IBaseWindow::IBaseWindow(QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::IBaseWindowPrivate(this));
}

IBaseWindow::IBaseWindow(const QString& title, QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::IBaseWindowPrivate(this));
    setTitle(title);
}

IBaseWindow::IBaseWindow(const KIcon& icon, const QString& title, QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::IBaseWindowPrivate(this));
    setIcon(icon);
    setTitle(title);
}

IBaseWindow::IBaseWindow(const QString& title, Aki::IBaseWindow::WindowType type,
                         QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::IBaseWindowPrivate(this));
    setTitle(title);
    setWindowType(type);
}

IBaseWindow::IBaseWindow(const KIcon& icon, const QString& title,
                         Aki::IBaseWindow::WindowType type, QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::IBaseWindowPrivate(this));
    setIcon(icon);
    setTitle(title);
    setWindowType(type);
}

IBaseWindow::~IBaseWindow()
{
}

KIcon
IBaseWindow::icon() const
{
    return _d->icon;
}

void
IBaseWindow::setIcon(const KIcon& icon)
{
    if (icon.isNull()) {
        return;
    }

    _d->icon = icon;
}

void
IBaseWindow::setTitle(const QString& title)
{
    _d->title = title;
}

QString
IBaseWindow::title() const
{
    return _d->title;
}

void
IBaseWindow::setTabColor(Aki::IBaseWindow::TabColor color)
{
    Q_UNUSED(color);
}

void
IBaseWindow::setWindowType(Aki::IBaseWindow::WindowType type)
{
    _d->type = type;
}

Aki::IBaseWindow::TabColor
IBaseWindow::tabColor() const
{
    return _d->tabColor;
}

Aki::IBaseWindow::WindowType
IBaseWindow::type() const
{
    return _d->type;
}
