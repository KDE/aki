#ifndef AKI_VIEWTABBAR_P_HPP
#define AKI_VIEWTABBAR_P_HPP

#include <QtCore/QObject>
#include <QtCore/QPoint>

class QMenu;
namespace Aki
{
class ViewTabBar;
class ViewTabBarPrivate : public QObject
{
    Q_OBJECT
public:
    ViewTabBarPrivate(Aki::ViewTabBar* qq);
    void setupContextActions();
public Q_SLOTS:
    void customContextMenuRequested();
    void closeTabTriggered();
public:
    QMenu* contextMenu;
    QPoint contextMenuPosition;
private:
    Aki::ViewTabBar* _q;
}; // End of class ViewTabBarPrivate.
} // End of namespace Aki.

#endif // AKI_VIEWTABBAR_P_HPP
