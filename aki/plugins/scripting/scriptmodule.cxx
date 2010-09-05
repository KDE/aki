#include "scriptmodule.hpp"
#include <interfaces/imaincontroller.hpp>
#include <interfaces/isettingspage.hpp>
using namespace Aki;

ScriptModule::ScriptModule(QObject* parent, Aki::IMainController* controller)
    : QObject(parent),
    _mainController(controller)
{
    setObjectName("Aki");
}

ScriptModule::~ScriptModule()
{
}

void
ScriptModule::addSettingsPage(Aki::ISettingsPage* page)
{
    _mainController->addSettingsPage(page);
}

void
ScriptModule::executeCommand(const QString& command)
{
#warning Implement executeCommand
    Q_UNUSED(command)
}

QObject*
ScriptModule::mainView() const
{
#warning Implement mainView
    return 0;
}

void
ScriptModule::printText(const QString& text)
{
#warning Implement printText
    Q_UNUSED(text);
}

void
ScriptModule::removeSettingsPage(Aki::ISettingsPage* page)
{
    _mainController->addSettingsPage(page);
}
