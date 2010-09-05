#ifndef AKI_SCRIPTMODULE_HPP
#define AKI_SCRIPTMODULE_HPP

#include <aki.hpp>
#include <QtCore/QObject>

namespace Aki
{
class IMainController;
class ISettingsPage;
class ScriptModule : public QObject
{
    Q_OBJECT
public:
    explicit ScriptModule(QObject* parent, Aki::IMainController* controller);
    ~ScriptModule();
    Q_INVOKABLE void addSettingsPage(Aki::ISettingsPage* page);
    Q_INVOKABLE void executeCommand(const QString& command);
    Q_INVOKABLE QObject* mainView() const;
    Q_INVOKABLE void printText(const QString& text);
    Q_INVOKABLE void removeSettingsPage(Aki::ISettingsPage* page);
private:
    Aki::IMainController* _mainController;
}; // End of class ScriptModule.
} // End of namespace Aki.

#endif // AKI_SCRIPTMODULE_HPP
