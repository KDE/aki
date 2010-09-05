#include "tabbar.hpp"
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QMimeData>
#include <QtGui/QDrag>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
using namespace Aki;

TabBar::TabBar(QWidget* parent)
    : KTabBar(parent)
{
    setSelectionBehaviorOnRemove(KTabBar::SelectLeftTab);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setTabsClosable(true);
    setMovable(true);
}

TabBar::~TabBar()
{
}

void TabBar::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MidButton) {
        QDrag* drag = new QDrag(this);
        QMimeData* data = new QMimeData;

        QByteArray encoded;
        QDataStream stream(&encoded, QIODevice::WriteOnly);

        data->setData("application/aki-tab", encoded);
        drag->setMimeData(data);
        drag->start();
        return;
    }
    KTabBar::mousePressEvent(event);
}

void TabBar::wheelEvent(QWheelEvent* event)
{
    if (count() < 2) {
        return;
    }

    int page = currentIndex();
    if (event->delta() < 0) {
        page = (page + 1) % count();
    } else {
        page--;
        if (page < 0) {
            page = count() - 1;
        }
    }
    setCurrentIndex(page);
    //KTabBar::wheelEvent(event);
}
