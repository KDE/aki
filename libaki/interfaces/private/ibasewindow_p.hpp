#ifndef AKI_IBASEWINDOW_P_HPP
#define AKI_IBASEWINDOW_P_HPP

#include "interfaces/ibasewindow.hpp"
#include <KDE/KIcon>
#include <QtCore/QObject>

namespace Aki
{
class IBaseWindow;
class IBaseWindowPrivate : public QObject
{
    Q_OBJECT
public:
    IBaseWindowPrivate(Aki::IBaseWindow* qq);
public:
    KIcon icon;
    QString title;
    Aki::IBaseWindow::TabColor tabColor;
    Aki::IBaseWindow::WindowType type;
private:
    Aki::IBaseWindow* _q;
}; // End of class IBaseWindowPrivate.
} // End of namespace Aki.

#endif // AKI_IBASEWINDOW_P_HPP
