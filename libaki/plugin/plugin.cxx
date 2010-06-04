#include "plugin.hpp"
#include "aki.hpp"
#include "interfaces/imaincontroller.hpp"
using namespace Aki;

Plugin::Plugin(QObject* parent)
    : QObject(parent)
{
}

Plugin::~Plugin()
{
}

Aki::IMainController*
Plugin::mainController() const
{
    return _mainController;
}

void
Plugin::setMainController(Aki::IMainController* controller)
{
    _mainController = controller;
}
