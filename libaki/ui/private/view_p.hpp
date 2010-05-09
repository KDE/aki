#ifndef AKI_VIEW_P_HPP
#define AKI_VIEW_P_HPP

#include <KDE/KIcon>
#include <QtCore/QObject>
#include <QtGui/QMenu>

class QStackedWidget;
namespace Aki
{
class MainWindow;
class View;
class ViewPrivate : public QObject
{
    Q_OBJECT
public:
    ViewPrivate(Aki::View* qq);
    Aki::MainWindow* mainWindow();
public:
    QStackedWidget* widgetStack;
private:
    Aki::View* _q;
}; // End of class ViewPrivate.
} // End of namespace Aki.

#endif // AKI_VIEW_P_HPP
