#include "channelview_p.hpp"
#include "interfaces/ibasewindow.hpp"
#include "ui/channelview.hpp"
using namespace Aki;

ChannelViewPrivate::ChannelViewPrivate(Aki::ChannelView* qq)
    : QObject(),
    isSplitEnabled(false),
    _q(qq)
{
    channelList.clear();
}
