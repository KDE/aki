#include "dockbutton.hpp"
#include "ui/dockwidget.hpp"
#include "private/dockbutton_p.hpp"
#include <QtGui/QPaintEvent>
#include <QtGui/QStyleOptionToolButton>
#include <QtGui/QStylePainter>
using namespace Aki;


DockButton::DockButton(QWidget* parent)
    : QToolButton(parent)
{
    _d.reset(new Aki::DockButtonPrivate(this));
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

DockButton::~DockButton()
{
}

Aki::DockWidget* DockButton::dock()
{
    return _d->dock;
}

Aki::DockWidget* DockButton::dock() const
{
    return _d->dock;
}

bool DockButton::isAutoHide() const
{
    return _d->hideable;
}

Qt::Orientation DockButton::orientation() const
{
    return _d->orientation;
}

void DockButton::paintEvent(QPaintEvent* e)
{
    if (orientation() == Qt::Horizontal) {
        QToolButton::paintEvent(e);
        return;
    }

    QStyleOptionToolButton option;
    initStyleOption(&option);

    QTransform tf;

    if (orientation() == Qt::Vertical) {
        tf = tf.rotate(90);
    } else {
        tf = tf.rotate(-90);
    }

    option.icon = icon();
    option.rect.setSize(QSize(option.rect.height(), option.rect.width()));

    QPixmap pix(option.rect.width(), option.rect.height());

    QStylePainter painter(&pix, this);
    painter.fillRect(pix.rect(), option.palette.background());
    painter.drawComplexControl(QStyle::CC_ToolButton, option);
    painter.end();

    QPainter p(this);

    if (orientation() == Qt::Vertical) {
        p.translate(0, height());
        p.rotate(-90);
    } else {
        p.translate(width(), 0);
        p.rotate(90);
    }

    p.drawPixmap(0, 0, pix);
}

void DockButton::setAutoHide(bool hideable)
{
    _d->hideable = hideable;
}

void DockButton::setDock(Aki::DockWidget* dock)
{
    _d->dock = dock;
    Q_ASSERT(_d->dock);
    connect(dock, SIGNAL(mouseLeave()),
            SIGNAL(mouseLeave()));
    setIcon(_d->dock->icon());
    setText(_d->dock->title());
}

void DockButton::setOrientation(Qt::Orientation orientation)
{
    _d->orientation = orientation;
}

QSize DockButton::sizeHint() const
{
    ensurePolished();

    QStyleOptionToolButton option;
    initStyleOption(&option);

    QFontMetrics fm = fontMetrics();
    const int charWidth = fm.width(QLatin1Char('x'));

    QSize textSize;

    if (toolButtonStyle() != Qt::ToolButtonIconOnly) {
        textSize = fm.size(Qt::TextShowMnemonic, option.text);
        textSize.rwidth() += 2 * charWidth;
    }

    const int spacing = 2;
    int iconWidth = 0;
    int iconHeight = 0;

    if (toolButtonStyle() != Qt::ToolButtonTextOnly) {
        if (orientation() == Qt::Vertical) {
            iconWidth = option.iconSize.height();
            iconHeight = option.iconSize.width();
        } else {
            iconWidth = option.iconSize.width();
            iconHeight = option.iconSize.height();
        }
    }

    int width = 4 + textSize.width() + iconWidth + spacing;
    int height = 4 + qMax(textSize.height(), iconHeight) + spacing;

    if (orientation() == Qt::Vertical) {
        return QSize(height, width);
    }

    return QSize(width, height);
}
