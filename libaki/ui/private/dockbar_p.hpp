#ifndef DOCKBAR_P_HPP
#define DOCKBAR_P_HPP

#include <QtCore/QObject>

class QAction;
class QSignalMapper;
namespace Aki
{
class DockBar;
class DockButton;
class DockWidget;
class DockBarPrivate : public QObject
{
    Q_OBJECT
public:
    DockBarPrivate(Aki::DockBar* qq);
    void createAction(Aki::DockButton* button);
public Q_SLOTS:
    void buttonTriggered(QAction* action);
    void dockAutoHideStateChanged(bool checked);
    void dockShow(QWidget* dock);
    void dockHide(QWidget* dock);
public:
    QSignalMapper* showDockMapper;
    QSignalMapper* hideDockMapper;
private:
    Aki::DockBar* _q;
}; // End of class DockBarPrivate.
} // End of namespace DockBarPrivate.

#endif // DOCKBAR_P_HPP
