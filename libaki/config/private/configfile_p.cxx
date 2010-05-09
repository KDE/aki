#include "configfile_p.hpp"
#include "config/configfile.hpp"
using namespace Aki;

ConfigFilePrivate::ConfigFilePrivate(Aki::ConfigFile* qq)
    : QObject(),
    config(0),
    _q(qq)
{
}
