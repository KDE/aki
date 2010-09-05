#include "channelcontainer.hpp"
#include "private/channelcontainer_p.hpp"
#include "ui/channelview.hpp"
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
using namespace Aki;

ChannelContainer::ChannelContainer(QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::ChannelContainerPrivate(this));
    _d->horizontalSplitter = new QSplitter(Qt::Horizontal);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_d->horizontalSplitter);
    setLayout(layout);
}

ChannelContainer::~ChannelContainer()
{
}

void
ChannelContainer::addChannelView(Aki::ChannelView* view)
{
    Q_ASSERT(view);
    _d->horizontalSplitter->addWidget(view);
}

Aki::ChannelView*
ChannelContainer::channelView(int index) const
{
    return qobject_cast<Aki::ChannelView*>(_d->horizontalSplitter->widget(index));
}

int
ChannelContainer::count() const
{
    return _d->horizontalSplitter->count();
}

int
ChannelContainer::indexOf(Aki::ChannelView* view) const
{
    const int count = _d->horizontalSplitter->count();
    for (int i = 0; i < count; ++i) {
        Aki::ChannelView* tmp =
            qobject_cast<Aki::ChannelView*>(_d->horizontalSplitter->widget(i));
        if (tmp == view) {
            return i;
        }
    }

    return 0;
}

void
ChannelContainer::insertChannelView(int index, Aki::ChannelView* view)
{
    Q_ASSERT(view);

    if (index <= 0) {
        _d->horizontalSplitter->insertWidget(0, view);
        return;
    } else if (index >= _d->horizontalSplitter->count()) {
        _d->horizontalSplitter->addWidget(view);
        return;
    }

    _d->horizontalSplitter->insertWidget(index, view);
}
