#include "serverviewtab_p.hpp"
#include "ui/serverviewtab.hpp"
#include "ui/channelcontainer.hpp"
#include <QtGui/QSplitter>
using namespace Aki;

ServerViewTabPrivate::ServerViewTabPrivate(Aki::ServerViewTab* qq)
    : QObject(),
    socket(0),
    channelContainer(0),
    verticalSplitter(0),
    _q(qq)
{
}
