#ifndef AKI_VIEW_P_HPP
#define AKI_VIEW_P_HPP

#include <QtCore/QObject>

class QStackedWidget;
class QVBoxLayout;

namespace Aki
{
class IBaseWindow;
class MainWindow;
class TabBar;
class View;
class ViewPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ViewPrivate(Aki::View* qq);
public:
    QStackedWidget* tabWidget;
    Aki::TabBar* tabBar;
    QVBoxLayout* layout;
    Aki::MainWindow* mainWindow;
public:
    void currentChanged(int index);
    void moveTab(int oldIndex, int newIndex);
    void tabCloseRequested(int index);
    void wheelDelta(int delta);
private:
    Aki::View* _q;
}; // End of class ViewPrivate.
} // End of namespace Aki.

#endif // AKI_VIEW_P_HPP
