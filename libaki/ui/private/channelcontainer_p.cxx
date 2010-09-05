#include "channelcontainer_p.hpp"
#include "ui/channelcontainer.hpp"
#include <QtGui/QSplitter>
using namespace Aki;

ChannelContainerPrivate::ChannelContainerPrivate(Aki::ChannelContainer* q)
    : horizontalSplitter(0),
    _q(q)
{
}
