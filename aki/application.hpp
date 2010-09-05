#ifndef AKI_APPLICATION_HPP
#define AKI_APPLICATION_HPP

#include <KDE/KUniqueApplication>

namespace Aki
{
class AkiWindow;
class Application : public KUniqueApplication
{
    Q_OBJECT
public:
    Application();
    ~Application();
    virtual int newInstance();
private:
    
private:
    Aki::AkiWindow* _mainWindow;
}; // End of class Application.
} // End of namespace Aki.

#endif // AKI_APPLICATION_HPP
