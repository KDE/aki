#include "channelview.hpp"
#include "interfaces/ibasewindow.hpp"
#include "ui/private/channelview_p.hpp"
#include <QtGui/QDragEnterEvent>
using namespace Aki;

const QString MIME("application/aki-tab");

ChannelView::ChannelView(QWidget* parent)
    : KTabWidget(parent)
{
    _d.reset(new Aki::ChannelViewPrivate(this));
}

ChannelView::~ChannelView()
{
}
